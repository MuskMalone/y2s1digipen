#include "uShell2.h"
#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void uShell2::exit(TokenList const & tokenList)
{
    if(tokenList.size() == 1) m_exitCode = 0;
    else 
    {
        int input = stoi(tokenList[0]); //string to integer
        m_exitCode = input;
    }
}

void uShell2::changePrompt(TokenList const & tokenList)
{
    if(tokenList.empty()) return;
    else
    {
        if(tokenList[0] == "changeprompt")
        {
            if(tokenList.size() > 1) m_prompt = tokenList[1];
        }
    }
}

void uShell2::execute(TokenList const & tokenList, unsigned startParam, unsigned endParam)
{ 
    if(tokenList.empty()) return;
    //create an array to store all the commands
    std::vector <const char*> command;

    //loop through tokenlist to get all the tokens 
    //put each command into the array
    for(unsigned i = startParam; i <= endParam; i++) 
    command.push_back(tokenList[i].c_str()); 

    //check for executable file name and command
    if(tokenList.size() >= 2)
    {
        if(tokenList[0][0] == '/' || tokenList[0].substr(0, 2) == "./")
        {
            //int execvp(const char *command to execute, char *const command to be passed[]);
            //convert vector command to array using data()
            //tokenlist[0] is already path with command
            execvp(command[0], const_cast<char**>(command.data()));
            //const_cast is to remove the const
        }
    }
    else
    {
        std::string s = m_vars["PATH"];
        std::stringstream ss {s};
        std::string directories;
        //join the path with the command
        while(std::getline(ss, directories, ':')) {
            //change std::string to const char* using c_str
            int pid = fork();
            if(pid == 0){
                execvp((directories + '/'+ std::string(command[0])).c_str(), const_cast<char**>(command.data()));
            }
        }
    }
    std::cerr << "Error: " << command[0] << " cannot be found" << std::endl;
}

void uShell2::doExternalCmd(TokenList const & tokenList)
{
    std::cout << std::flush; // Flush the standard output buffer
    int pid = fork(); // Spawn a child process
    // Child process
    if (pid == 0)
    {
        execute(tokenList, 0, tokenList.size());
        _Exit(0); // Exit child process
    }
    // Parent process
    else
    {
        // wait for the child to complete
        // waitpid(pid_t pid, int *status, int options);
        waitpid(pid, nullptr, 0);
    }
}

uShell2::uShell2 (bool bFlag):uShell(bFlag)
{
    // Set function pointers for changeprompt and exit
    // m_internalCmdList2 is a map that stores the command(interms of function ptrs) and their value
    m_internalCmdList2["changeprompt"] = &uShell2::changePrompt;
    m_internalCmdList2["exit"] = &uShell2::exit;
}

int uShell2::run()
{
    //a loop , check if the exit command is called
    while(!m_exit){
        std::cout << m_prompt << ">";//prints the right arrow
        std::string input_str{};; 
        //get the user input and if theres no more lines in the input
        if(!getInput(input_str)) {
            m_exit = true; //exit the program
            continue;
        }
        TokenList tokenList;
        tokenize(input_str, tokenList);//tokenize the input
        if(m_verbose) printVerbose(input_str); //print input in verbose mode
        //replace all variables
        //after clearing comments, check if result is empty
        if(!replaceVars(tokenList) || tokenList.empty()) continue;
        // Check for internal commands in m_internalCmdList
        const std::string& command = tokenList[0];
        if (command == "exit"){
            m_exit = true;
            if (tokenList.size() > 1) m_exitCode = std::stoi(tokenList[1]); //string to integer
            else m_exitCode = 0;
        }
        else if(command == "changeprompt" && tokenList.size() > 1) m_prompt = tokenList[1];
        else if(command == "echo" && tokenList.size() > 1) echo(tokenList);
        else if(command == "setvar") setVar(tokenList);
        else
        {
            // Check if internal commands is in mInternalCmdList2
            auto command_iterator = m_internalCmdList2.find(command);
            //while its not the end of the command list
            if (command_iterator != m_internalCmdList2.end())
            {
                //second of map mInternalCmdList2 is of type fInternalCmd2 which is a function pointer
                fInternalCmd2 functionPointer = command_iterator->second;
                //Call the function using the functionpointer and pass in tokenList as parameter
                (this->*functionPointer)(tokenList);
            }
            //If not found in both lists, treat as an external command 
            else doExternalCmd(tokenList);
        }
    }
    return m_exitCode;
}