// safe_exec_win.cpp
// -- safe execution of child process
// csd2180/csd2182 9/23
//
// compile with:
//   cl /W3 /EHsc safe_exec_win.cpp kernel32.lib

#include <iostream>
#include <windows.h>
using namespace std;


int main(void) {
  HANDLE pipe_read, pipe_write, old_std_out;
  
  /*
	 Setting the security attributes here.
	 In particular, setting the InheritHandle to TRUE
	 makes the pipe that's created inheritable by
	 the child process.
  */
  SECURITY_ATTRIBUTES sec_attrib;
  sec_attrib.nLength = sizeof(sec_attrib);
  sec_attrib.bInheritHandle = TRUE;
  sec_attrib.lpSecurityDescriptor = 0;
  
  /*
	Now pipe is created
    pipe_read is reading end
    pipe_write is the writing end
  */
  
  CreatePipe(&pipe_read,&pipe_write,&sec_attrib,0);
  
  
  /*
	1. STD_OUTPUT_HANDLE is the handle object pointing to the stdout
	2. Usually, stdout is set by default to be printing to console.
	3. However, SetStdHandle(STD_OUTPUT_HANDLE, pipe_write) makes it possible
	   for stdout to be writing to whatever is pointed to by the pipe_write handle
	   object.
	4. In this case, the pipe_write object is the writing end of the pipe.
	5. We need to save a handle object that's printing to console before
	   setting STD_OUTPUT_HANDLE to be something else. 
	6. This is because we need to set STD_OUTPUT_HANDLE back to printing to
	   console after creating the child process.
  */
  
  old_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
  SetStdHandle(STD_OUTPUT_HANDLE,pipe_write);

  
  /*
	Setting up the process info object and startupinfo. 
	Zeroing out the memory in the struct is important so that
	CreateProcess will be successful. Otherwise, rubbish values 
	inside the two structs may cause CreateProcess to 
	perform in unexpected ways. 
  */
  PROCESS_INFORMATION proc_info;
  ZeroMemory(&proc_info,sizeof(proc_info));
  STARTUPINFO start_info;
  ZeroMemory(&start_info,sizeof(start_info));
  
  /*
	 Process Creation 
	 The 0s are the default values for creating the processes.
	 I refer you to the MSDN documentation for the details 
	 of these values.
  */
  CreateProcess("jack.exe",0,0,0,TRUE,0,0,0,&start_info,&proc_info);
  
  
  /*
	 As mentioned above, we set STD_OUTPUT_HANDLE to be 
	 writing to console again after creating the process.
  */
  SetStdHandle(STD_OUTPUT_HANDLE,old_std_out);

  
  /*
	WaitForSingleObject waits
	for the child process to complete. Note
	that proc_info is the struct that was initialized
	when we call CreateProcess. The 5000ms is the time
	you are willing wait for the child process to complete.
	After 5s, if WaitForSingleObject returns WAIT_TIMEOUT,
	that means that the child process has still not completed
	within that time. Otherwise, assume that the pipe is 
	written into and try to read it using ReadFile. 
  */
  if (WaitForSingleObject(proc_info.hProcess,5000) == WAIT_TIMEOUT) {
    cout << "process not responding: terminating" << endl;
    TerminateProcess(proc_info.hProcess,0); }
  else {
    char buffer[1024];
    DWORD bytes_read;
    ReadFile(pipe_read,buffer,sizeof(buffer),&bytes_read,0);
    buffer[bytes_read] = 0;
    cout << "output:" << endl;
    cout << buffer;
  }

  CloseHandle(proc_info.hProcess);
  CloseHandle(proc_info.hThread);
  CloseHandle(pipe_read);
  CloseHandle(pipe_write);

  return 0;
}
