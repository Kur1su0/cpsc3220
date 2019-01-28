#define _GNU_SOURCE

void __attribute__ ((constructor)) rand_init(void);
void __attribute__ ((destructor)) cleanup(void);

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


int (*original_rand)(void) = NULL;

// Called when the library is unloaded
void cleanup(void)
{
  //you would use this function to close any files that were still open
  //or do any other clean up.
  printf("cleaning up.\n");
}


void rand_init(void)
{
  if (original_rand == NULL)
  {
  	original_rand = dlsym(RTLD_NEXT, "rand");
  }
  printf("initializing library.\n");
}


int rand (void)
{
  return original_rand() % 100;
}
