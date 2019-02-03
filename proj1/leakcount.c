
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXSIZE 80

int main(int argc, char **argv)
{
    
    //read to fork.
    if(!fork()){ //child
		execvp(argv[1],&argv[1]);

	}else{ 
		//parent
        wait(NULL);  
		//count for leaks and frees
	}
    abort();

	return 0;

}

