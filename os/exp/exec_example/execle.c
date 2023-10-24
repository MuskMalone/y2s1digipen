/* execle.c
** int execle(const char *path, const char *arg, ..., char * const envp[]);
**
** The initial argument for these functions is the name of a file that is to be executed.
**
** describe a list of one or more pointers to null-terminated strings 
** that represent the argument list available to the executed program. 
** The first argument, by convention, should point to the filename associated 
** with the file being executed. The list of arguments must be terminated by a 
** NULL pointer, and, since these are variadic functions, this pointer must 
** be cast (char *) NULL.
**
** allow the caller to specify the environment of the executed program via the argument envp. 
** The envp argument is an array of pointers to null-terminated strings and must be 
** terminated by a NULL pointer. 
**
** duplicate the actions of the shell in searching for an executable file 
** if the specified filename does not contain a slash (/) character. 
** The file is sought in the colon-separated list of directory pathnames 
** specified in the PATH environment variable. If this variable isn't defined, 
** the path list defaults to the current directory followed by the list of directories 
** returned by confstr(_CS_PATH). (This confstr(3) call typically returns the value "/bin:/usr/bin".)
**
** csd2180/2182 1/9/23
*/

#include <unistd.h>
#include <stdio.h>

int main(void)
{
    const char *envp[] =
    {
        "HOME=/",
        "PATH=/bin:/usr/bin",
        "TZ=UTC+0",
        "USER=william.zheng",
        0
    };
    execle("/bin/date", "date", "--utc", NULL, envp);
    fprintf(stderr, "Oops!\n");
    return -1;
}

