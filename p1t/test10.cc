/***************************************
 *  test10 - Deli modified for test  *
 ***************************************/

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <list>
#include <map>
#include <sstream>
#include <cstdlib>
#include "thread.h"

#define ARRAYSIZE(array) (sizeof((array))/sizeof((array[0])))

typedef struct _order_t {
    int cashier_id;
    int sandwich_id;
} order_t;

typedef struct _cashier_arg_t {
    int cashier_id;
    std::queue<int> order_list;
} cashier_arg_t;

std::vector<order_t> board;
int max_board;
int num_cashiers;
int num_makers;

// Locks
unsigned int lock_board;

// Condition Variables;
unsigned int CV_can_add =10;
unsigned int CV_can_take =15;

void maker(void* vp);
void cashier(void* cashier_arg);
std::queue<int>* cashiers;

void deli(void* vp){
//std::cout << "We are in deli\n";
    //start_preemptions(bool async, bool sync, int random_seed)
    /*std::cout << "Starting async preemptions" << std::endl;
    start_preemptions(true, false, 10);*/
    thread_create(maker, (void*) "maker");
    for (int i = 0; i < num_cashiers; ++i) {
        cashier_arg_t* cashier_arg = new cashier_arg_t;
        cashier_arg->cashier_id = i;
        cashier_arg->order_list = cashiers[i];
        thread_create(cashier,(void *) cashier_arg);
    }
}
void maker(void* vp){
    int last_sandwich_made = -1;
    // until sandwiches are finished
    do {
	//std::cout << "We are in maker\n";
        // Acquire lock
        thread_lock(lock_board);
        // Check conditions
        while(!(board.size() >= max_board || board.size() >= num_cashiers)) {
           // std::cerr << "[maker sleeps]" << std::endl;
            /*if(!thread_wait(lock_board, CV_can_take)){
 		std::cerr << "Maker is not waiting" << std::endl;
	    }*/
  	    thread_wait(lock_board, CV_can_take);
           // std::cerr << "[maker awake]" << std::endl;
        }
        while(board.size() >= max_board || board.size() >= num_cashiers) {
            int min_difference = 1000;
            int min_diff_index = 0;
            int diff;
            for (int i = 0; i < board.size(); ++i) {
                diff = abs(board[i].sandwich_id - last_sandwich_made);
                if (min_difference > diff) {
                    min_diff_index = i;
                    min_difference = diff;
                }
            }
            last_sandwich_made = board[min_diff_index].sandwich_id;
            std::cout << "READY: cashier " << board[min_diff_index].cashier_id << " sandwich " << board[min_diff_index].sandwich_id << std::endl;
            board.erase(board.begin() + min_diff_index);
        }
	
        // Release lock
        thread_unlock(lock_board);

        // Broadcast 
        thread_broadcast(lock_board, CV_can_add);
    } while(num_cashiers > 0);

    //initialization to -1
    // lock board
    // while (board not full){
    //      wait(board full)
    //  }
    //  while board full {
    //      use algorithm, make sandwich
    //      remove from board
    //      cout
    //  }
    //  broadcast;
    //  } unlock;
}

//assume the board lock is had here

// 3 conditions to check:
//      1. >= max board
//      2. >= num cashiers 
//      3. cashier_id on board already

bool cashier_cant_add(int cashier_id){
    if (board.size() >= max_board) {
        return true;

    } else if (board.size() >= num_cashiers) {
        return true;
    }

    for (int i = 0; i < board.size(); ++i) {
        if (board[i].cashier_id == cashier_id) {
            return true;
        }
    }
    return false;
}


void cashier(void* cashier_arg){    
    int cashier_id = ((cashier_arg_t*) cashier_arg)->cashier_id;
    std::queue<int> order_list = ((cashier_arg_t*)cashier_arg)->order_list;
    while(true) {
	//std::cout << "We are in cashier1\n";
        // Acquire Lock
        thread_lock(lock_board);

        // Check the board conditions
        while(cashier_cant_add(cashier_id)) {
           // std::cerr << "[cashier waiting (" << cashier_id << ")]" << std::endl;
            thread_wait(lock_board, CV_can_add);
            //std::cerr << "[cashier awake (" << cashier_id << ")]" << std::endl; 
        }
	//std::cout << "We are in cashier2\n";
        // If no more sandwiches left, exit.
        if (order_list.empty()) {      
            num_cashiers--;
            if (num_cashiers == 0) {
		//std::cerr << "[cashier quits (" << cashier_id << ")]" << std::endl; 
                return;
            }
            thread_unlock(lock_board);
            thread_broadcast(lock_board, CV_can_add);
           // std::cerr << "numcashiers = " << num_cashiers << std::endl;
            return;
        }

        // Create Structure to add to board
        order_t order;
        order.cashier_id = cashier_id;
        order.sandwich_id = order_list.front();
        order_list.pop();

        // Add to board
        board.push_back(order);
        std::cout << "POSTED: cashier " << order.cashier_id << " sandwich " << order.sandwich_id << std::endl;

        // Release Lock
        thread_unlock(lock_board);

        // Signal the maker if the board is full or its equal to num_cashiers
        if (board.size() >= max_board || board.size() >= num_cashiers) {
            //board.size() != 0 removed from the if statement  
	    //std::cerr << "Board size is: " << board.size() << std::endl;   
            //std::cerr << "** signal to maker **" << std::endl;
            thread_broadcast(lock_board, CV_can_take);
        } else {
           // std::cerr << "Board size is: " << board.size() << std::endl;
            //std::cerr << "** signal to cashiers **" << std::endl;
            thread_broadcast(lock_board, CV_can_add);
        }
    }
}

int main(int argc, char *argv[])
{
    /******************* Argument parsing ******************/
    std::map<int, std::list<int> > matrix;
    num_makers = 1;
    num_cashiers = 10;
    max_board = 4;
    cashiers    = new std::queue<int> [num_cashiers];
    int zero[]  = {250, 490, 510};
    int one[]   = {150, 480, 520};
    int two[]   = {50, 47, 530};
    int three[] = {25, 470, 540};
    int four[]  = {20, 460, 590};
    int five[]  = {950, 440, 580};
    int six[]   = {850, 920, 570};
    int seven[] = {750, 875, 560};
    int eight[] = {650, 45, 551};
    int nine[]  = {350, 35, 55};
    for(int i =0; i < 10; i++) matrix[i].push_back(0);
 
	matrix[0].insert(matrix[0].begin(), zero, zero+ARRAYSIZE(zero)+1);
 	matrix[1].insert(matrix[1].begin(), one, one+ARRAYSIZE(one)+1);
	matrix[2].insert(matrix[2].begin(), two, two+ARRAYSIZE(two)+1);
	matrix[3].insert(matrix[3].begin(), three, three+ARRAYSIZE(three)+1);
	matrix[4].insert(matrix[4].begin(), four, four+ARRAYSIZE(four)+1);
	matrix[5].insert(matrix[5].begin(), five, five+ARRAYSIZE(five)+1);
	matrix[6].insert(matrix[6].begin(), six, six+ARRAYSIZE(six)+1);
	matrix[7].insert(matrix[7].begin(), seven, seven+ARRAYSIZE(seven)+1);
	matrix[8].insert(matrix[8].begin(), eight, eight+ARRAYSIZE(eight)+1);
	matrix[9].insert(matrix[9].begin(), nine, nine+ARRAYSIZE(nine)+1);

    for(int i = 0; i < num_cashiers; i++){
	std::queue<int> sandwich_list;
		for(int j = 0; j < matrix[i].size(); j++){ 
			sandwich_list.push(matrix[i].front());
			matrix[i].pop_front();	
		}  
	cashiers[i] = sandwich_list;
    }
   
    /*************** Threaded Program *********************/
    thread_libinit(deli, (void*) "Deli Program");

    return 0;
}
