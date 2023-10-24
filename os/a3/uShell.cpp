#include "uShell3.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>

/***************************************************************************/
/*!
\brief
It sets PID to 0 and bActive to false.
*/
/***************************************************************************/
uShell3::ProcessInfo::ProcessInfo()
{
    PID = 0;
    bActive = false;
}

/***************************************************************************/
/*!
\brief
It sets PID to id and bActive to state.
*/
/***************************************************************************/
uShell3::ProcessInfo::ProcessInfo(int id, bool state)
{
    PID = id;
    bActive = state;
}

/***************************************************************************/
/*!
\brief
This function does finish command. It checks for finish command without parameters
and indicates the error. If the finish command is with the correct parameters, it
converts the argument to process id to wait for that process. It checks if the 
waited process ID is valid in the m_bgProcessList and also checks if the process to
be waited for is already done by checking bActive for the process. If everything
is ok, it waits and gets the returned status (using Waitpid()). At the end it prints
the status for the process and then set the process active flag to false
*/
/***************************************************************************/
void uShell3::finish(TokenList const& tokens)
{
    if (tokens.size() < 2)
    {
        std::cout << "Error: no such process index." << std::endl;
        return;
    }
    int idx {stoi(tokens[1])};
    if (!(static_cast<size_t>(idx) < m_bgProcessList.size())){
        //std::cout << idx << " " << m_bgProcessList.size();
        std::cout << "Error: no such process index." << std::endl;
        return;

    }
    if (!m_bgProcessList[idx].bActive){
        std::cout << "Process Index " << idx << " process " << m_bgProcessList[idx].PID << " is no longer a child process." << std::endl;
        return;
    }
    int status;
    waitpid(m_bgProcessList[idx].PID, &status, 0);
    m_bgProcessList[idx].bActive = false;
    std::cout << "process " << m_bgProcessList[idx].PID << " exited with exit status " << ((WIFEXITED(status)) ? WEXITSTATUS(status) : status) << std::endl;

    
}

/***************************************************************************/
/*!
\brief
This function determines whether the command exists or not. It gets the executable
file name. If the file carries an absolute path, it checks whether the file exists.
Otherwise, it sets up the environment variables list and loop through the list to
find whether the file exists.
*/
/***************************************************************************/
bool uShell3::exist(TokenList const& tokenList, unsigned startParam, unsigned endParam)
{
    endParam = endParam;
    if (tokenList[startParam][0] == '/' || tokenList[startParam].substr(0, 2) == "./")
        return (!access(tokenList[startParam].c_str(), F_OK));
    std::string path;
    path = m_vars["PATH"];
    std::istringstream iss{path};
    std::string command{};
    while(std::getline(iss, command, ':')){
        if (!access((command + "/" + tokenList[startParam]).c_str(), F_OK)) return true;
    }
    return false;

}

/***************************************************************************/
/*!
\brief
The function checks for rare case where first token is a pipe, where nothing will be done.
It flushes the buffer and then count the number of pipes in the token list. It adds pipe
information to a list typed of std :: vector < P ipeInfo > and creates the pipe (using
pipe(). You may save the position of the pipe in the token list so that it can be used to
separate the arguments. Once it has obtained the number of processes required, it loops
over for further checking. It identifies where the beginning and ending position of the
parameters in the token list. Otherwise if there is only a single process, it checks whether
there is a background process token. It reports the syntax error if there is an empty string
between the pipes. The function checks whether the command in the given token list with
the specified starting and ending position of the parameters exists by calling exist() and
reports the error accordingly. It returns if there is an error.

Otherwise, it starts to create the child processes using fork(). For each fork(), if
background process is specified, the parent does not wait and adds the child to the list by
pushing into m bgProcessList. Please note you need to print out the status for the child
process. 

Otherwise if no background process is required, the parent just adds the child
PID to the list of the processes with piping. For the child process, it sets the arguments
to the input in the token list given to the process and call execute(). If the process is
not the first one to be created, it closes stdin and replaces with pipe in. If the process is
not the last one, it closes stdout and replaces with pipe out. It closes all pipes that are
not needed anymore when calling execute() given in uShell2.h. Then the child exits by
calling exit(EXIT FAILURE). The parent process should close all pipes before waiting for
all the child processes to be done.
*/
/***************************************************************************/
void uShell3::doExternalCmd(TokenList const& tokenList){

    if (tokenList.front() == "|") return;
    std::cout << std::flush;
    std::vector<PipeInfo> piv{};
    int pid;
    //get the list of pipes
    for (size_t i {}; i < tokenList.size(); ++i) {
        PipeInfo pi{};
        pi.posInToken = static_cast<unsigned>(i);
        if (tokenList[i] == "|") {
            if (i == tokenList.size() - 1 || tokenList[i + 1] == "|" || tokenList[i - 1] == "|" ) {
                std::cout << "Error: syntax error" << std::endl;
                return;
            }
            piv.emplace_back(pi);
            if (pipe(piv.back().descriptor) == -1){
                _exit(EXIT_FAILURE);
            }
        }
    }
    if (!piv.empty()){
        for (size_t i{}; i <= piv.size(); ++i){
            unsigned start{ }, end{ };
            if (i == 0)
                start = 0, end = piv[i].posInToken;
            else if (i == piv.size())
                start = piv[i-1].posInToken + 1, end = tokenList.size();
            else
                start = piv[i-1].posInToken + 1, end = piv[i].posInToken;

            if(!exist(tokenList,start,end)){
                std::cout << "Error: " << tokenList[start] <<" cannot be found" << std::endl;
                return;
            }
        }
        for (size_t i{}; i <= piv.size(); ++i){

            int pid1 = fork();
            if (!pid1){
                unsigned start{ }, end{ };
                if (i == 0){ //front
                    close(piv[i].descriptor[PipeInfo::IN_DESCRIPTOR]);//closed unused read
                    dup2(piv[i].descriptor[PipeInfo::OUT_DESCRIPTOR], STDOUT_FILENO); //dup2 std::cout
                    close(piv[i].descriptor[PipeInfo::OUT_DESCRIPTOR]);
                    start = 0, end = piv[i].posInToken;
                }
                else if (i == piv.size()){ //end
                    close(piv[i-1].descriptor[PipeInfo::OUT_DESCRIPTOR]);//closed unused read
                    dup2(piv[i-1].descriptor[PipeInfo::IN_DESCRIPTOR], STDIN_FILENO); //dup2 std::cout
                    close(piv[i-1].descriptor[PipeInfo::IN_DESCRIPTOR]);
                    start = piv[i-1].posInToken + 1; end = tokenList.size();
                }
                else{

                    close(piv[i].descriptor[PipeInfo::IN_DESCRIPTOR]);//closed unused read
                    dup2(piv[i].descriptor[PipeInfo::OUT_DESCRIPTOR], STDOUT_FILENO); //dup2 std::cout
                    close(piv[i].descriptor[PipeInfo::OUT_DESCRIPTOR]);
                    
                    close(piv[i-1].descriptor[PipeInfo::OUT_DESCRIPTOR]);//closed unused read
                    dup2(piv[i-1].descriptor[PipeInfo::IN_DESCRIPTOR], STDIN_FILENO); //dup2 std::cout
                    close(piv[i-1].descriptor[PipeInfo::IN_DESCRIPTOR]);
                    start = piv[i-1].posInToken + 1; end = piv[i].posInToken;
                }
                
                execute(tokenList, start, end);
                _exit(0);
            }else{
                
                close(piv[(i == piv.size()) ? (i - 1) : i].descriptor[PipeInfo::OUT_DESCRIPTOR]);
                wait(NULL);
            }
        }
        //close(piv[back])
    }
    else{
        if (!exist(tokenList, 0, tokenList.size())) {
            std::cout << "Error: " << tokenList[0] <<" cannot be found" << std::endl;
            return;
        }
        if (tokenList.back() == "&"){   
            pid = fork();
            if (0 == pid){
                execute(tokenList, 0, tokenList.size() - 1);
            }
            else{
                m_bgProcessList.emplace_back(ProcessInfo{pid, true});
                std::cout << '[' << m_bgProcessList.size() - 1 << "] process " << pid << std::endl;
            }
        }else{
            pid = fork();
            if (0 == pid){
                execute(tokenList, 0, tokenList.size());
                _exit(0);
            }else{
                wait(0);
            }
        }
    }
}

/***************************************************************************/
/*!
\brief
It sets function pointers for finish in m_InternalCmdList3.
*/
/***************************************************************************/
uShell3::uShell3(bool bFlag) : uShell2(bFlag){
    // Set function pointers for finish
    m_internalCmdList3["finish"] = &uShell3::finish;
}

/***************************************************************************/
/*!
\brief
This function is enhanced based on the assignment A1.1. It has a loop and firstly check
whether exit command is called. If so, it stops. Otherwise it prints out the prompt, with
the right arrow. Then it gets user input. If there are no more lines from the input, it exits
from the program (assume it gets re-directed input from the files). Otherwise, it clears the
input buffer for next input. Obviously, it needs to skip if there is no input (e.g. empty line).
It starts to tokenize the input otherwise. After this, it prints the input if verbose mode is
set. It replaces all variables if possible. If the function call for replacement replaceV ars()
returns false, an error has occurred, it continues to next line of input. Please note that
replacement also clears comments, so we have to check if the result is empty. Next, it
finds if it is an internal command in the first list m internalCmdList. If so, it activates
the internal command. Otherwise, it finds in the second list m internalCmdList2 and
activates the internal command if there is a match. Else, it continues to search the
internal command in the third list m internalCmdList3. If the search for the
internal command fails, it regards the command as external command and does
the external command accordingly.1 Please note that if the next char is EOF, we
should end the loop to exit. Outside the loop, it returns exit code m exitCode.
*/
/***************************************************************************/
int uShell3::run(){

    while (!m_exit){

        std::cout << m_prompt << ">";
        std::string input{}; TokenList tokenList{};
        if (!getInput(input)) break;
        if (m_verbose) printVerbose(input);
        tokenize(input, tokenList);
        if(!replaceVars(tokenList)) continue;
        if (tokenList.empty()) continue;
        if (m_internalCmdList.find(tokenList[0]) != m_internalCmdList.end())
            (this->*(m_internalCmdList[tokenList[0]]))(tokenList);
		else if (m_internalCmdList2.find(tokenList[0]) != m_internalCmdList2.end())
			(this->*(m_internalCmdList2[tokenList[0]]))(tokenList);
        else if (m_internalCmdList3.find(tokenList[0]) != m_internalCmdList3.end())
			(this->*(m_internalCmdList3[tokenList[0]]))(tokenList);
		else {
            doExternalCmd(tokenList);
        }
    }
    return m_exitCode;
}
