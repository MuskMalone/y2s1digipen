PRG=prg.exe
PRGTEST=prgtest.exe
GCC=g++
GCCFLAGS=-std=c++20 -O2 # -pedantic-errors -Wall -Wextra -Werror

DRIVER0=new-coro-lib.cpp A2.1-header-and-test-cases/
DRIVER1=coro-lib.a A2.1-header-and-test-cases/

VALGRIND_OPTIONS=-q --leak-check=full

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0) $(GCCFLAGS)
1:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)context-saving-test-case.cpp $(GCCFLAGS)
2:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)double-wait-test.cpp $(GCCFLAGS)
3:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)fib-source-sink.cpp $(GCCFLAGS)
4:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)fib-threads.cpp $(GCCFLAGS)
5:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)matrix-multiply-test-case.cpp $(GCCFLAGS)
6:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)only-one-thread.cpp $(GCCFLAGS)
7:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)return-test1.cpp $(GCCFLAGS)
8:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)schedule-test.cpp $(GCCFLAGS)
9:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)switching-threads-example.cpp $(GCCFLAGS)
10:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0)wait-thread-example.cpp $(GCCFLAGS)

test1:
	$(GCC) -o $(PRGTEST) $(CYGWIN) $(DRIVER1)context-saving-test-case.cpp coro-lib.a $(GCCFLAGS)
test2:
	$(GCC) -o $(PRGTEST) $(CYGWIN) $(DRIVER1)double-wait-test.cpp coro-lib.a $(GCCFLAGS)
test3:
	$(GCC) -o $(PRGTEST) $(CYGWIN) $(DRIVER1)fib-source-sink.cpp coro-lib.a $(GCCFLAGS)
test4:
	$(GCC) -o $(PRGTEST) $(CYGWIN) $(DRIVER1)fib-threads.cpp coro-lib.a $(GCCFLAGS)
test5:
	$(GCC) -o $(PRGTEST) $(CYGWIN) $(DRIVER1)matrix-multiply-test-case.cpp coro-lib.a $(GCCFLAGS)
test6:
	$(GCC) -o $(PRGTEST) $(CYGWIN) $(DRIVER1)only-one-thread.cpp coro-lib.a $(GCCFLAGS)
test7:
	$(GCC) -o $(PRGTEST) $(CYGWIN) $(DRIVER1)return-test1.cpp coro-lib.a $(GCCFLAGS)
test8:
	$(GCC) -o $(PRGTEST) $(CYGWIN) $(DRIVER1)schedule-test.cpp coro-lib.a $(GCCFLAGS)
test9:
	$(GCC) -o $(PRGTEST) $(CYGWIN) $(DRIVER1)switching-threads-example.cpp coro-lib.a $(GCCFLAGS)
test10:
	$(GCC) -o $(PRGTEST) $(CYGWIN) $(DRIVER1)wait-thread-example.cpp coro-lib.a $(GCCFLAGS)

run:
	./$(PRG)
runtest:
	./$(PRGTEST)
	
clean:
	rm -f *.exe *.tds *.o *.obj *manifest* studentout* diff*
