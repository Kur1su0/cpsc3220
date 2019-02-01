#define _GNU_SOURCE

void __attribute__ ((constructor)) init(void);
void __attribute__ ((destructor)) cleanup(void);

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void free(void *ptr);
void *malloc(size_t size);


//int (*original_rand)(void) = NULL;
void* (*original_malloc)(size_t size) = NULL;
void (*original_free)(void *ptr) = NULL;

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
void init(void)
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
	//perror("malloc\n");
	void *ptr = original_malloc(size);
    int file;
    file=open("log.txt", O_RDWR|O_CREAT|O_APPEND);
    char what[1000];
    sprintf(what,"malloc %p size:%zu \n",ptr,size);
    write(file,what,strlen(what));
    close(file);

	return ptr;
}
void free(void *ptr){
	original_free(ptr);
    int file;
    file=open("log.txt", O_RDWR|O_CREAT|O_APPEND);
    char what[1000];
    sprintf(what,"free %p\n",ptr);
    write(file,what,strlen(what));
}


//int rand (void)
//{
//  return original_rand() % 100;
//}
