#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <map>
#include "thread.h"
#include "interrupt.h"

static std::queue<ucontext_t*> ready_queue;

static ucontext_t *uctx_init, *uctx_running, *uctx_yielded, *uctx_finished, *uctx_transfer;

static std::map<unsigned int, std::queue<ucontext_t*> > lock_queue_map;
static std::map<unsigned int, ucontext_t*> lock_values;

static std::map<unsigned int, std::map<unsigned int, std::queue<ucontext_t*> > > lockCV_queue;

static bool libinit_called = false;

/*  
 *  Function that controls user code and garbage collection of the finished threads.
 */

void clearMemory(){
    if (uctx_finished != 0) {
        /* std::cerr << "[DELETED: " << uctx_finished << "]" << std::endl; */
        delete (char*) uctx_finished->uc_stack.ss_sp;
        delete uctx_finished;
        uctx_finished = 0;
    }
}
void print_size() {
    std::cout << "Ready Queue Size = " << ready_queue.size() << std::endl;
    std::cout << "LockQ Map Size = " << lock_queue_map.size() << std::endl;
    std::cout << "LockValues Size = " << lock_values.size() << std::endl;
    std::cout << "LockCVQ Size = " << lockCV_queue.size() << std::endl;
}

void* magic_func(thread_startfunc_t func, void *arg){
    // Delete the finished thread
    clearMemory();
    // User Code Runs
    interrupt_enable();
    func(arg);
    interrupt_disable();
    clearMemory();
    // Exit if no more threads left.

    if (ready_queue.front() == NULL || ready_queue.size() == 0 || ready_queue.empty()) {
        //std::cerr << "[DELETED: " << uctx_running << "]" << std::endl;
	clearMemory();
        std::cout << "Thread library exiting." << std::endl;
        /* print_size(); */
        interrupt_enable();
        exit(EXIT_SUCCESS);
    } else {
        // If thread gets here, it is finished running.
        uctx_finished = uctx_running;

        // The next thread to run is taken from the ReadyQ
        uctx_running = ready_queue.front();
        ready_queue.pop();
        /* std::cerr << "magic func - setcontext: "<< uctx_running << std::endl; */
        swapcontext(uctx_finished, uctx_running);
	clearMemory();
    }
}

/*
 *  Starts the thread library. Only called once.
 */
int thread_libinit(thread_startfunc_t func, void *arg){
    interrupt_disable();
    /* print_size(); */
    uctx_finished = 0;
    if (libinit_called) {
        // libinit has already been called.
        interrupt_enable();
        return -1;
    } else {
        // set libinit to called
        libinit_called = true;
    }
    uctx_running = new (std::nothrow) ucontext_t;
    if (uctx_running == 0){
        interrupt_enable();
        // new returned a null pointer -- not enough memory
        return -1;
    }

    if (getcontext(uctx_running)) {
        interrupt_enable();
        return -1;
    }

    // Create stack and set parameters for new stack
    char *stack = new (std::nothrow) char [STACK_SIZE];
    if (stack == 0) {
        interrupt_enable();
        return -1;
    }
    uctx_running->uc_stack.ss_sp = stack;
    uctx_running->uc_stack.ss_size = STACK_SIZE;
    uctx_running->uc_stack.ss_flags = 0;
    uctx_running->uc_link = NULL;

    // Set first function for thread to call
    makecontext(uctx_running, (void (*)()) magic_func, 2, func, arg);
    clearMemory();
    /* std::cerr << "Libinit-setcontext" << uctx_running << std::endl; */
    if(setcontext(uctx_running) < 0) {
        interrupt_enable();
        return -1;
    }
    return 0;
}

/*
 * Starts new threads. Puts onto ReadyQ.
 */
int thread_create(thread_startfunc_t func, void *arg){
    interrupt_disable();
    /* print_size(); */
    // libinit must be first function to be called
    if (!libinit_called) {
        // libinit has not been called.
        interrupt_enable();
        return -1;
    }
    clearMemory();
    // Request new memory for thread.
    uctx_init = new (std::nothrow) ucontext_t;
    // New returned a null pointer -- not enough memory for thread.
    if (uctx_init == 0){
        interrupt_enable();
        return -1;
    }

    if (getcontext(uctx_init)) {
        interrupt_enable();
        return -1;
    }

    // Create stack and set parameters for new stack
    char *stack = new (std::nothrow) char [STACK_SIZE];
    if (stack == 0){
        interrupt_enable();
        // New returned a null pointer -- not enough memory
        return -1;
    }
    uctx_init->uc_stack.ss_sp = stack;
    uctx_init->uc_stack.ss_size = STACK_SIZE;
    uctx_init->uc_stack.ss_flags = 0;
    uctx_init->uc_link = NULL;

    makecontext(uctx_init, (void (*)()) magic_func, 2, func, arg);
    clearMemory();
    /* std::cerr << "RQ <-- " << uctx_init << std::endl; */
    ready_queue.push(uctx_init);
    interrupt_enable();
    return 0;
}

/*
 *  Yields Running thread to next thread.
 */
int thread_yield(){
    // We cannot call another yield within our function yield. Therefore
    // interrupts are disabled throughtout the entire function.
    interrupt_disable();
    clearMemory();
    // libinit must be first function to be called
    if (!libinit_called){
        interrupt_enable();
        return -1;
    } 

    // If other threads waiting, yield.
    if (!ready_queue.empty()) {
        // Running thread is yielded, and added to end of ReadyQ.
        /* std::cerr << "--YIELD--" << std::endl; */
        uctx_yielded = uctx_running;
        /* std::cerr << "RQ <-- " << uctx_yielded << std::endl; */
        ready_queue.push(uctx_yielded);

        // Next thread is taken off front of ReadyQ.
        uctx_running = ready_queue.front();
        ready_queue.pop();

        /* std::cerr << "[SWAP]: " << uctx_yielded << " --> " << uctx_running << std::endl; */
        if (swapcontext(uctx_yielded, uctx_running)) {
            interrupt_enable();
            return -1;
        }
        clearMemory();
    }
    interrupt_enable();
    return 0;
}

int thread_lock(unsigned int lock){
    //Disable for entire function hand off implementation.
    interrupt_disable();
    clearMemory();
    // libinit must be first function to be called
    if (!libinit_called){
        interrupt_enable();
        return -1;
    }

    if (lock_values[lock] == uctx_running) {
        // The same thread is trying to acquire the lock
        interrupt_enable();
        return -1;
    } else if (lock_values[lock] == 0) {
        lock_values[lock] = uctx_running;
    } else if ( ready_queue.front() == NULL || ready_queue.size() == 0 || ready_queue.empty() ) {
        // Deadlock. There are things on the lock queue but nothing to run next
        // on the readyQ
	clearMemory();
        std::cout << "Thread library exiting." << std::endl;
        /* print_size(); */
        interrupt_enable();
        exit(EXIT_SUCCESS);
    } else {
        // Must wait for turn for lock.
        uctx_yielded = uctx_running;
        /* std::cerr << "lockQ[" << lock << "] <-- " << uctx_yielded << std::endl; */
        lock_queue_map[lock].push(uctx_yielded);

        // The next thread to run is taken from the ReadyQ
        uctx_running = ready_queue.front();
        ready_queue.pop();
        /* std::cerr << "[SWAP]: " << uctx_yielded << " --> " << uctx_running << std::endl; */
        if (swapcontext(uctx_yielded, uctx_running)) {
            interrupt_enable();
            return -1;
        }
    }
    clearMemory();
    interrupt_enable();

    // disable interrupts
    // if (value == FREE) {
    //      value = BUSY
    // } else {
    //      add thread to lock_queue
    //      switch to next ready thread
    // }
    // enable interrupts
    return 0;
}

int thread_unlock(unsigned int lock){
    interrupt_disable();
    clearMemory();
    // libinit must be first function to be called
    if (!libinit_called){
        interrupt_enable();
        return -1;
    }

    if (lock_values[lock] != uctx_running) {
        // If the currently running thread does not hold the lock, it can't
        // release the lock.
        interrupt_enable();
        return -1;
    }

    lock_values[lock] = 0;
    if (!lock_queue_map[lock].empty()) {
        uctx_transfer = lock_queue_map[lock].front();
        lock_queue_map[lock].pop();

        /* std::cerr << "RQ <-- " << uctx_transfer << std::endl; */
        ready_queue.push(uctx_transfer);
        lock_values[lock] = uctx_transfer;
    }
    interrupt_enable();
    // disable interrupts
    // value = FREE
    // if (lock_queue not empty) {
    //      pop waiting thread off queue, put on ready queue
    //      value = BUSY
    //  }
    //  enable interrupts
    return 0;
}

int thread_wait(unsigned int lock, unsigned int cond){
    /********** Unlock code *********/
    interrupt_disable();
    clearMemory();
    // libinit must be first function to be called
    if (!libinit_called){
        interrupt_enable();
        return -1;
    }

    if (lock_values[lock] != uctx_running) {
        // If the currently running thread does not hold the lock, it can't
        // release the lock.
        interrupt_enable();
        return -1;
    }

    lock_values[lock] = 0;
    // Handoff the lock to next person in lock queue.
    if (!lock_queue_map[lock].empty()) {
        uctx_transfer = lock_queue_map[lock].front();
        lock_queue_map[lock].pop();

        /* std::cerr << "RQ <-- " << uctx_transfer << std::endl; */
        ready_queue.push(uctx_transfer);
        lock_values[lock] = uctx_transfer;
    }
    /********** End Unlock Code ********/

    clearMemory();
    uctx_yielded = uctx_running;

    /* std::cerr << "lockCVQ[" << lock << "][" << cond << "] <-- " << uctx_yielded << std::endl; */
    lockCV_queue[lock][cond].push(uctx_yielded);

    if (ready_queue.front() == NULL || ready_queue.size() == 0 || ready_queue.empty()) {
        clearMemory();
        std::cout << "Thread library exiting."<< std::endl;
        /* print_size(); */
        interrupt_enable();
        exit(EXIT_SUCCESS);
    } else {
        // The next thread to run is taken from the ReadyQ
        //std::cout << ready_queue.size()  << std::endl;
        uctx_running = ready_queue.front();
        ready_queue.pop();

        /* std::cerr << "[SWAP]: " << uctx_yielded << " --> " << uctx_running << std::endl; */
        swapcontext(uctx_yielded, uctx_running);
        clearMemory();
	//std::cout << lock << " we are near the end " << cond << std::endl;
    }

    /******** lock code ******/
    if (lock_values[lock] == uctx_running) {
        // The same thread is trying to acquire the lock
        interrupt_enable();
        return -1;
    } else if (lock_values[lock] == 0) {
        lock_values[lock] = uctx_running;
    } else if ( ready_queue.front() == NULL || ready_queue.size() == 0 || ready_queue.empty() ) {
        // Deadlock. There are things on the lock queue but nothing to run next
        // on the readyQ
	clearMemory();
        std::cout << "Thread library exiting." << std::endl;
        /* print_size(); */
        interrupt_enable();
        exit(EXIT_SUCCESS);
    } else {
        // Must wait for turn for lock.
        uctx_yielded = uctx_running;
        /* std::cerr << "lockQ[" << lock << "] <-- " << uctx_yielded << std::endl; */
        lock_queue_map[lock].push(uctx_yielded);

        // The next thread to run is taken from the ReadyQ
        uctx_running = ready_queue.front();
        ready_queue.pop();
        /* std::cerr << "[SWAP]: " << uctx_yielded << " --> " << uctx_running << std::endl; */

        if (swapcontext(uctx_yielded, uctx_running)) {
            interrupt_enable();
            return -1;
        }
    }
    clearMemory();

    /********** End lock code **********/
    interrupt_enable();
    return 0;
}

int thread_signal(unsigned int lock, unsigned int cond){
    interrupt_disable();
    clearMemory();
    // libinit must be first function to be called
    if (!libinit_called){
        interrupt_enable();
        return -1;

    } else if (!lockCV_queue[lock][cond].empty()) {
        uctx_transfer = lockCV_queue[lock][cond].front();
        lockCV_queue[lock][cond].pop();
        /* std::cerr << "[SIGNAL]: RQ <-- " << uctx_transfer << " <-- lockCVQ[" << lock << "][" << cond << "]" << std::endl; */
        ready_queue.push(uctx_transfer);
    } 

    interrupt_enable();

    // pop front of lock,CV wait_queue
    // put onto ready
    return 0;
}

int thread_broadcast(unsigned int lock, unsigned int cond){
    interrupt_disable();
    clearMemory();
    // libinit must be first function to be called
    if (!libinit_called){
        interrupt_enable();
        return -1;
    }

    while(!lockCV_queue[lock][cond].empty()) {
        uctx_transfer = lockCV_queue[lock][cond].front();
        lockCV_queue[lock][cond].pop();
        /* std::cerr << "[BROADCAST]: RQ <-- " << uctx_transfer << " <-- lockCVQ[" << lock << "][" << cond << "]" << std::endl; */
        ready_queue.push(uctx_transfer);
    }
    
    //There won't be anymore things waiting on this condition.
    lockCV_queue[lock].erase(cond);
    
    // pop all from of lock,CV wait_queue
    // put onto ready
    clearMemory();
    interrupt_enable();
    return 0;
}
