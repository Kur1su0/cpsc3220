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
  //destruct_rec(HEAD);

  
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
  //printf("initializing malloc and free library.\n");
  //int file=open("log.txt",O_WRONLY|O_CREAT |O_TRUNC,0666);
  //close(file);


}

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

    TAIL->addr = ptr;
    TAIL->size = size;
    TAIL->isFreed= False;
    TAIL->next = NULL;
    term++;
    /*
    int file;
    file=open("log.txt", O_RDWR|O_CREAT|O_APPEND| O_SYNC,0666);
    char what[1000];
    sprintf(what,"%p %zu\n",ptr,size);
    write(file,what,strlen(what));
    close(file);
    */
    //fprintf(stderr,"malloc term %d\n",term);
	return ptr;
}
void free(void *ptr){
    /*
    int file;
    file=open("log.txt", O_RDWR|O_CREAT|O_APPEND| O_SYNC,0666);
    char what[1000];
    sprintf(what,"free %p\n",ptr);
    write(file,what,strlen(what));
    close(file);
    */
    //fprintf(stderr,"free term%d\n",term);
    int cur=0;int freed_count=0;
    data* free_check_ptr = HEAD;
    while(term!=0 && cur<=term && ptr!=NULL){
        if(free_check_ptr->addr == ptr &&
           free_check_ptr->isFreed == False){
            free_check_ptr->isFreed = True;
            freed_count++;
            //fprintf(stderr,"found\n");
            break;
        }
        free_check_ptr = free_check_ptr->next;
        cur++;
    }
	if (ptr!=NULL) original_free(ptr);
}

