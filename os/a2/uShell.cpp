#include "uShell2.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h> // POSIX header file for 'wait' function
/************************************************************************/ /*!
\ brief
	Reads the	exit	code from the token	l i s t
\param tokenList
	Stores	the	exit	code that	will	be extracted
*/
/************************************************************************/ 
void uShell2::exit ( TokenList const & tokenList ){
	::exit((tokenList.size() > 2) ? std::stoi(tokenList[1]) : 0);
}
/************************************************************************/ /*!
\ brief
	Changes the prompt that	starts	at every	line
\param tokenList
	The l i s t	of	tokens where	shell	can	retrieve	the	string	value
*/
/************************************************************************/ 
void uShell2::changePrompt( TokenList const & tokenList ){
	if (tokenList.size() < 2) return;
	m_prompt = mergeTokens(tokenList, 1);
}
/************************************************************************/ /*!
\ brief
Sets up the arugment l i s t s required for calling the execvp function , and creates the child process
\param tokenList
	The l i s t	of	tokens to get the data value from
\param startParam
	The starting	token to parse data from the	l i s t
\param endParam
	1 pass the	last	token to parse data from the	l i s t
*/
/************************************************************************/ 
void uShell2::execute ( TokenList const & tokenList , unsigned startParam , unsigned endParam ){
	std::vector<const char *>args{};
	for (size_t i {startParam}; i < endParam; ++i){
		args.push_back(tokenList[i].c_str());
	}

	args.push_back(NULL);

	//pathname
	if (tokenList[0][0] == '/' || tokenList[0].substr(0, 2) == "./"){
		int res =execvp(tokenList[0].c_str(), const_cast<char**>(args.data())); 
		::exit(res);
	}
	//filename
	else{
		std::string path;
    	path = m_vars["PATH"];
		std::istringstream iss{path};
		std::string command{};
		int status{};
		while(std::getline(iss, command, ':')){
			int pid {};
			pid = fork();
			if (!pid){
				int res = execvp((command + '/' + tokenList[0]).c_str(), const_cast<char**>(args.data())); 
				// std::cout << res <<  " res" << std::endl;
				::exit(res);
			} else{
				waitpid(pid, &status, 0);
				// std::cout << WEXITSTATUS(status)<<  " stat"  << std::endl;
				if (WEXITSTATUS(status)!=255) break;
			}
		}
		::exit(WEXITSTATUS(status));
	}
}
/************************************************************************/ /*!
\ brief
Calls an external command using the passed in parameters . \param tokenList
	The l i s t	of	tokens to get the data value from
*/
/************************************************************************/ 
void uShell2::doExternalCmd( TokenList const & tokenList ){
	int pid {getpid()};
	std::cout << std::flush;

	pid = fork();
	if (0 == pid){
		execute(tokenList, 0, tokenList.size());
	}else{
		int status;
		wait(&status);
		//std::cout << WEXITSTATUS(status) << std::endl;
			std::cout << std::flush;

		if (WEXITSTATUS(status) == 255)
			std::cout << "Error: " << tokenList[0] << " cannot be found\n";
	}
}
/************************************************************************/ /*!
\ brief
	Creates the	class	object	of	uShell2
\param bFlag boolean value to decide whether to echo input
*/
/************************************************************************/ 
uShell2::uShell2(bool bFlag) : uShell{bFlag}{
	m_verbose = bFlag;
	m_internalCmdList2["exit"] = &uShell2::exit;
	m_internalCmdList2["changeprompt"] = &uShell2::changePrompt;
	m_internalCmdList2["echo"] = &uShell2::echo;
    m_internalCmdList2["setvar"] = &uShell2::setVar;
}
/************************************************************************/ /*!
\ brief
Public	function	for	external	call .	Execute in	loops and waits	for	input . 
\return
	Exit code ,	of	the	exit command
*/
/************************************************************************/ 
int uShell2::run(){

    while (!m_exit){
        
        std::cout << m_prompt << ">";
        std::string input{}; TokenList tokenList{};
        if (!getInput(input)) break;
        if (m_verbose) printVerbose(input);
        tokenize(input, tokenList);
        if(!replaceVars(tokenList)) continue;
        if (tokenList.empty()) continue;
		if (m_internalCmdList2.find(tokenList[0]) == m_internalCmdList2.end())
			doExternalCmd(tokenList);
		else{
			(this->*(m_internalCmdList2[tokenList[0]]))(tokenList);
		}

    }
    return m_exitCode;

}
