#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mythreads.h"

#define MAXTHREAD 1000
#define RUN       1
#define SUSPEND   2
#define EMPTY     0

//typedef void *(*thFuncPtr) (void *); 
//int total_id = 0;
typedef struct thread_info{
    char stack[STACK_SIZE];
    int id;
    int state;
    ucontext_t context;
    thFuncPtr Func;
    void *arg;
}thread_info;

typedef struct thread_head{
    ucontext_t parent;
    int total_thread_num;
    int curr_id;
    struct thread_info *info;
}thread_head;

    struct thread_head* list = NULL;

//init thread
extern void threadInit(){
    list = (struct thread_head*)calloc(1,sizeof(thread_head));
    list->total_thread_num = MAXTHREAD;
    list->curr_id = 0;  //from 0 to MAXTHREAD - 1 .
    list->info = (struct thread_info*)calloc(list->total_thread_num,sizeof(struct thread_info));
    int i=0;
   
    //init all threads as empty.
    for (i=0;i<list->total_thread_num;i++){
        list->info[i].state = EMPTY;
    }

}


extern int threadCreate(thFuncPtr funcPtr, void *argPtr){
    int args = *((int*)argPtr);
    int id = 0;
    //assign a id for new threads.
    for(id=0; id < MAXTHREAD; id++){
        if(list->info[id].state == EMPTY) break;

    }
    if(id >= MAXTHREAD) return -1; //out of thread pool.
    list->info[id].id = id;
    getcontext(&(list->info[id].context));
    list->info[id].context.uc_stack.ss_sp = list->info[id].stack;
    list->info[id].context.uc_stack.ss_size = STACK_SIZE;
    list->info[id].context.uc_stack.ss_flags = 0;
    list->info[id].context.uc_link = &(list->parent);
    
    list->info[id].Func = funcPtr;
    list->info[id].arg = argPtr;
    list->info[id].state = RUN;
    makecontext(&(list->info[id].context), (void(*)(void))funcPtr, 1, argPtr) ;
    swapcontext(&(list->parent),&(list->info[id].context));
    


    
    
    return id;
}
/*
extern void threadYield(); 
extern void threadJoin(int thread_id, void **result);

//exits the current thread -- closing the main thread, will terminate the program
extern void threadExit(void *result); 

extern void threadLock(int lockNum); 
extern void threadUnlock(int lockNum); 
extern void threadWait(int lockNum, int conditionNum); 
extern void threadSignal(int lockNum, int conditionNum); 

//this 
extern int interruptsAreDisabled;
*/
