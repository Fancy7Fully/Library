/***************************************
 *  test2 - Basic Wait/Broadcast Test  *
 ***************************************/

#include <iostream>
#include <queue>
#include "thread.h"


unsigned int lock;
unsigned int hasRoom, hasOne;
int sodas;

void drinker(void* vp){
    std::cout << "We entered drinker" << std::endl;
    thread_lock(lock);
    std::cout << "drinker lock acquired" << std::endl;

    while(sodas < 1) {
        std::cout << "drinker waiting" << std::endl;
        thread_wait(lock, hasOne);
        std::cout << "drinker wait comes back" << std::endl;        
    }
    sodas--;
    
    thread_unlock(lock);
    if(sodas == 0) {
        thread_broadcast(lock, hasRoom);
        std::cout << "broadcast from drinker on hasRoom" << std::endl;
    } else {
        thread_broadcast(lock, hasOne);
    }
}

void stocker(void* vp) {
    std::cout << "We entered stocker" << std::endl;
    thread_lock(lock);
    std::cout << "stocker lock acquired" << std::endl;

    while(sodas > 0) {
        thread_wait(lock, hasRoom);
        std::cout << "stocker wait" << std::endl;
    }
    sodas++;

    thread_unlock(lock);
    thread_broadcast(lock, hasOne);
}

void master_func(void* vp) {
    std::cout << "Master func created" << std::endl;
    sodas=0;
    hasRoom = 500;
    hasOne  = 1000;
    thread_create(drinker, (void*) "First Function");
    thread_create(drinker, (void*) "Second Function");
    thread_create(drinker, (void*) "Third Function");
    thread_create(drinker, (void*) "Fourth Function");
    thread_create(drinker, (void*) "Fifth Function");
    thread_create(drinker, (void*) "Sixth Function");
    thread_create(stocker, (void*) "Main Function");
}

int main(int argc, char *argv[])
{
    thread_libinit(master_func, (void*) "First function");
    return 0;
}
