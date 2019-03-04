#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mythreads.h"

#define MAXTHREAD 5
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
    ucontext_t main;
    int total_thread_num;
    int curr_id;
    struct thread_info *info;
}thread_head;

    struct thread_head* list = NULL;

//init thread
void threadInit(){
    list = (struct thread_head*)calloc(1,sizeof(thread_head));
    list->total_thread_num = MAXTHREAD;
    list->curr_id = 0;  //from 0 to MAXTHREAD - 1 .
    list->info = (struct thread_info*)calloc(list->total_thread_num,sizeof(struct thread_info));
    int i=0;
    list->info[0].context = list->main;
    //init all threads as empty.
    for (i=0;i<list->total_thread_num;i++){
        list->info[i].state = EMPTY;
    }
    list->info[0].state = RUN;
}


int threadCreate(thFuncPtr funcPtr, void *argPtr){
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
    //list->info[id].context.uc_link = &(list->main);

    list->info[id].Func = funcPtr;
    list->info[id].arg = argPtr;
    list->info[id].state = RUN;
    makecontext(&(list->info[id].context), (void(*)(void))funcPtr, 1, argPtr) ;
    list->curr_id = id;
    list->total_thread_num++;
    swapcontext(&(list->info[0].context),&(list->info[id].context));
    return id;
}

int next_id(int cur){
    int counter=0;
    int i = cur+1;
    int next = -1;
    while(counter < MAXTHREAD+1){
        i%=MAXTHREAD;
	//printf("id: %d status %d\n ",i,list->info[i].state);
	if(list->info[i].state != EMPTY && i != cur){
            next = i;
	        break;
	}
	counter++;
	i++;
    }
    //printf("cur: %d NEXT: %d\n",cur,next);
    return next==-1?cur:next;
}


void threadYield(){
    int id = list->curr_id;
    int NextId = -1;
    NextId = next_id(id);
    //printf("id :%d, next %d\n",id,NextId);
    //int counter = 0;
    //int i = 0;
	list->info[id].state = SUSPEND;
        
      //  if(NextId == id ){
            //printf("(from %d to main)\n",id);
      //      swapcontext(&(list->info[id].context) ,&(list->main));
      //  }
       // else{
            //printf("(from %d to %d)\n",id,NextId);
	        list->curr_id = NextId;
            swapcontext(&(list->info[id].context) ,&(list->info[NextId].context));
      //      }
    

}

void threadJoin(int thread_id, void **result){
   // if(list->info[thread_id].state == SUSPEND){
       //list->info[thread_id].state=RUN;
       //printf("join\n");
       list->curr_id = thread_id;
      swapcontext(&(list->info[0].context), &(list->info[thread_id].context));
       
 //   }


}


//exits the current thread -- closing the main thread, will terminate the program
/*
void threadExit(void *result){
    int id = list->curr_id;
    swapcontext(&(list->info[id].context) ,&(list->info[0].context));
}
*/
/*
extern void threadLock(int lockNum); 
extern void threadUnlock(int lockNum); 
extern void threadWait(int lockNum, int conditionNum); 
extern void threadSignal(int lockNum, int conditionNum); 

//this 
extern int interruptsAreDisabled;
*/
