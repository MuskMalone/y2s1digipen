// win_share.cpp
// -- Shared memory between processes
// csd2180/csd2182 9/23

#include <iostream>
#include <cstring>
#include <conio.h>
#include <windows.h>
using namespace std;


int main(void) {
  char buffer[1024];
  HANDLE share = CreateFileMapping(INVALID_HANDLE_VALUE,0,
                   PAGE_READWRITE,0,sizeof(buffer),"win_share_data");
  if (GetLastError() != ERROR_ALREADY_EXISTS) {
    cout << "Message to write: ";
    cin.getline(buffer,sizeof(buffer));
    char *memory = (char*)MapViewOfFile(share,FILE_MAP_ALL_ACCESS,
                            0,0,sizeof(buffer));
    CopyMemory(memory,buffer,strlen(buffer)+1);
    UnmapViewOfFile(memory);
    _getch();
    return 0;
  }

  char *memory = (char*)MapViewOfFile(share,FILE_MAP_READ,
                          0,0,sizeof(buffer));
  CopyMemory(buffer,memory,strlen(memory)+1);
  cout << "Message read: " << buffer << endl;

  cout << "Enter anything to quit" << endl;
  _getch();
  UnmapViewOfFile(memory);
  CloseHandle(share);

  return 0;
}
