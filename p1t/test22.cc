/***************************************************
 *  test22 - Signal/Broadcast called without lock  *
 ***************************************************/

#include <iostream>
#include "thread.h"

unsigned int x;
unsigned int y;

int func1(void* vp){
    long int id = (long int) vp;
    std::cout << "func1 id: " << id << std::endl;
    if ( thread_signal(x, y) ) {
        std::cout << "signal failed" << std::endl;
    } else {
        std::cout << "called signal without holding lock succesfully" << std::endl;
    }
    thread_yield();
    
    if ( thread_broadcast(x, y) ) {
        std::cout << "broadcast failed" << std::endl;
    } else {
        std::cout << "called broadcast without holding lock successfully" << std::endl;
    }
}

int master_func(void* vp){
    long int id = (long int) vp;
    std::cout << "master_func id: "<< id << std::endl;
    thread_lock(x);
    if (thread_create((thread_startfunc_t) func1, (void *) 100)) {
        std::cout << "thread create failed" << std::endl;
    }
    thread_yield();
    std::cout << "master func exited with lock(x)" << std::endl;
}

int main(int argc, char *argv[])
{
    x = 0;
    y = 0;
    thread_libinit((thread_startfunc_t) master_func, (void *) 200);
        
    return 0;
}
