#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 10000


void debug_display();
void process();


int verbose = 0;
int access = 0,tlb_miss=0,page_fault=0;

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

PTE* pte;
TLBE* tlbe;
CME* cme;
info cfg;
unsigned int buf;
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
void print_bitmap(){
        int bit=0,j=0;
        for(j=1; j < 24+1; j++){
            bit = (buf >> (24-j)) & 1UL;
            printf("%i",bit);
            printf("%s",j%4==0?" ":"");
        }
    printf("\n");
}

void printData(){
   printf("PF:%d\n",cfg.PF);
   printf("TE:%d\n",cfg.TE);
   printf("UP:%d\n",cfg.UP);

}
void alloca_entry(){
    pte = (PTE*)calloc(65536,sizeof(PTE));
    tlbe = (TLBE*)calloc(cfg.TE,sizeof(TLBE));
    cme = (CME*)calloc(cfg.PF,sizeof(CME));
}
int check_bit_len(unsigned int num){
    int count = 0; 
    while (num){ 
        num >>= 1; 
        count++; 
    } 
    return count>24?1:0;
}


int main(int argc, char* argv[]){
    if(argc == 2){
        verbose=strcmp(argv[1],"-v")==0?1:0;
    }
    printf("verbose:%d\n",verbose);
    //Read input.
    readData();
    printData();
    if(verbose==1){
        printf("paging simulation\n");
        printf("65536 virtual pages in the virtual address space\n");
        printf("%d physical page frames\n",cfg.PF);
        printf("%d TLB entries\n",cfg.TE);
        printf("use vectors in core map are shifted every %d accesses\n",cfg.UP);

    }
    alloca_entry();

    while (!feof(stdin)){
       fscanf(stdin,"%x\n",&buf);
       //printf("0x%06x\t",buf);
       if(check_bit_len(buf)==1){
           printf("trigger print_func\n");
          debug_display();
       }else{
           access++;
           //print_bitmap();
           process(access);




       }
    }
    //stdin stream
    return 0;
}
void set_bit(char type[4],unsigned short* vpn, unsigned char* pfn){
    int i=0,bit=0,j=1;
    if(strcmp("vpn",type)==0){
        for(i=0+1;i<16+1;i++){
           bit = (buf>>(24-i))&1UL;
           //printf("%d",bit);
           *vpn = (*vpn & ~(1ULL << (16-j))) | (bit << (16-j));
           j++;
        }
            //printf("\n");
    }
    else{
        for(i=16+1;i<24+1;i++){
           bit = (buf>>(24-i))&1UL;
           //printf("%d",bit);
           *pfn = (*pfn & ~(1ULL << (8-j))) | (bit << (8-j));
           j++;
        }
        
    }
}

void process(){
    //2A. Find TLB hits
    if(verbose==1){
        printf("access %d:\n",access);
        printf("  virtual address is%14s0x%06x:\n"," ",buf);

    }
    //split
    unsigned short vpn=0;
    unsigned char pfn=0;
    set_bit("vpn",&vpn,&pfn);
    set_bit("pfn",&vpn,&pfn);
    //int i=0;
    //for(i=0;i<cfg.TE;i++){
    //
    //}

}

void debug_display(){


}
