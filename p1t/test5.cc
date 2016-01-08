/********************************
 *  test5 - class fifo example  *
 ********************************/

#include <iostream>
#include "thread.h"

unsigned int x = 0;
void func1(void* vp)
{
    long int id = (long int) vp;
    if (thread_lock(x)) {
        std::cout << "lock failed" << std::endl;
    } else{
        std::cout << id << " locks" << std::endl;
    }
    std::cout << id << " yields" << std::endl;
    thread_yield();
    std::cout << id << " yield return" << std::endl;
    if (thread_unlock(x)) {
        std::cout << "unlock failed" << std::endl;
    } else {
        std::cout << id << " unlocked" << std::endl;
    }
}

void func2(void* vp)
{
    long int id = (long int) vp;
    std::cout << id << " locks" << std::endl;
    if (thread_lock(x)) {
        std::cout << "lock failed" << std::endl;
    } else{
        std::cout << id << " locks" << std::endl;
    }
    if (thread_unlock(x)) {
        std::cout << "unlock failed" << std::endl;
    } else{
        std::cout << id << " unlocks" << std::endl;
    }
    std::cout << id << " finished" << std::endl;
}

void func3(void* vp)
{
    long int id = (long int) vp;
    std::cout << id << " yields" << std::endl;
    thread_yield();
    std::cout << id << " locks" << std::endl;
    if (thread_lock(x)) {
        std::cout << "lock failed" << std::endl;
    } else{
        std::cout << id << " locks" << std::endl;
    }
    std::cout << id << " unlock" << std::endl;
    thread_unlock(x);
    std::cout << id << " finished" << std::endl;


}
void master_func(void* vp)
{ 
    thread_create(func1, (void*) 1);
    thread_create(func2, (void*) 2);
    thread_create(func3, (void*) 3);
}

int main(int argc, char *argv[])
{
    thread_libinit(master_func, (void*) "master");
    return 0;
}
