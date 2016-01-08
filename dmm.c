#include <stdio.h>  // needed for size_t
#include <unistd.h> // needed for sbrk
#include <assert.h> // needed for asserts
#include "dmm.h"

void coalesce();

/* You can improve the below metadata structure using the concepts from Bryant
 * and OHallaron book (chapter 9).
 */

typedef struct metadata {
    /* size_t is the return type of the sizeof operator. Since the size of an
     * object depends on the architecture and its implementation, size_t is used
     * to represent the maximum size of any object in the particular
     * implementation. size contains the size of the data object or the number of
     * free bytes
     */
    size_t size;
    struct metadata* next;
    struct metadata* prev;
} metadata_t;

/* freelist maintains all the blocks which are not in use; freelist is kept
 * sorted to improve coalescing efficiency 
 */

static metadata_t* freelist = NULL;

void* dmalloc(size_t numbytes) {
    /* initialize through sbrk call first time */
    if(freelist == NULL) {
        if(!dmalloc_init())
            return NULL;
    }

    assert(numbytes > 0);

    /* print_freelist(); */

    /* your code here */
    metadata_t *allocated, *previous, *traverse, *next;
    /* printf("FREELIST: %p\n",freelist); */
    traverse = freelist;

    // Loop through freelist to find big enough block.
    while(traverse != NULL ){

        // Compare if enough room to allocate requested number of bytes.
        if(traverse->size >= (ALIGN(numbytes)+METADATA_T_ALIGNED)){

            // Initialize structure with size and address of VERY beginning of block
            size_t temp = traverse->size;
            allocated = traverse;
            allocated->size = ALIGN(numbytes);


            //DEBUG
            /* printf("allocated and traverse: %p\n", allocated); */

            // Update freelist
            previous = traverse->prev;
            next = traverse->next;
            /* printf("traverse->prev: %p\n", previous); */
            /* printf("traverse->next: %p\n", next); */
            /* printf("temp original: %zu\n", temp); */

            // Move the current node to beginning of free block
            void* freeblock_beginning = (void *) traverse;
            freeblock_beginning = freeblock_beginning + METADATA_T_ALIGNED + allocated->size;
            traverse = (metadata_t *) freeblock_beginning;
        
            /* printf("new traverse: %p\n", freeblock_beginning); */

            /* printf("temp after should be the same: %zu\n", temp); */
            traverse->size = temp-allocated->size - METADATA_T_ALIGNED;
            /* printf("That new size though: %zu\n", traverse->size); */
            traverse->next = next;


            // Check if block was the first of freelist, if it is,
            // make "freeblock_beginning" the head of freelist.
            if(previous != NULL){
                previous->next = traverse;
                traverse->prev = previous;
            } else {
                freelist = freeblock_beginning;
                if (freelist->next != NULL) {
                    freelist->next->prev = freelist;
                }
                freelist->prev = NULL;
            }
            return (allocated + (METADATA_T_ALIGNED)/sizeof(metadata_t));
        }
        traverse = traverse->next;
    };
    return NULL;
}

void dfree(void* ptr) {
	/* your code here */
	int done = 0;

	/*Starting at the beginning of the freelist*/
	metadata_t* current = freelist;
	
	/*Setting the correct location for the pointer 
	to be freed while taking the header into account*/

	metadata_t* newspaceptr = ptr - METADATA_T_ALIGNED;

        /*Traversing freelist*/
        while(current != NULL || !done){
            /* printf("[IN DFREE]: current = %p, newspaceptr = %p\n", current, newspaceptr); */

            /*Checking if newspaceptr is the head of freelist*/
            if((current->prev == NULL) && (current > newspaceptr)){
                /* printf("[IN DFREE]: current->prev = %p, newspaceptr = %p\n", current->prev, newspaceptr); */
                current->prev = newspaceptr;
                newspaceptr->next = freelist;
                newspaceptr->prev = NULL;
                freelist = newspaceptr;
                done = 1;

                /*Checking if newspaceptr is in the middle of freelist*/
            } else if((current->prev < newspaceptr) && (current > newspaceptr)){
                newspaceptr->prev = current->prev;
                newspaceptr->next = current;
                current->prev->next = newspaceptr;
                current->prev = newspaceptr;
                done = 1;

                /*Checking if newspaceptr is the tail of freelist*/
            } else if((current < newspaceptr) && (current->next == NULL)){
                newspaceptr->prev = current;
                newspaceptr->next = NULL;
                current->next = newspaceptr;
                done = 1;
            }
            /*Freelist traversal*/
            current = current->next;
        }
        coalesce();
}

void coalesce(){
    metadata_t *traverse = freelist;
    int traversed = 0;
    while(traverse != NULL){
        /* printf("[IN COALESCE] - traverse->size: %zu\n", traverse->size); */
        /* printf("[IN COALESCE] - (void *) traverse + traverse->size + METADATA_T_ALIGNED: %p\n", (void *) traverse + traverse->size + METADATA_T_ALIGNED); */
        /* printf("[IN COALESCE] - traverse->next: %p\n", traverse->next); */
        if(((void *) traverse + traverse->size + METADATA_T_ALIGNED) == traverse->next){
            traverse->size = traverse->size + traverse->next->size + METADATA_T_ALIGNED;
            traverse->next = traverse->next->next;
            if (traverse->next != NULL) {
                traverse->next->prev = traverse;
            } 
            traversed = 1;
        }
        if(traversed){
            traversed = 0;
            traverse = freelist;
        }
        else{
            traverse = traverse->next;
        }
    }
    /* print_freelist(); */
}

bool dmalloc_init() {

    /* Two choices:
     * 1. Append prologue and epilogue blocks to the start and the
     * end of the freelist
     *
     * 2. Initialize freelist pointers to NULL
     *
     * Note: We provide the code for 2. Using 1 will help you to tackle the
     * corner cases succinctly.
     */

    size_t max_bytes = ALIGN(MAX_HEAP_SIZE);
    /* returns heap_region, which is initialized to freelist */
    freelist = (metadata_t*) sbrk(max_bytes);
    /* Q: Why casting is used? i.e., why (void*)-1? */
    if (freelist == (void *)-1)
        return false;
    freelist->next = NULL;
    freelist->prev = NULL;
    freelist->size = max_bytes-METADATA_T_ALIGNED;
    return true;
}

/* for debugging; can be turned off through -NDEBUG flag*/
void print_freelist() {
    metadata_t *freelist_head = freelist;
    while(freelist_head != NULL) {
        DEBUG("\tFreelist Size:%zd, Head:%p, Prev:%p, Next:%p\t",
                freelist_head->size,
                freelist_head,
                freelist_head->prev,
                freelist_head->next);
        freelist_head = freelist_head->next;
    }
    DEBUG("\n");
}
