#include "plock.h"
/********************
 * cpsc-3220 proj2  *
 * Zice Wei         *
 * C73880993        *
 ********************/
//Policy
/*  1. Always use a consistent structure.
    2. Always synchronize with locks and condition variables.
    3. Always acquire the lock at the beginning of the method and
         release it before the return.
    5. Always wait in a while loop.
    6. Never use thread_sleep() to make one thread wait for another;
                    use a condition variable instead.
*/



//local helper func.
node_t* find_thread(node_t* lock, int id, int* flag);
void pretty_print(node_t* rover);
void insert_thread(node_t* head, int priority);
//enQ : insert into Queue in order of high priority
//      return node's pos.
node_t* enQ(node_t** head, int priority);
//deQ : remove node from Q and return it.
node_t* deQ(plock_t* list);
node_t* clean_node(node_t* _node);

node_t* RUNNING = NULL;

plock_t *plock_create()
{
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
void clean_rec(node_t* rover){
    if(rover != NULL){
        clean_rec(rover->next);
	//clean_node(&rover);
        pthread_cond_destroy(&rover->waitCV);
        free(rover);
	rover = NULL;
    }
}

void plock_destroy( plock_t *lock )
{
    node_t* rover = NULL;
    while(lock->head!=NULL){
        rover = lock->head;
        lock->head = lock->head->next;
        
        pthread_cond_destroy(&rover->waitCV);
        free(rover);
	rover = NULL;
    }
    pthread_mutex_destroy(&lock->mlock);
    free(lock);
    lock = NULL;


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
//TODO: Check 5.5.2 implementation of Condition Variables ---
void plock_enter( plock_t *lock, int priority )
{
    
    pthread_mutex_lock(&lock->mlock);
    node_t* what = NULL;

    what = enQ(&(lock->head), priority);
    //lock->value=lock->head!=NULL?BUSY:FREE;
    while(1){
	
	if(lock->value==FREE && priority==lock->head->priority){
	    break; 
	} 
	pthread_cond_wait(&what->waitCV, &lock->mlock );
    }
    lock->value=BUSY;
    what = NULL;
    what = deQ(lock);
    what = clean_node(what);
     

    pthread_mutex_unlock(&(lock->mlock));
    



}






/* plock_exit
 * This function checks the state variables of the plock data structure to
 * determine what update action to take. For example, it may need to signal
 * a waiting thread using the appropriate pthread library call.
 */



void plock_exit( plock_t *lock )
{
    pthread_mutex_lock(&lock->mlock);
    //pretty_print(lock->head);
    if(lock->value != FREE && lock->head != NULL){

        pthread_cond_signal(&lock->head->waitCV);
    }
    lock->value = FREE;
    pthread_mutex_unlock(&lock->mlock);
    // return NULL;
}







//Helper Functions:

// pretty_print
//@args: rover -> head of thread list.
void pretty_print(node_t* rover)
{
    while(rover != NULL) {
        printf("%d --> ",rover->priority);
        rover = rover->next;
    }
    if(rover == NULL) printf("NULL\n");
}

// find_thread.
// @args rover  : cur node.
// @args id     : thread priority #.
// @args *flag  : 1--> found, 0--> not found.
node_t* find_thread(node_t* rover, int id, int* flag)
{

    pretty_print(rover);
    while(1) {
        if(rover->priority == id ) {
            *flag = 1;
            //printf("----Found---id:%i---\n",rover->priority);
            break;
        }
        if(rover->next == NULL) {
            *flag=0;
            break;
        }
        rover = rover->next;
    }
    return rover;
}



node_t* enQ(node_t** head, int priority){
    node_t* rover = *head;
    
    node_t* tar = malloc(sizeof(node_t));
    tar->priority = priority;
    pthread_cond_init(&tar->waitCV, NULL);
    tar->next = NULL;

    if(rover == NULL /*|| rover->next == NULL*/) { //may change.
        //if(rover ==NULL){
	    *head = tar;
            (*head)->next = NULL;
	/*}else{
            (*head)->next=tar;
            (*head)->next->next=NULL;

	}*/

    } else {
	//rover = rover->next;
        //Greatest, then 
        if(rover->priority < priority ){
            tar->next = (*head); //skip 1st node.
            (*head) = tar;
        }
        if(rover->priority >= priority){
            while(rover->next != NULL){
                if( priority > rover->next->priority && priority <= rover->priority) break;
                rover = rover->next;
            }
            
            tar->next = rover->next;
            rover->next = tar;
        }

    }
    return tar;
}
//deQ : remove node from Q and return it.
node_t* deQ(plock_t* list){
    node_t* rover = list->head;
    list->head  = list->head->next;
    return rover;

}

node_t* clean_node(node_t* what){
    pthread_cond_destroy(&what->waitCV);
    (what)->next = NULL;
    free(what);
    what = NULL;
    return what;


}
