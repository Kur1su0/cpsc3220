#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define FREE 0
#define BUSY 1

typedef struct node_def {
    int priority;           /* priority of the waiting thread          */
    pthread_cond_t waitCV;  /* thread waits on this condition variable */
    struct node_def *next;  /* node is part of a singly-linked list    */
} node_t;

typedef struct {
    int value;              /* FREE or BUSY                            */
    pthread_mutex_t mlock;  /* mutex protecting the plock structure    */
    node_t *head;           /* linked list of waiting threads          */
} plock_t;


//Functions
plock_t *plock_create();
void plock_destroy( plock_t *lock );
void plock_enter( plock_t *lock, int priority );
void plock_exit( plock_t *lock );

/*  plock_creat:
 *  This function calls malloc() to allocate space for an instance of the
 *  plock_t data type, initializes the plock value to FREE, initializes
 *  the mutex mlock using the appropriate pthread library call,
 *  appropriately initializes the waiting list head pointer, and returns
 *  a pointer to the instance.
 *
 */
plock_t *plock_create(){

    return NULL;
}


/* plock_destory:
 * This function first garbage collects any nodes remaining on the waiting
 * list by destroying the condition variables using the appropriate pthread
 * library call and freeing the nodes. It then destroys the mutex mlock
 * using the appropriate pthread library call and finally frees the plock
 * data structure itself.
 */
void plock_destroy( plock_t *lock ){

    return NULL;
}




/*plock_exit 
 *This function checks the state variables of the plock data structure to
 *determine if the calling thread can proceed or instead must be added to
 *a waiting list. If the thread must be added, a node instance is created
 *and the condition variable within the node must be initialized using the
 *appropriate pthread library call. (The creation and addition could be
 *structured as a private helper function if you wish. Helper functions
 *should be contained in the plock.c source file.) The thread can then
 *wait on the condition variable in the node.
 *
 */

void plock_enter( plock_t *lock, int priority ){

    return NULL;
}






/*plock_exit
 *This function checks the state variables of the plock data structure to
 *determine what update action to take. For example, it may need to signal
 *a waiting thread using the appropriate pthread library call.
 */



void plock_exit( plock_t *lock ){

   return NULL;
}
