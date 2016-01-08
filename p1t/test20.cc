/*******************************************
 *  test20 - Libinit must be called first  *
 *******************************************/

#include <iostream>
#include "thread.h"


int func1(void* vp){
    long int id = (long int) vp;
    std::cout << "func1 id: "<< id << std::endl;
}

int main(int argc, char *argv[])
{
    start_preemptions(false, true, 10);
    unsigned int x = 0;
    unsigned int y = 0;
    if ( thread_create((thread_startfunc_t) func1, (void *) 100) ) {
        std::cout << "thread_create correctly failed" << std::endl;
    }
    if ( thread_yield() ) {
        std::cout << "thread_yield correctly failed" << std::endl;
    }
    if ( thread_lock(x) ) {
        std::cout << "thread_lock correctly failed" << std::endl;
    }
    if ( thread_unlock(x) ) {
        std::cout << "thread_unlock correctly failed" << std::endl;
    }
    if ( thread_wait(x,y) ) {
        std::cout << "thread_wait correctly failed" << std::endl;
    }
    if ( thread_signal(x,y) ) {
        std::cout << "thread_signal correctly failed" << std::endl;
    }
    if ( thread_broadcast(x,y) ) {
        std::cout << "thread_broadcast correctly failed" << std::endl;
    }

    thread_libinit((thread_startfunc_t) func1, (void *) 200);
        
    return 0;
}
