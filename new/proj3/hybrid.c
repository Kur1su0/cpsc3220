/*****************
 * proj3         *
 *****************/

#include "hybrid.h"
#define GROUP 32

//Bitset op citied from:
// https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit

char* bitmap_allocate();
char* list_allocate( int type );
int sanity_check(char *release_ptr);

void list_release (char* rover, int type);
void bitmap_free(char* release_ptr);
void find_min_bitmap();


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

void print_bitmap(){
    int i = 0, j=0, bit = 0;
    for(i=0; i<NUM_BITMAP_WORDS;i++){
        for(j=1; j < GROUP+1; j++){
            bit = (bitmap[i] >> (GROUP-j)) & 1UL;
            printf("%i",bit);
        }
        printf(" ");

    }
    printf("\n");

}
//Find lowest-0-bit and point where to the arena_head[0].
void find_min_bitmap(){
    //1. Loc empty slot
    int i = 0, j=0, bit = 0,pos=0,flag=0;
    //bitmap[0] = 0x80000000;
    for(i=0; i<NUM_BITMAP_WORDS;i++){
        for(j=1; j < GROUP+1; j++){
            bit = (bitmap[i] >> (GROUP-j)) & 1UL;
            if(bit==0) {
                pos = j+(i*GROUP);
                flag = 1;
                break;
               
                }
        }
        if(flag == 1) break;
    }
 
   //2.move header to empty slot.
   char* min = level_0_arena;
   arena_head[0] = min + (pos-1)*ARENA_0_BLOCK_SIZE; //Ask prof.
   //arena_head[0] = min_address + (pos-1)*ARENA_0_BLOCK_SIZE;
   //3.Bitset op citied from:
   // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
   bitmap[i] = (bitmap[i] & ~(1ULL << (GROUP-j))) | (1 << (GROUP-j));
   //bitmap[i] ^= (-1 ^ bitmap[i]) & (1UL << (GROUP-j));
  print_bitmap();  
   
}

char* bitmap_allocate(){
   if(arena_count[0]==0) return NULL;
    find_min_bitmap();
    long long unsigned int *val = (long long unsigned int*)arena_head[0];
    *val =  (long long unsigned int)(HEADER_SIGNATURE);
    
    arena_count[0]--;
    if(arena_count[0]==0) arena_head[0]=NULL;
    print_bitmap();
    
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
        bitmap_free(release_ptr-8);
    }
   else if( header == (long long unsigned int)(HEADER_SIGNATURE+1)){
       list_release(release_ptr-8, 1);
   }
   else if( header == (long long unsigned int)(HEADER_SIGNATURE+2)) {
       list_release(release_ptr-8, 2);
   }


}

void bitmap_free(char* ptr){

    //get # of block
    char* min = level_0_arena;
    int where = (ptr - min)/ARENA_0_BLOCK_SIZE;
    //int where = (ptr - min_address)/ARENA_0_BLOCK_SIZE;
    
    
    int bitmap_group = 0, entry_num = 0;
    bitmap_group = where / GROUP;
    entry_num = where % GROUP + 1;
    bitmap[bitmap_group] = (bitmap[bitmap_group] & ~(1ULL << (GROUP-entry_num))) | (0 << (GROUP-entry_num));
    arena_head[0] = ptr; 
    long long unsigned int *val = (long long unsigned int*)arena_head[0];
    *val = 0x00000000;
    arena_count[0]++;
    //print_bitmap();
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
