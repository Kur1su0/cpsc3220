#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************
 *  Zice Wei                          *
 *  cpsc-3220                         *
 *  project 4                         *
 *  (See instruction in the makefile) *
 *                                    *
 **************************************/


void debug_display();
int process();
int process_frame(unsigned short vpn, unsigned char offset);
int update_tlb(unsigned short vpn, unsigned char offset);

int verbose = 0;
int access = 0,tlb_miss=0,page_fault=0,current_tlb=0;

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

void shift_vector(){
    if(access%cfg.UP==0){
        if(verbose==1) printf("shift use vectors\n");
        int i=0;
        for(i=0; i<cfg.PF; i++){
            cme[i].use_vector >>= 1;
        }
    }

}

int main(int argc, char* argv[]){
    if(argc == 2){
        verbose=strcmp(argv[1],"-v")==0?1:0;
    }
    //Read input.
    readData();
    //printData();
    if(verbose==1){
        printf("paging simulation\n");
        printf("  65536 virtual pages in the virtual address space\n");
        printf("  %d physical page frames\n",cfg.PF);
        printf("  %d TLB entries\n",cfg.TE);
        printf("  use vectors in core map are shifted every %d accesses\n\n",cfg.UP);

    }
    alloca_entry();

    while (!feof(stdin)){
       fscanf(stdin,"%x\n",&buf);
       //printf("0x%06x\t",buf);
       if(check_bit_len(buf)==1){
          debug_display();
          printf("\n");
       }else{
           access++;
           //print_bitmap();
           int where = 0;
           where = process();
           if(verbose==1 && where!=-1) printf("  tlb update of vpn 0x%04x with pfn 0x%02x\n",tlbe[where].vpn,tlbe[where].pfn);
           shift_vector();



       }
    }
    if(verbose==0){
    printf("statistics\n");
    printf("  accesses    = %d\n",access);
    printf("  tlb misses  = %d\n",tlb_miss);
    printf("  page faults = %d\n",page_fault);
    }else{
        debug_display();
    }
    
    free(pte);
    free(tlbe);
    free(cme);

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


int process(){
    //2A. Find TLB hits
    int where = 0; 
    if(verbose==1){
        printf("access %d:\n",access);
        printf("  virtual address is%14s0x%06x\n"," ",buf);

    }
    //split
    unsigned short vpn=0;
    unsigned char offset=0;
    set_bit("vpn",&vpn,&offset);
    set_bit("offset",&vpn,&offset);
   // printf("vpn:0x%04x\n",vpn);
   // printf("offset:0x%02x\n",offset);
    int i=0;
    for(i=0; i<cfg.TE; i++){
        if(vpn==tlbe[i].vpn && tlbe[i].valid==1){
            //TLB hit. Use pfn get phy addr.
            cme[tlbe[i].pfn].use_vector|=0x80;
            if(verbose==1)printf("  tlb hit, physical address is%6s0x%02x%02x\n"," ",tlbe[i].vpn,offset);
            return -1;
        }
    }
   //2B. TLB MISS.
       //3A. if presen bit is on.
   if(verbose==1)printf("  tlb miss\n");
   tlb_miss++;

   if(pte[vpn].presence==1){
   //page hit..Use pfn.
       if(verbose==1)printf("  page hit, physical address is%5s0x%02x%02x\n"," ",pte[vpn].pfn,offset);
       //Update TLB.
       where = update_tlb(vpn,pte[vpn].pfn);
       cme[pte[vpn].pfn].use_vector|=0x80;
       return where;
   }else{
   //Page fault.
       page_fault++;
       if(verbose==1)printf("  page fault\n");
       where = process_frame(vpn,offset);
       return where;

    }
    
    

    //pte = (PTE*)calloc(65536,sizeof(PTE));
    //tlbe = (TLBE*)calloc(cfg.TE,sizeof(TLBE));
    //cme = (CME*)calloc(cfg.PF,sizeof(CME));
    

}
int process_frame(unsigned short vpn, unsigned char offset){
//Page fault.
   //If there is free frame
   int where = 0;
   int i=0,free_frame_flag=0;
   for(i=0; i<cfg.PF;i++){
       if(cme[i].valid==0){
           free_frame_flag = 1;
           break;
       }
   }

   if(free_frame_flag==1){
       if(verbose==1) printf("  unused page frame allocated\n");
       cme[i].vpn=vpn;
       cme[i].use_vector|=0x80;
     
       cme[i].valid = 1;

       pte[vpn].pfn=(unsigned char)i;
       pte[vpn].presence=1;
      
       where = update_tlb(vpn,(unsigned char)i);
       
       if(verbose==1){
           printf("  physical address is%15s0x%02x%02x\n"," ",(unsigned char)i,offset);
       }
         
   }else{
       if(verbose==1) printf("  page replacement needed\n");
       unsigned int min_vector = 0xfff;
       int min_index = cfg.PF;
       for(i=0; i<cfg.PF; i++){
           if((cme[i].use_vector < min_vector) && (cme[i].use_vector!=0x00)){
               min_index = i;
               min_vector = cme[i].use_vector;
           }
       }
       if (min_vector==0xfff){
           min_index = 0;
           min_vector = cme[0].use_vector;

       }
       if(verbose==1) printf("  TLB invalidate of vpn 0x%x\n",cme[min_index].vpn); //TODO:wrong val?
       for(i=0;i<cfg.TE;i++){
           if(tlbe[i].vpn == cme[min_index].vpn){
               tlbe[i].valid = 0;
               tlbe[i].vpn = 0x0;
               tlbe[i].pfn = 0x0;
           }
       }
       //Clean old page table & core map
       pte[(int)cme[min_index].vpn].presence = 0;
       pte[(int)cme[min_index].vpn].pfn = 0x0;

       //Build new page & core map.
       cme[min_index].use_vector = 0x00;
       cme[min_index].use_vector|=0x80;
       cme[min_index].vpn = vpn;
       pte[vpn].pfn = (unsigned char)min_index;
       where = update_tlb(vpn,min_index);
       pte[vpn].presence=1;
       if(verbose==1) {
           printf("  replace frame %d\n",min_index);
           printf("  physical address is%15s0x%02x%02x\n"," ",min_index,offset);
       }
       
       
   }
   return where;

}
int update_tlb(unsigned short vpn, unsigned char pfn){
    int i=0;
    for(i=0; i<cfg.TE; i++){
        if(tlbe[i].valid==0){
            tlbe[i].pfn = pte[vpn].pfn;
            tlbe[i].vpn = vpn;
            tlbe[i].valid = 1;
            return i;
        }
    }
    i = current_tlb;
    tlbe[current_tlb].pfn = pte[vpn].pfn;
    tlbe[current_tlb].vpn = vpn;
    tlbe[current_tlb].valid = 1;
    current_tlb++;
    current_tlb%=cfg.TE;
    
    return i;

}


void debug_display(){
    if(verbose==1) printf("\n");
    printf("statistics\n");
    printf("  accesses    = %d\n",access);
    printf("  tlb misses  = %d\n",tlb_miss);
    printf("  page faults = %d\n",page_fault);
    printf("\ntlb\n");
    for(int i=0; i<cfg.TE; i++){
        printf("  valid = %d, vpn = 0x%04x, pfn = 0x%02x\n",
                   tlbe[i].valid,tlbe[i].vpn,tlbe[i].pfn);
    }
    printf("\ncore map table\n");
    for(int i=0; i<cfg.PF; i++){
        printf("  pfn = 0x%02x: valid = %d, use vector = 0x%02x, vpn = 0x%04x\n",
                   i, cme[i].valid, cme[i].use_vector, cme[i].vpn);
    }
    printf("\nfirst ten entries of page table\n");
    for(int i=0; i<10; i++){
        printf("  vpn = 0x%04x: presence = %d, pfn = 0x%02x\n",
                 i, pte[i].presence,pte[i].pfn);
    }

} 
