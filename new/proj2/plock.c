#include "plock.h"

//local helper func.
node_t* find_thread(node_t* lock, int id, int* flag);
void pretty_print(node_t* rover);


plock_t *plock_create(){
    //1. init plock
    plock_t* List = malloc(sizeof(plock_t));
    //2. set value to FREE.
    List->value = FREE;
    //3. init head*
    List->head = NULL;
    //4. init mutexlock
    pthread_mutex_init(&(List->mlock),NULL);
    
    return List;
}



/* plock_destory:
 * This function first garbage collects any nodes remaining on the waiting
 * list by destroying the condition variables using the appropriate pthread
 * library call and freeing the nodes. It then destroys the mutex mlock
 * using the appropriate pthread library call and finally frees the plock
 * data structure itself.
 */
void plock_destroy( plock_t *lock ){
    //return NULL;
}

/* plock_enter
 * This function checks the state variables of the plock data structure to
 * determine if the calling thread can proceed or instead must be added to
 * a waiting list. If the thread must be added, a node instance is created
 * and the condition variable within the node must be initialized using the
 * appropriate pthread library call. (The creation and addition could be
 * structured as a private helper function if you wish. Helper functions
 * should be contained in the plock.c source file.) The thread can then
 * wait on the condition variable in the node.
 *
 */

void plock_enter( plock_t *lock, int priority ){
    if(lock->head == NULL){
        //init thread.
        lock->head = malloc(sizeof(node_t));
        
        lock->head->priority = priority;
        //TODO: Do somting with wait CV.
        lock->head->next = NULL;
    }
    else{
        int flag = 0;
        node_t* tar = find_thread(lock->head, priority, &flag);
        if(flag==1){
            //found thread. tar = desired thread.
            //TODO: Something about CV.

        }
        else{
            //Add to tail.
            tar->next = malloc(sizeof(node_t));
            tar->next->priority = priority;
            //Set up: 1.CV.
            tar->next->next = NULL;

        }

    }
    //return NULL;
}






/* plock_exit
 * This function checks the state variables of the plock data structure to
 * determine what update action to take. For example, it may need to signal
 * a waiting thread using the appropriate pthread library call.
 */



void plock_exit( plock_t *lock ){

  // return NULL;
}







//Helper Functions:

// pretty_print
//@args: rover -> head of thread list.
void pretty_print(node_t* rover){ 
    while(rover != NULL){
        printf("%d --> ",rover->priority);
        rover = rover->next;
    }
    if(rover == NULL) printf("NULL\n");
}

// find_thread.
// @args rover  : cur node.
// @args id     : thread priority #.
// @args *flag  : 1--> found, 0--> not found.
node_t* find_thread(node_t* rover, int id, int* flag){
    
    pretty_print(rover); 
    while(1){
        if(rover->priority == id ){
            *flag = 1;
            printf("----Found---id:%i---\n",rover->priority);
            break;
        }
        if(rover->next == NULL){
            *flag=0;
            break;
        }
        rover = rover->next;
    }
    return rover;
} 




