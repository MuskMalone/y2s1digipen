#
# Makefile for lecture 8 (including Windows/Linux code)
#
#

WINDOWS_THREAD_DEMO1_SRCDIR= .
WINDOWS_INCLUDE1="C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\shared"
WINDOWS_INCLUDE2="C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\ucrt"
WINDOWS_INCLUDE3="C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\um"
WINDOWS_INCLUDE4="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include"

WINDOWS_LIB1="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x86"
WINDOWS_LIB2="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib"
WINDOWS_LIB3="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x86"

win_all:  win_thread_demo1 

	
win_thread_demo1: ${WINDOWS_THREAD_DEMO1_SRCDIR}/win_thread.cpp
	@cl /std:c++17 /EHsc /W3 /I ${WINDOWS_INCLUDE1} /I ${WINDOWS_INCLUDE2} /I ${WINDOWS_INCLUDE3} /I ${WINDOWS_INCLUDE4} /Fe${WINDOWS_THREAD_DEMO1_SRCDIR}/win_thread.exe ${WINDOWS_THREAD_DEMO1_SRCDIR}/win_thread.cpp /link  /LIBPATH:${WINDOWS_LIB1} /LIBPATH:${WINDOWS_LIB2} /LIBPATH:${WINDOWS_LIB3}
	
	
win_clean:
	@del *.exe
	@del *.obj
	
