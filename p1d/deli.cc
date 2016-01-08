#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "thread.h"

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
unsigned int CV_can_add;
unsigned int CV_can_take;

void maker(void* vp);
void cashier(void* cashier_arg);
std::queue<int>* cashiers;

void deli(void* vp){
    //start_preemptions(bool async, bool sync, int random_seed)
    /*std::cout << "Starting async preemptions" << std::endl;
    start_preemptions(true, false, 10);*/

    /*std::cout << "Starting sync preemptions" << std::endl;
    start_preemptions(false, true, 15);*/

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
        // Acquire lock
        thread_lock(lock_board);
        // Check conditions
        while(!(board.size() >= max_board || board.size() >= num_cashiers)) {
            std::cerr << "[maker sleeps]" << std::endl;
            /*if(!thread_wait(lock_board, CV_can_take)){
 		std::cerr << "Maker is not waiting" << std::endl;
	    }*/
  	    thread_wait(lock_board, CV_can_take);
            std::cerr << "[maker awake]" << std::endl;
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

        // Acquire Lock
        thread_lock(lock_board);

        // Check the board conditions
        while(cashier_cant_add(cashier_id)) {
            //std::cerr << "[cashier waiting (" << cashier_id << ")]" << std::endl;
            thread_wait(lock_board, CV_can_add);
            //std::cerr << "[cashier awake (" << cashier_id << ")]" << std::endl; 
        }

        // If no more sandwiches left, exit.
        if (order_list.empty()) {
            /*std::cerr << "[cashier quits (" << cashier_id << ")]" << std::endl; */
            num_cashiers--;
            if (num_cashiers == 0) {
                return;
            }
            thread_unlock(lock_board);
            thread_broadcast(lock_board, CV_can_add);
            /*std::cerr << "numcashiers = " << num_cashiers << std::endl;*/ 
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
            //std::cerr << "Board size is: " << board.size() << std::endl;
            //std::cerr << "** signal to cashiers **" << std::endl;
            thread_broadcast(lock_board, CV_can_add);
        }

    }
}

int main(int argc, char *argv[])
{
    /******************* Argument parsing ******************/

    // Check for Correct Usage
    if (argc < 3) {
        std::cerr << "[Usage Error]: " << argv[0] << " max_orders *input_files" << std::endl;
        exit(EXIT_FAILURE);
    }

    /* num input files = num cashiers *
     * num makers is always 1 *
     * max_orders is maximum board */

    num_cashiers = argc - 2;
    int num_makers = 1;
    std::istringstream(argv[1]) >> max_board;

    // Create array of vectors to keep track of cashiers
    cashiers = new std::queue<int> [num_cashiers];

    // Reading orders from files
    // 1. Open file
    // 2. Get line
    // 3. Convert line(str) to int add to list
    // 4. Goto 2
    // 5. Put orders for cashier in array
    
    for (int i = 0; i < num_cashiers; ++i) {
        std::queue<int> sandwich_list;
        std::ifstream file(argv[i+2]);
        if (!file.is_open()) {
            std::cerr << "[File Error]: " << argv[i+2] << " could not be opened" << std::endl;char
            exit(EXIT_FAILURE);
        }
        std::string line;
        while(getline(file, line)) {
            int sandwich;
            std::istringstream(line) >> sandwich;
            sandwich_list.push(sandwich);
        }
        cashiers[i] = sandwich_list;
    }

    /*************** Threaded Program *********************/
    thread_libinit(deli, (void*) "Deli Program");

    return 0;
}
