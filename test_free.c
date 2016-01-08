#include <stdlib.h>
#include <stdio.h>
#include "dmm.h"

int main(int argc, char *argv[])
{
    // Summary of Calls
    // A. ar0 = dmalloc(300)
    // B. ar1 = dmalloc(300)
    // C. ar2 = dmalloc(300)
    // D. ar3 = dmalloc(300) *should fail*
    // E. dfree(ar0)
    // F. ar4 = dmalloc(100)
    // G. ar5 = dmalloc(200) *should fail*
    // H. dfree(ar2)
    // I. ar6 = dmalloc(250)
    // J. ar7 = dmalloc(200) *should fail*

    char *ar0, *ar1, *ar2, *ar3, *ar4, *ar5, *ar6, *ar7;
    int i;

    // A.
    printf("ar 0: calling dmalloc(300)\n");
    ar0 = dmalloc(300);
    if (ar0 == NULL) {
        fprintf(stderr, "ar0 dmalloc has failed");
        exit(1);
    } 
    
    for (i = 0; i < 300; ++i) {
        ar0[i] = 'A';
    }

    for (i = 0; i < 10; ++i) {
        printf("%c", ar0[i]);
    }
    printf("...");

    for (i = 290; i < 300; ++i) {
        printf("%c", ar0[i]);
    }
    printf("\n");

    // B.
    printf("ar 1: calling dmalloc(300)\n");
    ar1 = dmalloc(300);
    if (ar1 == NULL) {
        fprintf(stderr, "ar1 dmalloc has failed");
        exit(1);
    } 
    
    for (i = 0; i < 300; ++i) {
        ar1[i] = 'B';
    }

    for (i = 0; i < 10; ++i) {
        printf("%c", ar1[i]);
    }
    printf("...");

    for (i = 290; i < 300; ++i) {
        printf("%c", ar1[i]);
    }
    printf("\n");

    // C.
    printf("ar 2: calling dmalloc(300)\n");
    ar2 = dmalloc(300);
    if (ar2 == NULL) {
        fprintf(stderr, "ar2 dmalloc has failed");
        exit(1);
    } 
    
    for (i = 0; i < 300; ++i) {
        ar2[i] = 'C';
    }

    for (i = 0; i < 10; ++i) {
        printf("%c", ar2[i]);
    }
    printf("...");

    for (i = 290; i < 300; ++i) {
        printf("%c", ar2[i]);
    }
    printf("\n");

    // D.
    printf("ar 3: calling dmalloc(300)\n");
    ar3 = dmalloc(300);
    if (ar3 != NULL) {
        fprintf(stderr, "ar3 dmalloc did not return NULL");
        exit(1);
    } 

    printf("ar 3 dmalloc(300) successfully failed\n");

    // E.
    dfree(ar0);
    printf("Calling dfree(ar0)\n");
    
    // F.
    printf("ar 4: calling dmalloc(100)\n");
    ar4 = dmalloc(100);
    if (ar4 == NULL) {
        fprintf(stderr, "ar4 dmalloc failed");
        exit(1);
    } 
    
    for (i = 0; i < 100; ++i) {
        ar4[i] = 'F';
    }

    for (i = 0; i < 10; ++i) {
        printf("%c", ar4[i]);
    }
    printf("...");

    for (i = 90; i < 100; ++i) {
        printf("%c", ar4[i]);
    }
    printf("\n");
    
    // G.
    printf("ar 5: calling dmalloc(200)\n");
    ar5 = dmalloc(200);
    if (ar5 != NULL) {
        fprintf(stderr, "ar5 dmalloc did not return NULL");
        exit(1);
    } 
    printf("ar 5 dmalloc(200) successfully failed\n");
    printf("\n");

    // H.
    dfree(ar2);
    printf("Calling dfree(ar2)\n");

    // I.
    printf("ar 6: calling dmalloc(250)\n");
    ar6 = dmalloc(250);
    if (ar6 == NULL) {
        fprintf(stderr, "ar6 dmalloc failed");
        exit(1);
    } 
    
    for (i = 0; i < 250; ++i) {
        ar6[i] = 'I';
    }

    for (i = 0; i < 10; ++i) {
        printf("%c", ar6[i]);
    }
    printf("...");

    for (i = 240; i < 250; ++i) {
        printf("%c", ar6[i]);
    }
    printf("\n");

    // J.
    printf("ar 7: calling dmalloc(200)\n");
    ar7 = dmalloc(200);
    if (ar7 != NULL) {
        fprintf(stderr, "ar7 dmalloc did not return NULL");
        exit(1);
    } 
    printf("ar7 dmalloc(200) successfully failed\n");
    printf("\n\n");

    printf("All free tests passed!\n");

    return 0;
}
