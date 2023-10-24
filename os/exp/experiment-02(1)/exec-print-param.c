#include "unistd.h"
#include <stdio.h>
int main()
{
	execl("./print-param", "1", "2", "3", "4", "5",	NULL);	
}