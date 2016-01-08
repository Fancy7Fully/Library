/*****************************
 *  test1 - basic lock test  *
 *****************************/

#include <iostream>
#include "thread.h"

unsigned int x;

void func1(void* vp){
    std::cout << "func1 Called" << std::endl;

    int z = thread_lock(x);
    std::cout << "Lock x acquired" << std::endl;
    if (thread_yield()) {
        std::cout << "thread yield failed" << std::endl;
    }
    std::cout << "Hello from 1" << std::endl;
    thread_unlock(x);
    std::cout << "Lock x released" << std::endl;
    std::cout << "Func1 finished" << std::endl;
}

void func2(void* vp)
{
    std::cout << "func2 Called" << std::endl;
    thread_lock(x);
    std::cout << "Lock x acquired" << std::endl;
    std::cout << "Hello from 2" << std::endl;
    thread_unlock(x);
    std::cout << "Lock x released" << std::endl;
    if (thread_yield()) {
        std::cout << "thread yield failed" << std::endl;
    }
    std::cout << "Func2 finished" << std::endl;
}

void master_func(void* vp) {
    std::cout << "Master func created" << std::endl;
    thread_create(func1, (void*) "First Function");
    thread_create(func2, (void*) "Second Function");
}

int main(int argc, char *argv[])
{
    thread_libinit(master_func, (void*) "First function");
    return 0;
}
