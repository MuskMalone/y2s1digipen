/* execlp.c
** int execvp(const char *file, char *const argv[]);
**
** The initial argument for these functions is the name of a file that is to be executed.
** 
** provide an array of pointers to null-terminated strings that 
** represent the argument list available to the new program. 
** The first argument, by convention, should point to the filename 
** associated with the file being executed. 
** The array of pointers must be terminated by a NULL pointer.
**
** cs180 24/1/17
*/

#include <unistd.h>
#include <stdio.h>

int main(void)
{
    char *argv[] = { "grep", "-r", "execvp", "execvp.c", 0 };
    execvp(argv[0], &argv[0]);
    fprintf(stderr, "Oops!\n");
    return -1;
}

