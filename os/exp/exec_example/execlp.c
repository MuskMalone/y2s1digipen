/* execlp.c
** int execlp(const char *file, const char *arg, ...);
**
** The initial argument for these functions is the name of a file that is to be executed.
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
    execlp("env", "env", NULL);
    fprintf(stderr, "Oops!\n");
    return -1;
}

