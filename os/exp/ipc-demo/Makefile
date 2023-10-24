#
# Makefile for lecture 5 (including Windows/Linux code)
#
#

FORK_SHARE_SRCDIR=posix_mem_share_demo1
PROC_SHARE_SRCDIR=posix_mem_share_demo2
MSG_DEMO1_SRCDIR=posix_msg_demo1
MSG_DEMO2_SRCDIR=posix_msg_demo2
POSIX_PIPES_SRCDIR=posix_pipes_demo
POSIX_PIPES2_SRCDIR=posix_pipes_demo2
WINDOWS_PIPES_SRCDIR=windows_pipes_demo
WINDOWS_SHARE_DEMO1_SRCDIR=windows_mem_share_demo1
WINDOWS_SHARE_DEMO2_SRCDIR=windows_mem_share_demo2
POSIX_PIPES_SRCDIR=posix_pipes_demo
WINDOWS_INCLUDE1="C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\shared"
WINDOWS_INCLUDE2="C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\ucrt"
WINDOWS_INCLUDE3="C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\um"
WINDOWS_INCLUDE4="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include"

WINDOWS_LIB1="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x86"
WINDOWS_LIB2="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib"
WINDOWS_LIB3="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x86"
linux_all: fork_share proc_share  msg_demo1 msg_demo2 posix_pipe_demo posix_pipe_demo2

win_all:  win_share_demo1 win_share_demo2  windows_pipe_demo

fork_share: ${FORK_SHARE_SRCDIR}/fork_share.c
	@gcc -o posix_mem_share_demo1/fork_share posix_mem_share_demo1/fork_share.c

proc_share: ${PROC_SHARE_SRCDIR}/proc1_share.c ${PROC_SHARE_SRCDIR}/proc2_share.c ${PROC_SHARE_SRCDIR}/proc3_share.c 
	@gcc -W ${PROC_SHARE_SRCDIR}/proc1_share.c -o ${PROC_SHARE_SRCDIR}/proc1_share
	@gcc -W ${PROC_SHARE_SRCDIR}/proc2_share.c -o ${PROC_SHARE_SRCDIR}/proc2_share
	@gcc -W ${PROC_SHARE_SRCDIR}/proc3_share.c -o ${PROC_SHARE_SRCDIR}/proc3_share

msg_demo1: fork_msg

msg_demo2: create_msgq send_msg rcv_msg
	
posix_pipe_demo: ${POSIX_PIPES_SRCDIR}/pipes.c
	@g++ -o ${POSIX_PIPES_SRCDIR}/pipe_demo ${POSIX_PIPES_SRCDIR}/pipes.c

posix_pipe_demo2: ${POSIX_PIPES2_SRCDIR}/ls_pipe_sort.c
	@g++ -o ${POSIX_PIPES2_SRCDIR}/pipe_demo2 ${POSIX_PIPES2_SRCDIR}/ls_pipe_sort.c
	
fork_msg: ${MSG_DEMO1_SRCDIR}/fork_msg.c
	@g++ -o ${MSG_DEMO1_SRCDIR}/fork_msg  ${MSG_DEMO1_SRCDIR}/fork_msg.c
	
create_msgq: ${MSG_DEMO2_SRCDIR}/create_msgq.cpp
	@g++ -o ${MSG_DEMO2_SRCDIR}/create_msgq  ${MSG_DEMO2_SRCDIR}/create_msgq.cpp
	
send_msg: ${MSG_DEMO2_SRCDIR}/send_msg.cpp
	@g++ -o ${MSG_DEMO2_SRCDIR}/send_msg ${MSG_DEMO2_SRCDIR}/send_msg.cpp
	
rcv_msg: ${MSG_DEMO2_SRCDIR}/rcv_msg.cpp
	@g++ -o ${MSG_DEMO2_SRCDIR}/rcv_msg ${MSG_DEMO2_SRCDIR}/rcv_msg.cpp
	
win_share_demo1: ${WINDOWS_SHARE_DEMO1_SRCDIR}/win_share.cpp
	@cl /EHsc /W3 /I ${WINDOWS_INCLUDE1} /I ${WINDOWS_INCLUDE2} /I ${WINDOWS_INCLUDE3} /I ${WINDOWS_INCLUDE4} /Fe${WINDOWS_SHARE_DEMO1_SRCDIR}/win_share.exe ${WINDOWS_SHARE_DEMO1_SRCDIR}/win_share.cpp /link  /LIBPATH:${WINDOWS_LIB1} /LIBPATH:${WINDOWS_LIB2} /LIBPATH:${WINDOWS_LIB3}
	
win_share_demo2: win_share_mem1 win_share_mem2

win_share_mem1: ${WINDOWS_SHARE_DEMO2_SRCDIR}/win_share_mem1.cpp
	@cl /std=c++17 /EHsc /W3 /I ${WINDOWS_INCLUDE1} /I ${WINDOWS_INCLUDE2} /I ${WINDOWS_INCLUDE3} /I ${WINDOWS_INCLUDE4} /Fe${WINDOWS_SHARE_DEMO2_SRCDIR}/win_share_mem1.exe ${WINDOWS_SHARE_DEMO2_SRCDIR}/win_share_mem1.cpp /link  /LIBPATH:${WINDOWS_LIB1} /LIBPATH:${WINDOWS_LIB2} /LIBPATH:${WINDOWS_LIB3}
	
win_share_mem2: ${WINDOWS_SHARE_DEMO2_SRCDIR}/win_share_mem2.cpp
	@cl /EHsc /W3 /I ${WINDOWS_INCLUDE1} /I ${WINDOWS_INCLUDE2} /I ${WINDOWS_INCLUDE3} /I ${WINDOWS_INCLUDE4} /Fe${WINDOWS_SHARE_DEMO2_SRCDIR}/win_share_mem2.exe ${WINDOWS_SHARE_DEMO2_SRCDIR}/win_share_mem2.cpp /link  /LIBPATH:${WINDOWS_LIB1} /LIBPATH:${WINDOWS_LIB2} /LIBPATH:${WINDOWS_LIB3}
		
windows_pipe_demo: ${WINDOWS_PIPES_SRCDIR}/jack.cpp ${WINDOWS_PIPES_SRCDIR}/safe_exec_win.cpp
	@cl /EHsc /W3 /I ${WINDOWS_INCLUDE1} /I ${WINDOWS_INCLUDE2} /I ${WINDOWS_INCLUDE3} /I ${WINDOWS_INCLUDE4} /Fe${WINDOWS_PIPES_SRCDIR}/jack.exe ${WINDOWS_PIPES_SRCDIR}/jack.cpp /link /LIBPATH:${WINDOWS_LIB1} /LIBPATH:${WINDOWS_LIB2} /LIBPATH:${WINDOWS_LIB3}
	@cl /EHsc /W3  /I ${WINDOWS_INCLUDE1} /I ${WINDOWS_INCLUDE2} /I ${WINDOWS_INCLUDE3} /I ${WINDOWS_INCLUDE4} /Fe${WINDOWS_PIPES_SRCDIR}/safe_exec_win.exe	 ${WINDOWS_PIPES_SRCDIR}/safe_exec_win.cpp /link /LIBPATH:${WINDOWS_LIB1} /LIBPATH:${WINDOWS_LIB2} /LIBPATH:${WINDOWS_LIB3}
	
win_clean:
	@del *.exe
	@del *.obj
	
linux_clean:
	@rm -f fork_share
	@rm -f proc1_share
	@rm -f proc2_share
	@rm -f proc3_share
	@rm -f create_msgq
	@rm -f send_msg
	@rm -f rcv_msg
