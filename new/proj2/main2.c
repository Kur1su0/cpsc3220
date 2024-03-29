/* priority lock (plock) test driver
 *
 * compile with:             gcc -Wall plock.c main.c -pthread
 * run memcheck tests with:  valgrind --tool=memcheck ./a.out
 * run helgrind tests with:  valgrind --tool=helgrind ./a.out
 */



#include "plock.h"



plock_t *priority_lock;      /* global lock shared among worker threads */


/*
void *worker( void *args )
{

    int thread_id = *( (int *) args );
    int priority  = *( ( (int *) args ) + 1 );
    plock_enter( priority_lock, priority );
    printf( "thread %d starts with priority %d\n", thread_id, priority );
    sleep( 2 );
    printf( "thread %d ends\n", thread_id );
    plock_exit( priority_lock );
    pthread_exit( NULL );

}


*/
int main( int argc, char **argv )
{

    pthread_t threads[20];
    int i;
    int rc;
    int args[20][2] =            /* pairs of thread id and priority */
    {   {  0,  0 }, {  1,  5 }, {  2,  3 }, {  3,  4 }, {  4,  1 },
        {  5, 10 }, {  6, 15 }, {  7, 13 }, {  8, 14 }, {  9, 11 },
        { 10,  0 }, { 11,  5 }, { 12,  3 }, { 13,  4 }, { 14,  1 },
        { 15, 10 }, { 16, 15 }, { 17, 13 }, { 18, 14 }, { 19, 11 }
    };

    priority_lock = plock_create();



//    for( i = 0; i < 20 ; i++ ) {
 //       rc = pthread_create( &threads[i], NULL, &worker, (void *)( args[i] ) );
 //       if( rc ) {
 //           printf( "** could not create thread %d\n", i );
 //           exit( -1 );
 //       }
 //       if( ( i & 3 ) == 0 ) sleep( 1 );
 //         plock_enter(priority_lock,i);
 //  }

          enQ(&priority_lock->head, 4);
          pretty_print(priority_lock->head);
	  
	  enQ(&priority_lock->head, 5);
          pretty_print(priority_lock->head);
          
	  
          node_t* _node= deQ(&priority_lock->head);
	  printf("P:%d\n",_node->priority);
          pretty_print(priority_lock->head);
	  free(_node);
          
	  printf("destotyed\n");
	  plock_destroy( priority_lock );
          pretty_print(priority_lock->head);
	  //  }
 //  }
 //  }
exit(1);

    for( i = 0; i < 20; i++ ) {
        rc = pthread_join( threads[i], NULL );
        if( rc ) {
            printf( "** could not join thread %d\n", i );
            exit( -1 );
        }
    }

    plock_destroy( priority_lock );
    return 0;

}
