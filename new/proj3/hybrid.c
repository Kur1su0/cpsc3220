#include "hybrid.h"

//struct design.
typedef struct{
    char* what;

}node;

char* bitmap_allocate();
char* list_allocate( int type );

/*
The allocate() function finds a free block that matches the requested
size in number of bytes. (A byte is equal to the 8-bit char data type
in C, so char* pointers are used.)

The function determines the arena with the appropriate blocksize that
will accommodate the requested size plus 8 extra bytes for the header.
The bitmap or free list of an arena is based on the starting address
of free blocks, but the allocate() function inserts a header in an
allocated block and returns a pointer that is just beyond the header.
Thus, if the free block that is selected starts at address 0x1000,
the function inserts the header at address 0x1000 and sends back the
address 0x1008 to the caller.

The bitmap for arena 0 is indexed in a big-endian manner in each
[32-bit] word in an array of bitmap words (i.e., from left-to-right).
So 0x8000000 in word 0 of the bitmap represents the first block in
arena 0, and 0x0001000 in word 2 of the bitmap represents block 79
(i.e., 2*32 + 15) in arena 0. A 0 in the bitmap indicates that the
block is free, and a 1 indicates that the block is allocated. Free
blocks in arena 0 are totally empty (i.e., they do not contain
pointers or status bits).

For arena 0, the free block with the lowest address, if one is
present, should be the one allocated.

The other two arenas each use the [initial bytes] of free blocks to
hold the next-block pointers to implement singly-linked free lists.
Thus free blocks in arenas 1 and 2 are not completely empty.

For arenas 1 and 2, the free block at the head of the free list, if
one is present, should be the one allocated.

Each allocated block has an 8-byte header that contains a 7-hex-digit
signature and a single-hex-digit index of the arena to which the
block belongs. The index will help the release() function identify
the appropriate arena. The header values are:

  0xab51ead0 => arena 0
  0xab51ead1 => arena 1
  0xab51ead2 => arena 2

If the arena corresponding to the request size has no free blocks,
the allocate() function returns a NULL pointer. Also, if the function
is called with a size that is non-positive or that is too large to
fit within the largest blocksize, the function returns a NULL pointer.

Note that, unlike some other popular memory allocation techniques,
if a particular arena is fully allocated, there is no splitting or
coalescing of block from other arenas and no system calls to obtain
more blocks.

My allocate() function uses helper functions and looks like this:

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

The free-block count for the appropriate arena is decremented if an
allocation is successful.

*/
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
char* list_allocate( int type ){
    switch(type){
        case 1:
        return NULL;
        break;
        
        case 2:
        break;
    }
}
/*
The release() function returns a valid block to the appropriate bitmap
or free list. The function performs several sanity checks on the memory
address passed by the caller, including alignment and a simple range
check. If the address passes those checks, a pointer to the header
(i.e., the start of the block) is constructed and the signature in
the header of checked. These sanity checks can use this code:

  char *ptr;
  long long unsigned int header, *header_ptr;

  if( (long long int) release_ptr & 0x7 ){
    printf( "pointer not aligned on 8B boundary in release() function\n" );
    printf( "  => no action taken\n" );
    return;
  }
  if( ( release_ptr < min_address ) || ( release_ptr > max_address )  ){
    printf( "pointer out of range in release() function\n" );
    printf( "  => no action taken\n" );
    return;
  }
  ptr = release_ptr - 8;
  header_ptr = (long long unsigned int *) ptr;
  header = *header_ptr;
  if( ( header & 0xfffffff0 ) != HEADER_SIGNATURE ){
    printf( "header does not match in release() function\n" );
    printf( "  => no action taken\n" );
    return;
  }

Once these checks are passed, the arena index is extracted from the
header, and the block is marked as free in the bitmap for arena 0
or returned as a node at the head of the appropriate free list for
arenas 1 and 2.

The free-block count for the appropriate arena is incremented if a
release is successful.
*/
void release( char *release_ptr ){
  //check(); 
  char *ptr;
  long long unsigned int header, *header_ptr;

  if( (long long int) release_ptr & 0x7 ){
    printf( "pointer not aligned on 8B boundary in release() function\n" );
    printf( "  => no action taken\n" );
    return;
  }
  if( ( release_ptr < min_address ) || ( release_ptr > max_address )  ){
    printf( "pointer out of range in release() function\n" );
    printf( "  => no action taken\n" );
    return;
  }
  ptr = release_ptr - 8;
  header_ptr = (long long unsigned int *) ptr;
  header = *header_ptr;
  if( ( header & 0xfffffff0 ) != HEADER_SIGNATURE ){ //mask some to last 3
    printf( "header does not match in release() function\n" );
    printf( "  => no action taken\n" );
    return;
  }
//pass;
}


