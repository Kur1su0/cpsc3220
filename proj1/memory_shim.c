#define _GNU_SOURCE

void __attribute__ ((constructor)) malloc_init(void);
void __attribute__ ((constructor)) free_init(void);
//void __attribute__ ((constructor)) rand_init(void);

void __attribute__ ((destructor)) cleanup(void);

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void free(void *ptr);
void *malloc(size_t size);

//int (*original_rand)(void) = NULL;
void (*original_malloc)(void) = NULL;
void (*original_free)(void) = NULL;

// Called when the library is unloaded
void cleanup(void)
{
  //you would use this function to close any files that were still open
  //or do any other clean up.
  printf("cleaning up.\n");
}

/* malloc_init: redirection lib malloc to this function
 * 
 */
void malloc_init(void)
{
  if (original_malloc == NULL)
  {
  	original_malloc = dlsym(RTLD_NEXT, "malloc");
  }
  if (original_free == NULL)
  {
  	original_free = dlsym(RTLD_NEXT, "free");
  }
  printf("initializing malloc and free library.\n");


}

void *malloc(size_t size){
    printf("malloc size%zu\n",size);
    

        	
}
void free(void *ptr){
    printf("addr %d",&ptr);
}


//int rand (void)
//{
//  return original_rand() % 100;
//}
