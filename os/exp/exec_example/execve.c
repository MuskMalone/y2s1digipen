/* execve.c
** int execvpe(const char *file, char *const argv[],char *const envp[]);
**
** The initial argument for these functions is the name of a file that is to be executed.
** 
** provide an array of pointers to null-terminated strings that 
** represent the argument list available to the new program. 
** The first argument, by convention, should point to the filename 
** associated with the file being executed. 
** The array of pointers must be terminated by a NULL pointer.
**
** back end for execl, exece, execlp, execv, execp
**
** L: individual parameters in the call (variable argument list):
** V: as an array of char* 
** E: The versions with an 'e' at the end let you additionally pass an array of char* 
** that are a set of strings added to the spawned processes environment 
** before the exec'ed program launches
** P: The versions with 'p' in there use the environment path variable to search 
** for the executable file named to execute.
**
** csd2180/2182 1/9/23
*/

#include <unistd.h>
#include <stdio.h>

int main(void)
{
    char *argv[] = { "/bin/sh", "-c", "date", 0 };
    char *envp[] =
    {
        "HOME=/",
        "PATH=/bin:/usr/bin",
        "TZ=UTC+0",
        "USER=william.zheng",
        0
    };
    execve(argv[0], &argv[0], envp);
    fprintf(stderr, "Oops!\n");
    return -1;
}

