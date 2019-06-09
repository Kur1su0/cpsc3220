#include "hybrid.h"

//struct design.

char* bitmap_allocate();
char* list_allocate( int type );
int sanity_check(char *release_ptr);

void list_release (char* rover, int type);
void bitmap_free(char* release_ptr);



char *allocate( int size ){

    if( size <= 0 ){
      return NULL;
    }else if( size <= ( arena_block_size[0] - 8 ) ){
      return bitmap_allocate();
    }else if( size <= ( arena_block_size[1] - 8 ) ){
      return list_allocate( 1 );
    }else if( size <= ( arena_block_size[2] - 8 ) ){
      return list_allocate( 2 );
    }else{
      return NULL;
    }
}
char* bitmap_allocate(){
   if(arena_count[0]==0) return NULL; 
    char** block_ptr = NULL;

   
    long long unsigned int *val = (long long unsigned int*)arena_head[0];


    *val =  (long long unsigned int)(HEADER_SIGNATURE); //XXX: need to cast????
 
    arena_head[0] = (char*)val + ARENA_0_BLOCK_SIZE;
    if(arena_count[0]==0) arena_head[0]=NULL;
    
    //bitmap[];
    int where = (arena_head[0] - min_address) / ARENA_0_BLOCK_SIZE;
    printf("#%d block\n",where);
  
    arena_count[0]--;
    return (char*)val+ sizeof(char*);
 
}

char* list_allocate( int type ){
    if(arena_count[type]==0) return NULL;
    //char* val =arena_head[type];
    arena_count[type]--;
    char** block_ptr = NULL;

   
    long long unsigned int *val = (long long unsigned int*)arena_head[type];
    //long long unsigned int *val = (long long unsigned int*)arena_head[type];
    
        block_ptr = (char**)(arena_head[type]);
        block_ptr = (char**)(*block_ptr);


    *val =  (long long unsigned int)(HEADER_SIGNATURE +type); //XXX: need to cast????
 
    arena_head[type] = (char*)block_ptr;
    if(arena_count[type]==0) arena_head[type]=NULL;
    return (char*)val+ sizeof(char*);

}


void release( char *release_ptr ){
    if(sanity_check(release_ptr)==-1) return;
  
    char *ptr;
    long long unsigned int header, *header_ptr;
    ptr = release_ptr - 8;
    header_ptr = (long long unsigned int *) ptr;
    header = *header_ptr;
  
    if( header == (long long unsigned int)HEADER_SIGNATURE){
        printf("BITMAP\n");
        bitmap_free(release_ptr-8);
    }
   else if( header == (long long unsigned int)(HEADER_SIGNATURE+1)){
       printf("1\n");
       list_release(release_ptr-8, 1);
   }
   else if( header == (long long unsigned int)(HEADER_SIGNATURE+2)) {
       printf("2\n");
       list_release(release_ptr-8, 2);
   }


}

void bitmap_free(char* ptr){

    long long unsigned int *val = (long long unsigned int*)arena_head[0];
    *val = 0x00000000;
    arena_head[0] = ptr; 
    arena_count[0]++;

}


void list_release(char* ptr, int type){
    //ptr points to the 8-bit block.
    char** block_ptr = (char**)ptr;
    *block_ptr = arena_head[type];
    arena_head[type]=ptr;
    arena_count[type]++;

}

int sanity_check(char *release_ptr){
  char *ptr;
  long long unsigned int header, *header_ptr;
  
  if( (long long int) release_ptr & 0x7 ){
    printf( "pointer not aligned on 8B boundary in release() function\n" );
    printf( "  => no action taken\n" );
    return -1;
  }
  if( ( release_ptr < min_address ) || ( release_ptr > max_address )  ){
    printf( "pointer out of range in release() function\n" );
    printf( "  => no action taken\n" );
    return -1;
  }
  ptr = release_ptr - 8;
  header_ptr = (long long unsigned int *) ptr;
  header = *header_ptr;
  if( ( header & 0xfffffff0 ) != HEADER_SIGNATURE ){ //mask some to last 3
    printf( "header does not match in release() function\n" );
    printf( "  => no action taken\n" );
    return -1;
  }
  
  return 1;

}
