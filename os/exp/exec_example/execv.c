/* execv.c
** int execv(const char *path, char *const argv[]);
**
** The initial argument for these functions is the name of a file that is to be executed.
**
** provide an array of pointers to null-terminated strings 
** that represent the argument list available to the new program. 
** The first argument, by convention, should point to 
** the filename associated with the file being executed. 
** The array of pointers must be terminated by a NULL pointer.
**
** csd2180/2182 1/9/23
*/

#include <unistd.h>
#include <stdio.h>

int main(void)
{
    char *argv[] = { "ls", "-l", NULL };
    execv("/bin/ls", argv);
    fprintf(stderr, "Oops!\n");
    return -1;
}

