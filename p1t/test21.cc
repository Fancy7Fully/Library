/**********************************
 *  test21 - wait/broadcast test  *
 **********************************/

#include <iostream>
#include <queue>
#include "thread.h"

unsigned int x=0, y=0;
unsigned int hasRoom, hasOne;
int sodas;
int num_drinker = 0;

void drinker(void* vp){
    num_drinker++;
    int id = (long int) vp;
    std::cout << "We entered drinker " << id << std::endl;
    thread_lock(y);
    thread_lock(x);
    std::cout << "drinker " << id << " lock acquired" << std::endl;

    while(sodas < 1) {
        std::cout << "drinker " << id << " waiting" << std::endl;
        thread_wait(x, hasOne);
        std::cout << "drinker " << id << " wait comes back" << std::endl;        
    }
    sodas--;
    std::cout << "drinker " << id << " Drank. Now there are sodas = " << sodas << std::endl;
    thread_unlock(x);
    thread_unlock(y);
    std::cout << "locks released" << std::endl;
    num_drinker--;
    std::cout << "num drinkers is now = " << num_drinker << std::endl;
    if(sodas == 0) {
        thread_broadcast(y, hasRoom);
        std::cout << "broadcast from drinker " << id << " on hasRoom" << std::endl;
    } else {
        thread_broadcast(x, hasOne);
        std::cout << "broadcast from drinker " << id << " on hasOne" << std::endl;
    }
}

void stocker(void* vp) {
    do{
        std::cout << "We entered stocker and sodas = "<< sodas << std::endl;
        thread_lock(x);
        thread_lock(y);
        std::cout << "stocker lock acquired" << std::endl;

        while(sodas > 0) {
            std::cout << "stocker waits" << std::endl;
            thread_wait(y, hasRoom);
            std::cout << "stocker wakes up" << std::endl;
        }

        sodas++;
        std::cout << "[" << sodas << "] Sodas" << std::endl;
        thread_unlock(y);
        thread_unlock(x);
        std::cout << "stocker lock released" << std::endl;
        thread_signal(x, hasOne);
        std::cout << "stocker broadcast on hasOne" << std::endl;

    } while(num_drinker > 0);
}

void master_func(void* vp) { 
    std::cout << "Master func created" << std::endl;
    sodas   = 0;
    hasRoom = 500;
    hasOne  = 1000;
    thread_create(drinker, (void*) 1);
    thread_create(drinker, (void*) 2);
    thread_create(drinker, (void*) 3);
    thread_create(drinker, (void*) 4);
    thread_create(drinker, (void*) 5);
    thread_create(drinker, (void*) 6);
    thread_create(stocker, (void*) "Main Function");
}

int main(int argc, char *argv[])
{
    thread_libinit(master_func, (void*) "First function");
    return 0;
}
