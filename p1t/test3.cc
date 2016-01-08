/***************************************
 *  test3 - Basic Wait/Broadcast Test  *
 ***************************************/
#include <iostream>
#include <queue>
#include "thread.h"

unsigned int x=0, y=0;
unsigned int hasRoom, hasOne;
int sodas;
int num_drinker = 0;

void drinker(void* vp){
    num_drinker++;
    int id = num_drinker;
    std::cout << "We entered drinker " << id << std::endl;
    thread_lock(y);
    thread_lock(x);
    std::cout << "drinker lock acquired" << std::endl;

    while(sodas < 1) {
        std::cout << "drinker " << id << " waiting" << std::endl;
        thread_wait(x, hasOne);
        std::cout << "drinker " << id << " wait comes back" << std::endl;        
    }
    sodas--;
    thread_unlock(x);
    thread_unlock(y);
    num_drinker--;
    if(sodas == 0) {
        thread_broadcast(y, hasRoom);
        std::cout << "broadcast from drinker " << id << " on hasRoom" << std::endl;
    } else {
        thread_broadcast(x, hasOne);
    }
}

void stocker(void* vp) {
    do{
        std::cout << "We entered stocker and sodas "<< sodas << std::endl;
        thread_lock(x);
        thread_lock(y);
        std::cout << "stocker lock acquired" << std::endl;

        while(sodas > 0) {
            thread_wait(y, hasRoom);
            std::cout << "stocker wait" << std::endl;
        }

        sodas++;
        std::cout << "stocker lock released" << std::endl;
        thread_unlock(y);
        thread_unlock(x);
        thread_broadcast(x, hasOne);

    } while(num_drinker > 0);
}

void master_func(void* vp) { 
    std::cout << "Master func created" << std::endl;
    sodas   = 0;
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
// diff -iwy --suppress-common-lines a b
