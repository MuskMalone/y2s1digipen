PRG=shell.exe
GCC=g++
GCCFLAGS=-std=c++17 -O2 # -pedantic-errors -Wall -Wextra -Werror

DRIVER0=uShell2_ref.obj uShell_ref.obj uShell.cpp shellmain3.obj
DRIVER1=uShell2_ref.obj uShell_ref.obj uShellTest.cpp shellmain3.obj

VALGRIND_OPTIONS=-q --leak-check=full

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0) $(GCCFLAGS)
test:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER1) $(GCCFLAGS)
run:
	./$(PRG) > out.txt
clean:
	rm -f *.exe *.tds *.o *.obj *manifest* studentout* diff*
