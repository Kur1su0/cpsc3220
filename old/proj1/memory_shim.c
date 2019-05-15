/* program 1.
 *
 * Shared lib for deteccting mem leak using lib call.
 */

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
#define True 1
#define False 0


//int (*original_rand)(void) = NULL;
void* (*original_malloc)(size_t size) = NULL;
void (*original_free)(void *ptr) = NULL;

//Struct
typedef struct data{
    void* addr;
    size_t size;
    int isFreed;
    void* next;
}data;

data* HEAD  = NULL;
data* TAIL = NULL;
int term = 0;

// Destruct link list.
void destruct_rec(data* rover){
    if (rover!=NULL){
        destruct_rec(rover->next);
        free(rover);
        rover = NULL;

    }
}

// Called when the library is unloaded
void cleanup(void)
{
  //you would use this function to close any files that were still open
  //or do any other clean up.
  int cur=0;
  data* rover = HEAD;
  int total_num = 0;
  size_t total_size = (size_t)0;
  //print leak summary.
  while(cur < term && term !=0 ){
      if(rover->isFreed == False){
          fprintf(stderr,"LEAK\t%zu\n",rover->size);
          total_size = total_size + rover->size;
          total_num++;
      }
      rover = rover->next;
      cur++;
  }
  rover = NULL;
  fprintf(stderr,"TOTAL\t%d\t%zu\n",total_num,total_size);
  destruct_rec(HEAD);

  
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
}

/* redirect malloc to there and add malloc info to a link list*/
void *malloc(size_t size){
	//perror("malloc\n");
	void *ptr = original_malloc(size);
    if (term == 0) {
        HEAD = original_malloc(sizeof(data));
        TAIL = HEAD;
    }else{
        TAIL->next = original_malloc(sizeof(data));
        TAIL = TAIL->next;
    }
    //init list
    TAIL->addr = ptr;
    TAIL->size = size;
    TAIL->isFreed= False;
    TAIL->next = NULL;
    term++;
	return ptr;
}

/* redirect free to there and add free info to a link list*/
void free(void *ptr){
    int cur=0;int freed_count=0;
    data* free_check_ptr = HEAD;
    //check for free flag.
    while(term!=0 && cur<=term && ptr!=NULL){
        if(free_check_ptr->addr == ptr &&
           free_check_ptr->isFreed == False){
            free_check_ptr->isFreed = True;
            freed_count++;
            break;
        }
        free_check_ptr = free_check_ptr->next;
        cur++;
    }
	if (ptr!=NULL) original_free(ptr);
}

