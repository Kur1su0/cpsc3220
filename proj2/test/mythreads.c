#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mythreads.h"

#define MAXTHREAD 5000
#define EMPTY     0
#define RUN       1
#define SUSPEND   2
#define FINISH    3
#define JOIN      4 

#define TRUE 1
#define FALSE 0

//typedef void *(*thFuncPtr) (void *); 
//int total_id = 0;
typedef struct thread_info{
    char stack[STACK_SIZE];
    int id;
    int state;
    ucontext_t context;
    thFuncPtr Func;
    void *arg;
    void* result;
}thread_info;

typedef struct thread_head{
    ucontext_t main;
    int total_thread_num;
    int curr_id;
    struct thread_info *info;
}thread_head;

    struct thread_head* list = NULL;
    int interruptsAreDisabled;
//init thread
void threadInit(){
    interruptsAreDisabled = 1;

    list = (struct thread_head*)calloc(1,sizeof(thread_head));
    list->total_thread_num = MAXTHREAD;
    list->curr_id = 0;  //from 0 to MAXTHREAD - 1. 0 indicates the main thread.
    list->info = (struct thread_info*)calloc(list->total_thread_num,sizeof(struct thread_info));
    int i=0;
    list->info[0].context = list->main;
    //init all threads as empty.
    for (i=0;i<list->total_thread_num;i++){
        list->info[i].state = EMPTY;
    }
    list->info[0].state = RUN;
    
    interruptsAreDisabled = 0;
}

//helper function for passing function to thead, if thread done, state changed to FINISH.
void thread_helper(int id){
    list->info[id].result = list->info[id].Func(list->info[id].arg);
    
    list->info[id].state = FINISH;
    swapcontext(&(list->info[id].context),&(list->info[0].context));

   
}



int threadCreate(thFuncPtr funcPtr, void *argPtr){
    interruptsAreDisabled = 1;
    
    int id = 1;
    //assign a id for new threads.
    for(id=1; id < MAXTHREAD; id++){
        if(list->info[id].state == EMPTY) break;

    }
    
    if(id >= MAXTHREAD) return -1; //out of thread pool.
    list->info[id].id = id;
    getcontext(&(list->info[id].context));
    list->info[id].context.uc_stack.ss_sp = list->info[id].stack;
    list->info[id].context.uc_stack.ss_size = STACK_SIZE;
    list->info[id].context.uc_stack.ss_flags = 0;

    list->info[id].Func = funcPtr;
    list->info[id].arg = argPtr;
    list->info[id].state = RUN;
    
    makecontext(&(list->info[id].context),(void(*)(void))thread_helper, 1, id);
    //int current_id  = list->curr_id;//dave old id.
    list->curr_id = id;// assign new id
    list->total_thread_num++;
    //XXX: becareful with the XXX current_id XXX.
    swapcontext(&(list->info[0].context),&(list->info[id].context));
    interruptsAreDisabled = 0;
    return id;
}

int next_id(int cur){
    int counter=0;
    int i = cur+1;
    int next = -1;
    while(counter < MAXTHREAD+1){
        i%=MAXTHREAD;
	//printf("id: %d status %d\n ",i,list->info[i].state);
	if(list->info[i].state != EMPTY && list->info[i].state!= FINISH  && i != cur){
            next = i;
	        break;
	}
	counter++;
	i++;
    }
    return next;
}


void threadYield(){
    interruptsAreDisabled = 1;

    int id = list->curr_id;
    int NextId = -1;
    NextId = next_id(id);
    if(NextId == -1 || list->info[id].state==FINISH) return;
    if(list->info[list->curr_id].state!=FINISH){
        list->curr_id = NextId; 
        swapcontext(&(list->info[id].context) ,&(list->info[NextId].context));
    }
    
    interruptsAreDisabled = 0;
}

void threadJoin(int thread_id, void **result){ 
    interruptsAreDisabled = 1;
    if (list->info[thread_id].state == FINISH) return;
    while(list->info[thread_id].state != FINISH){
        threadYield();

        *result = (void*)list->info[thread_id].result;
    }
    list->curr_id = 0;   
    
    interruptsAreDisabled = 0;
}


//exits the current thread -- closing the main thread, will terminate the program

void threadExit(void *result){
    interruptsAreDisabled = 1;
    int id = list->curr_id;


    if(id ==0 ) exit(0); //calling this funct in main thread causes exit of the whole program.
    else{
        
        list->info[id].state = FINISH;
        if(result!=NULL) list->info[id].result = result;
        list->curr_id = 0;
        swapcontext(&(list->info[id].context),&(list->info[0].context));
       
    }

    interruptsAreDisabled = 0;
}


/*
extern void threadLock(int lockNum); 
extern void threadUnlock(int lockNum); 
extern void threadWait(int lockNum, int conditionNum); 
extern void threadSignal(int lockNum, int conditionNum); 

//this 
extern int interruptsAreDisabled;
*/
