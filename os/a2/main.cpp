#include "uShell2.h"
#include <cstring>
int main(int argc, char *argv[]){
    
    uShell2 shell{((argc > 1) && (0 == strcmp(argv[1], "-v")))};
    return shell.run();
}