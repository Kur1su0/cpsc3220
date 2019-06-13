#include <stdio.h>
#include <string.h>

int verbose = 0;

typedef struct{
    unsigned char presence;   /* could be single bit */
    unsigned short pad;
    unsigned char pfn;        /* 8 bits */
  } PTE;

  typedef struct{
    unsigned char valid;      /* could be single bit */
    unsigned short vpn;       /* 16 bits */
    unsigned char pfn;        /* 8 bits */
  } TLBE;

  typedef struct{
    unsigned char valid;      /* could be single bit */
    unsigned char use_vector; /* 8 bits for pseudo-LRU replacement */
    unsigned short vpn;       /* 16 bits */
  } CME;


PTE _PTE;
TLBE _TLBE;
CME _CME;


int main(int argc, char* argv[]){
    if(argc == 2){
        verbose=strcmp(argv[1],"-v")==0?1:0;
    }
    printf("verbose:%d\n",verbose);
    //Read input.
    
    return 0;



}
