#include <string>	//std::string
#include <vector>	//std::vector
#include <map>	//std : :map
#include "../a1/uShell.h" //base
/************************************************************************/ /*!
\ brief
uShell2	class .	Acts as a command prompt that	takes	in input and performs
commands based on the input .
*/
/************************************************************************/ 
class	uShell2	:	public	uShell
{ 
    protected : typedef void ( uShell2 ::* fInternalCmd2 )( TokenList const &);
/*! Store the l i s t of strings to function pointers of internal command in uShell2 , i . e . for changeprompt and exit */
	std::map<std::string ,	fInternalCmd2> m_internalCmdList2 ;
/************************************************************************/ /*!
\ brief
	Reads the	exit	code from the token	l i s t
\param tokenList
	Stores	the	exit	code that	will	be extracted
*/
/************************************************************************/ 
void	exit ( TokenList const & tokenList );
/************************************************************************/ /*!
\ brief
	Changes the prompt that	starts	at every	line
\param tokenList
	The l i s t	of	tokens where	shell	can	retrieve	the	string	value
*/
/************************************************************************/ 
void changePrompt( TokenList const & tokenList ); 
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
void execute ( TokenList const & tokenList , unsigned startParam , unsigned endParam );
/************************************************************************/ /*!
\ brief
Calls an external command using the passed in parameters . \param tokenList
	The l i s t	of	tokens to get the data value from
*/
/************************************************************************/ 
void doExternalCmd( TokenList const & tokenList ); 
public :
/************************************************************************/ /*!
\ brief
	Creates the	class	object	of	uShell2
\param bFlag boolean value to decide whether to echo input
*/
/************************************************************************/ 
uShell2 ( bool bFlag );
/************************************************************************/ /*!
\ brief
Public	function	for	external	call .	Execute in	loops and waits	for	input . 
\return
	Exit code ,	of	the	exit command
*/
/************************************************************************/ 
int run ();
};
