#include "uShell.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
/************************************************************************/ /*!
\ brief
Get one line of input from std::cin . Removes the trailing \r i f any 
\param input
	The input	string	to take the user	input by reference
\return return	false if	encountering the EOF
*/
/************************************************************************/ 
bool uShell::getInput ( std::string & input ) const {
    if (!std::getline(std::cin, input, '\n')) return false;
    if (input.back() == '\r') input.pop_back();

    return true;
}

/************************************************************************/ /*!
\ brief
Print the user input without printing the starting and the trailing spaces . 
\param input
	The input	string
*/
/************************************************************************/ 
void uShell::printVerbose ( std::string	const & input ){
    size_t lhs = input.find_first_not_of(" \n\t\r"), rhs = input.find_last_not_of(" \n\t\r");
    std::cout << input.substr(lhs,  (rhs - lhs) + 1) << std::endl;
}

/************************************************************************/ /*!
\ brief
Tokenize the input	string	into a	l i s t	of	strings	which are	separated by spaces .
\param input
	The input	string	given by the user
\param tokenList
	Token	l i s t	to	f i l l	up ,	passed by reference	to add strings	to	it
*/
/************************************************************************/ 
void uShell::tokenize ( std::string	const & input ,	TokenList & tokenList ) const {
    std::istringstream ss{input};
    std::string s{};
    // std::cout << "tokenize start\n";
    // std::cout << input << std::endl;
    while (ss >> s) {
        // std::cout << s << std::endl;
        tokenList.push_back(s);
        
    }
    // std::cout << "tokenize end\n";

}
bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

/************************************************************************/ /*!
\ brief
Search and replace from the map all	variables	in the token	l i s t	with the value 
\param tokenList
Token	l i s t	to	replace	the	variables	with 
\return
	Boolean value to decide whether there	is a invalid	variable	found in the	string .
	Returning true means all	valid .	Otherwise ,
	Returning	false means there	is a invalid	variable	( abort the search ).
*/
/************************************************************************/ 
bool uShell::replaceVars ( TokenList & tokenList ) const {

    //remove comments
    for (size_t i{}; i < tokenList.size(); ++i){
        auto const& token {tokenList[i]};
        if (token == "#") {
            tokenList.erase(std::next(tokenList.begin(), i), std::next(tokenList.begin(), tokenList.size())); 
            break;
        }
    }
    //look for dollar sign
    for (std::string & token: tokenList){
        //value of first is the idx of the last known position of the last char '{'
        size_t first {token.find_last_of("${")}, last{token.find_first_of('}')};
        //std::cout << std::endl<< first << " " << last <<std::endl;
        if (first != std::string::npos && last != std::string::npos){
            --first;
            std::string macro {token.substr(first + 2,last - first - 2 )};
            if (!isValidVarname(macro.c_str(), macro.c_str() + macro.length())) continue;
            if (m_vars.find(macro) != m_vars.end()){
                replace(token, token.substr(first, last-first+1), m_vars.at(macro)); //std::regex_replace(token, isvalid, m_vars.at(macro));
            }
            else{
                std::cout << "Error: " << macro << " is not a defined variable.\n";
                return false;
            }
        }
    }
    //std::for_each(tokenList.begin(), tokenList.end(), [](std::string s){ std::cout << s << std::endl; });
    return true;
}
/************************************************************************/ /*!
\ brief
Check whether each character is a number or digit from the given start and end of a character array .
\param start
	Pointer to the	start	of	string	to search from
\param end
	Pointer to the end of	string where search	stops
\return boolean value to	represent	i f	the	string	is	valid
Returning true means each character in the given string is valid . Otherwise , it returns false .
*/
/************************************************************************/ 
bool uShell::isValidVarname ( char const * start ,	char const * end) const {
    int i{};
    while (start + i != end)
        if (!isalnum(*(start + i++))) return false;
    
    return true;
}
/************************************************************************/ /*!
\ brief
Merge all the tokens from a given token list , separated by a space , into one single string . We expect return value optimization and move scemantics help . \param tokenList
	The token	l i s t	to take from
\param startPos
The starting	position /index to	start	merging from \return
The merged string
*/
/************************************************************************/ 
std::string uShell::mergeTokens( TokenList const & tokenList , unsigned startPos ) const {
    std::string out{};
    // std::cout << tokenList.size() << std::endl;
    for (auto it {tokenList.begin() + startPos}; it != tokenList.end(); it++){
        // std::cout << *it << std::endl;
        out+= *it + " ";
    }
    out.pop_back();
    return out;
}
/************************************************************************/ /*!
\ brief
	Echo the user	input
\param tokenList
	The l i s t	of data to read in
*/
/************************************************************************/ 
void uShell::echo ( TokenList const & tokenList ){
    if (tokenList.size() < 2) return;
    std::cout << mergeTokens(tokenList, 1) << std::endl;
}
/************************************************************************/ /*!
\ brief
	Set the value	of a variable
\param tokenList
	The l i s t	of	tokens to get the data value from
*/
/************************************************************************/ 
void uShell::setVar ( TokenList const & tokenList ){
    if (tokenList.size() < 3 && isValidVarname(tokenList[1].c_str(), tokenList[1].c_str() + tokenList[1].size())) {
        m_vars[tokenList[1]] = std::string{};
        return;
    }    
    m_vars[tokenList[1]] = mergeTokens(tokenList, 2);
}

/************************************************************************/ /*!
\ brief
	Creates the	class	object	of	uShell
\param bFlag boolean value to decide whether to echo input
*/
/************************************************************************/ 
uShell::uShell ( bool bFlag ) :m_internalCmdList{},  m_vars{}, m_prompt{"uShell"}, m_verbose{bFlag}, m_exit{false}, m_exitCode{0}{
    m_internalCmdList["echo"] = &uShell::echo;
    m_internalCmdList["setvar"] = &uShell::setVar;
}
/************************************************************************/ /*!
\ brief
	Public	function	for	external	call .	Execute in	loops and waits	for	input .
\return
	Exit code ,	of	the	exit command
*/
/************************************************************************/ 
int uShell::run (){
    while (!m_exit){
        
        std::cout << m_prompt << ">";
        std::string input{}; TokenList tokenList{};
        if (!getInput(input)) break;
        if (m_verbose) printVerbose(input);
        tokenize(input, tokenList);
        if(!replaceVars(tokenList)) continue;
        if (tokenList.empty()) continue;
        if (tokenList[0] == "exit"){
            m_exit = true;

            m_exitCode = (tokenList.size() > 2) ? std::stoi(tokenList[1]) : 0;

        }else if (tokenList[0] == "changeprompt"){
            m_prompt = mergeTokens(tokenList, 1);
        }else{
            if (tokenList.size() < 2) continue;
            if (m_internalCmdList.find(tokenList[0]) == m_internalCmdList.end())
                continue;
            else{
                (this->*(m_internalCmdList[tokenList[0]]))(tokenList);
            }
        }

    }
    return m_exitCode;
}