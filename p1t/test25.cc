/****************************************************
 *  test8 - make many threads to run out of memory  *
 ****************************************************/

#include <iostream>
#include <stdlib.h>
#include "thread.h"

void func1(void* vp)
{
    int a = (long int) vp;
    std::cout << "func1: " << a << std::endl;
}
int master_func(void* vp)
{
    long int i;
    for (i = 0; i < 100; ++i) {
        if (thread_create(func1, (void*) i)) {
            std::cout << "thread_create failed" << std::endl;
            return -1;
            
        }
    }
}

int main(int argc, char *argv[])
{
    thread_libinit((thread_startfunc_t) master_func, (void*) "First function");
    return 0;
}
