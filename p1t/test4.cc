/*****************************************************
 *  test4 - Thread acquires/release same lock twice  *
 *****************************************************/

#include <iostream>
#include <stdlib.h>
#include "thread.h"

// Locks
static unsigned int x;

void func1(void* vp)
{
    if (thread_lock(x)) {
        std::cout << "UNSUCCESFUL" << std::endl;
    } else {
        std::cout << "First lock(x) Acquired: " << std::endl;
    }
    
    if (thread_lock(x)) {
       std::cout << "second thread_lock(x) successfully fails" << std::endl; 
    }

    std::cout << "body" << std::endl;

    if (thread_unlock(x)) {
        std::cout << "UNSUCCESFUL" << std::endl;
    } else {
        std::cout << "First lock(x) Released: " << std::endl;
    }

    if (thread_unlock(x)) {
       std::cout << "second thread_unlock(x) successfully fails" << std::endl; 
    }
}

int main(int argc, char *argv[])
{
    x = 1;
    thread_libinit(func1, (void*) "First function");
    return 0;
}
