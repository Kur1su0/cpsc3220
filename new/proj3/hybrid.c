#include "hybrid.h"

//struct design.

char* bitmap_allocate();
char* list_allocate( int type );
int sanity_check(char *release_ptr);

void list_release (char* rover, int type);



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

    return NULL;
}

void move_head(int type){


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
