#include <stdlib.h>
#include <stdio.h>
#include "dmm.h"

int main(int argc, char *argv[])
{
    // Summary of Calls
    // A. ar0 = dmalloc(50)
    // ar1 = dmalloc(50)
    // ar2 = dmalloc(50)
    // ar3 = dmalloc(50)
	// ar4 = dmalloc(50)
	// ar5 = dmalloc(50)
	// ar6 = dmalloc(50)
	// ar7 = dmalloc(50)
	// ar8 = dmalloc(50)
	// ar9 = dmalloc(50)
	// ar10 = dmalloc(50)
	// B. ar11 = dmalloc(300) should fail*
    // C. dfree(ar0), dfree(ar1)
    // D. ar12 = dmalloc(90)
    // E. ar13 = dmalloc(200) *should fail*
    // F. dfree(ar2), dree(ar3)
    // G. ar14 = dmalloc(90)
    // H. ar15 = dmalloc(200) *should fail*
	// I. dfree(ar5), dfree(ar4)
	// J. ar16 = dmalloc(125)
	// K. ar17 = dmalloc(200) *should fail.

    char *ar0, *ar1, *ar2, *ar3, *ar4, *ar5, *ar6, *ar7, *ar8, *ar9, *ar10, *ar11, *ar12, *ar13, *ar14, *ar15, *ar16, *ar17;
    int i;

    // A.
    printf("ar 0: calling dmalloc(300)\n");
    ar0 = dmalloc(50);
    ar1 = dmalloc(50);
    ar2 = dmalloc(50);
    ar3 = dmalloc(50);
    ar4 = dmalloc(50);
    ar5 = dmalloc(50);
    ar6 = dmalloc(50);
    ar7 = dmalloc(50);
    ar8 = dmalloc(50);
    ar9 = dmalloc(50);
    ar10 = dmalloc(50);
    if (ar0*ar1*ar2*ar3*ar4*ar5*ar6*ar7*ar8*ar9*ar10 == NULL) {
        fprintf(stderr, "ar0..ar10 dmalloc has failed");
        exit(1);
    } 
    
    for (i = 0; i < 50; ++i) {
        ar0[i] = 'A';
        ar1[i] = 'B';
        ar2[i] = 'C';
        ar3[i] = 'D';
        ar4[i] = 'E';
        ar5[i] = 'F';
        ar6[i] = 'G';
        ar7[i] = 'H';
        ar8[i] = 'I';
        ar9[i] = 'J';
        ar10[i] = 'K';
    }

    for (i = 0; i < 10; ++i) {
        printf("%c", ar0[i]);
    }
    printf("...");

    for (i = 49; i < 50; ++i) {
        printf("%c", ar0[i]);
        printf("%c", ar1[i]);
        printf("%c", ar2[i]);
        printf("%c", ar3[i]);
        printf("%c", ar4[i]);
        printf("%c", ar5[i]);
        printf("%c", ar6[i]);
        printf("%c", ar7[i]);
        printf("%c", ar8[i]);
        printf("%c", ar9[i]);
        printf("%c", ar10[i]);
    }
    printf("\n");

    // B.
    printf("ar 11: calling dmalloc(300)\n");
    ar11 = dmalloc(300);
    if (ar11 != NULL) {
        fprintf(stderr, "ar3 dmalloc did not return NULL");
        exit(1);
    }

    printf("ar 11 dmalloc(300) successfully failed\n");

    // C.
    dfree(ar0);
    dfree(ar1);
    printf("Calling dfree(ar0) and dfree(ar1)\n");

    // D.
    printf("ar 4: calling dmalloc(100)\n");
    ar12 = dmalloc(123);
    if (ar12 == NULL) {
        fprintf(stderr, "ar4 dmalloc failed");
        exit(1);
    } 
    
    for (i = 0; i < 123; ++i) {
        ar12[i] = 'Z';
    }

    for (i = 0; i < 10; ++i) {
        printf("%c", ar12[i]);
    }
    printf("...");

    for (i = 122; i < 123; ++i) {
        printf("%c", ar12[i]);
    }
    printf("\n");
    
    // E.
    dfree(ar3);
    dfree(ar0);
    dfree(ar1);
    dfree(ar2);
    printf("Calling dfree(ar3)\n");
    printf("Calling dfree(ar0)\n");
    printf("Calling dfree(ar1)\n");
    printf("Calling dfree(ar2)\n");

    // I.
    printf("ar 6: calling dmalloc(250)\n");
    ar13 = dmalloc(150);
    if (ar13 == NULL) {
        fprintf(stderr, "ar6 dmalloc failed");
        exit(1);
    }

    for (i = 0; i < 150; ++i) {
        ar13[i] = 'Y';
    }

    for (i = 0; i < 10; ++i) {
        printf("%c", ar13[i]);
    }
    printf("...");

    for (i = 149; i < 150; ++i) {
        printf("%c", ar13[i]);
    }
    printf("\n");
}
