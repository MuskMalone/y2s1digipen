#include "A2.1-header-and-test-cases/new-coro-lib.h"
#include <sys/resource.h>
#include <errno.h>
#include <ucontext.h>
#include <deque>
#include <iostream>
#include <map>
#include <utility>
#define MEGABYTE 1 * 1024 * 1024

namespace CORO{
    enum ThreadState : int{
        RUNNING = 0, DONE
    };
}
namespace{

    struct TCB{
        CORO::ThreadID id;
        ucontext_t context;
        CORO::ThreadState state;
        bool stackAllocated;

    };
    struct Thread{
        TCB * tcb;
        void* (*routine)(void*);
        void* args;
        void* retval;
        bool firstRun{false};
    };

    //two Qs for scheduling
    std::deque<Thread> running;
    //std::deque<Thread> ready;
    std::deque<Thread> finish;
    std::map<CORO::ThreadID, std::pair<Thread*, void*>> waiting; // key is the thread they are waiting for, thread is itself
    Thread* currThread;

    TCB* get_new_tcb(){
        
        static unsigned id = 0;
        TCB* out{};
        if (out = (TCB*)malloc(sizeof(TCB)))
            out->id = id++;
        return out;
    }
    void destroy_tcb(TCB* tcb){
        if (tcb->stackAllocated){
            free(tcb->context.uc_stack.ss_sp);
        }
        free(tcb);
    }

    void thread_wrapper(){
        std::cout << "thread_wrapper\n";
        
        void* op = nullptr;
        if (currThread->routine) 
            op = currThread->routine(currThread->args);
        currThread->retval = op;
        std::cout << op << '\n';
        CORO::thread_exit(op);
    }
}
namespace CORO
{

void thd_init(){
    //init the main thread
    ::running.clear();
    //ready.clear();
    TCB *tcb{};
    Thread t{};
    if (!(tcb = get_new_tcb())) abort();
    if (getcontext(&tcb->context) == -1){
        std::cout << "init failed" << std::endl;

        destroy_tcb(tcb);
        abort();
    }
    t.tcb = tcb;
    ::running.push_back(t);
    tcb->state = RUNNING;
    currThread = &(running.front());
    tcb->context;
    std::cout << "makecontextstart\n";
    makecontext(&(tcb->context), thread_wrapper, 0);
     std::cout << "makecontextend\n";
    setcontext(&currThread->tcb->context);
}

ThreadID new_thd( void*(*routine)(void*), void*args){
    TCB* tcb;
    if (!(tcb = get_new_tcb())) abort();
    if (getcontext(&tcb->context) == -1){
        std::cout << "new thread getcontext failed" << std::endl;

        destroy_tcb(tcb);
        abort();
    }

    // Allocate memory

    void *stack;

    if ((stack = malloc(MEGABYTE)) == NULL) {
        std::cout << "stack alloc failed" << std::endl;

        abort();
    }

    // Update the thread control bock

    tcb->context.uc_stack.ss_flags = 0;
    tcb->context.uc_stack.ss_size = MEGABYTE;
    tcb->context.uc_stack.ss_sp = stack;
    tcb->stackAllocated = true;
    Thread t{tcb, routine, args};
    tcb->state = RUNNING;
    running.push_back(t);
    std::cout << "makecontext\n";
    makecontext(&(tcb->context), thread_wrapper, 0);

    if (running.size() == 1)
        setcontext(&currThread->tcb->context);
    return running.back().tcb->id;
}

void thread_exit(void * return_value){
    finish.push_back(*currThread);
    currThread->tcb->state = DONE;
    ::running.pop_front();
    if (running.size() > 0){
        currThread = &(running.front());
        setcontext(&currThread->tcb->context);  // also unblocks SIGPROF
    }
}
int wait_thread(ThreadID id, void **value){
    // Check if the thread has already terminated
    for (auto it = finish.begin(); it != finish.end(); ++it) {
        if (it->tcb->id == id) {
            *value = it->retval; // Set the return value
            //destroy_tcb(it->tcb); // Free up the resources
            finish.erase(it); // Remove the thread from the finish queue
            return WAIT_SUCCESSFUL;
        }
    }

    // If the thread hasn't terminated, suspend the current thread and move it to the waiting state
    waiting[id] = std::make_pair(currThread, value);
    //currThread->tcb->state = WAITING;

    // Schedule another thread to run
    if (!running.empty()) {
        currThread = &running.front();
        running.pop_front();
        running.push_back(*currThread);
        setcontext(&currThread->tcb->context);
    }

    // After the waited thread completes, the current thread will resume here
    // Retrieve the return value of the waited thread
    *value = waiting[id].second;

    // Remove the current thread from the waiting map
    waiting.erase(id);

    return WAIT_SUCCESSFUL;
}
void thd_yield(){
    std::cout << "thd_yield\n";
    // Save the context of the current thread
    if (getcontext(&currThread->tcb->context) == -1) {
        std::cerr << "Failed to get context for current thread." << std::endl;
        abort();
    }

    // Check if there are other threads in the ready queue
    if (!running.size() > 1) {
        // Move the current thread to the end of the ready queue
        running.push_back(*currThread);
        running.pop_front();

        // Schedule the next thread from the ready queue
        currThread = &running.front();
        running.pop_front();
        running.push_front(*currThread);

        // Switch to the next thread's context
        setcontext(&currThread->tcb->context);
        std::cout << "hello im context\n";
    }
    // If no other threads are ready, just continue executing the current thread
}
void push_value(void* pushed_value){

}
void pull_value(void** pulled_value){

}
}