/* execl.c
** int execl(const char *path, const char *arg, ...);
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
** take the environment for the new process image from the external variable 
** environ in the calling process.
** csd2180/2182 1/9/23
*/


#include <unistd.h>
#include <stdio.h>

int main(void)
{
    execl("/usr/bin/grep", "grep", "-r","execlp", "execlp.c", NULL);
    fprintf(stderr, "Oops!\n");
    return -1;
}

