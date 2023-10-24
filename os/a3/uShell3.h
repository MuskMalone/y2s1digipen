#include <string>
#include <vector>
#include <map>
#include "../a2/uShell2.h"

/***************************************************************************/
/* !
\brief
 uShell3 class. Acts as a command prompt that takes in input and performs
 commands based on the input.
*/
/***************************************************************************/
class uShell3 : public uShell2
{
protected:
typedef void (uShell3::*fInternalCmd3)(TokenList const&);

/*! Store the list of strings to function pointers of internal command
    in uShell3, i.e finish.*/
std::map<std::string, fInternalCmd3> m_internalCmdList3;

/***************************************************************************/
/* !
\brief
 Process info for background processes.
*/
/***************************************************************************/
struct ProcessInfo
{
    /*! The process ID given by OS */
    int PID;

    /*! The state of the process, whether it is active*/
    bool bActive;

    /***************************************************************************/
    /* !
    \brief
    Default constructor
    */
    /***************************************************************************/
    ProcessInfo();

    /***************************************************************************/
    /* !
    \brief
    Value constructor, set process id and current state
    \param id
    The process ID given by the OS
    \param state
    The state of the process to set
    */
    /***************************************************************************/
    ProcessInfo(int id, bool state);
};

/***************************************************************************/
/* !
\brief
 Process info for piping commands.
*/
/***************************************************************************/
struct PipeInfo{
    /*! File descriptor array. The first is READ/IN descriptor, the
        second WRITE/OUT descriptor.*/
    int descriptor[2];

    /*! The position of the pipe token "|" within the token list */
    unsigned posInToken;

    /*! Const value for READ/IN descriptor */
    static const int IN_DESCRIPTOR = 0;

    /*! Const value for WRITE/OUT descriptor */
    static const int OUT_DESCRIPTOR = 1;
};

/*! Store the list of background processes */
std::vector<ProcessInfo> m_bgProcessList;

/*!
\brief
Determine whether the command exists or not.
\param tokenList
The list of tokens to get the command and arguments from
\param startParam
The starting token to parse data from the list
\param endParam
The last token to parse data from the list
*/
/***************************************************************************/
bool exist(TokenList const& tokenList,
                     unsigned startParam,
                     unsigned endParam);

/***************************************************************************/
/*!
\brief
Calls an external command using the passed in parameters.
\param tokenList
The list of tokens to get the data value from
*/
/***************************************************************************/
void doExternalCmd(TokenList const& TokenList);

/***************************************************************************/
/*!
\brief
Finish command: wait for a background process to finish
\param tokenList
The list of data to read in, the process ID to wait for
*/
/***************************************************************************/
void finish(TokenList const& tokenList);

public:
/***************************************************************************/
/*!
\brief
Creates the class object of uShell3
\param bFlag
boolean value to decide whether to echo input
*/
/***************************************************************************/
uShell3(bool bFlag);

/***************************************************************************/
/*!
\brief
Public function for external call. Execute in loops and waits for input.
\return
Exit code, of the exit command
*/
/***************************************************************************/
int run();

};