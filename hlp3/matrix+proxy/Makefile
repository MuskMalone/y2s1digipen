PRG=shell.exe
PRG1=shell1.exe
GCC=g++
GCCFLAGS=-std=c++17 -O2 -pedantic-errors -Wall -Wextra -Werror

DRIVER0=matrix-proxy-driver.cpp
DRIVER1=-D_COPY matrix-proxy-driver.cpp

VALGRIND_OPTIONS=-q --leak-check=full

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0) $(GCCFLAGS)
test:
	$(GCC) -o $(PRG1) $(CYGWIN) $(DRIVER1) $(GCCFLAGS)
	./$(PRG1) 5 > out-test.txt
run:
	./$(PRG) 5 > out.txt
mem:
	@echo "running memory test"
	@echo "should run in less than 1500 ms"
	valgrind $(VALGRIND_OPTIONS) ./$(PRG) 5 > out.txt
	@echo "lines after this are memory errors";
clean:
	rm -f *.exe *.tds *.o *.obj *manifest* studentout* diff*
