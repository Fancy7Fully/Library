#include <stdlib.h>
#include <stdio.h>
#include "dmm.h"


// Segmentation fault occurs in this sequence of calls
// A. dmalloc(300)
// B. dmalloc(300)
// C. dmalloc(300)
//
// After first dmalloc call, we write 300 bytes of information, which runs into
// the header of other blocks. Segmentation fault occurs on line 69 of dmm.c
// inside dmalloc function.
//
int main(int argc, char *argv[])
{

    int i;
    char *ar0, *ar1, *ar2;
    ar0 = dmalloc(300);
    for (i = 0; i < 300; ++i) {
        ar0[i] = 'A';
    }

    ar1 = dmalloc(300);
    for (i = 0; i < 300; ++i) {
        ar1[i] = 'B';
    }
    ar2 = dmalloc(300); 
    
    for (i = 0; i < 300; ++i) {
        ar2[i] = 'C';
    }

    for (i = 0; i < 300; ++i) {
        printf("%c", ar2[i]);
    }
    printf("\n");

    return 0;
}
