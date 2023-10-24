PRG=shell.exe
GCC=g++
GCCFLAGS=-std=c++17 -O2 -pedantic-errors -Wall -Wextra -Werror

DRIVER0=uShell_ref.obj uShell.cpp shellmain2.obj

VALGRIND_OPTIONS=-q --leak-check=full

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0) $(GCCFLAGS)

run:
	./$(PRG) > out.txt
clean:
	rm -f *.exe *.tds *.o *.obj *manifest* studentout* diff*
