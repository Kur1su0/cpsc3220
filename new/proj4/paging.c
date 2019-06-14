#include <stdio.h>
#include <stdlib.h>
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

  typedef struct{
      int PF;
      int TE;
      int UP;
  } info;

PTE pte;
TLBE tlbe;
CME cme;
info cfg;
char buf [100][7];
int len = 0;

void readData(){
    FILE* fptr = NULL;
    if( (fptr=fopen("paging.cfg","rb"))==NULL ){
         printf("bad file path\n");
         exit(1);
    }
    char junk[100];
    fscanf(fptr,"%s %d\n",junk,&cfg.PF);
    fscanf(fptr,"%s %d\n",junk,&cfg.TE);
    fscanf(fptr,"%s %d\n",junk,&cfg.UP);
    fclose(fptr);
}
void readBuf(){
    int i=0;
    while (!feof(stdin)){
       fscanf(stdin,"%s\n",buf[i]);
       i++;
    }
    len = i+1;
}
void printData(){
   printf("PF:%d\n",cfg.PF);
   printf("TE:%d\n",cfg.TE);
   printf("UP:%d\n",cfg.UP);
   int i=0;
   for(i=0;i<100;i++) {
        if(i+1==len) break;
        printf("%s\n",buf[i]);
    }

}

int main(int argc, char* argv[]){
    if(argc == 2){
        verbose=strcmp(argv[1],"-v")==0?1:0;
    }
    printf("verbose:%d\n",verbose);
    //Read input.
    readData();
    readBuf();
    printData();
    
    //stdin stream
    return 0;



}
