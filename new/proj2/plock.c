#include "plock.h"

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
