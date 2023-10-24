#include "uShell.h"
#include <cstring>
int main(int argc, char *argv[]){
    
    uShell shell{((argc > 1) && (0 == strcmp(argv[1], "-v")))};
    return shell.run();
}