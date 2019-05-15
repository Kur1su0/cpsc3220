/* leakcount.c
 * Purpose: 
 *
 *
 *
 */
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAXSIZE 1000

int main(int argc, char **argv)
{
    //read to fork.
    if(!fork()){ //child
		//execve(cmd[0],cmd,envs);
        setenv("LD_PRELOAD","./memory_shim.so",0);
		execvp(argv[1],&argv[1]);
        unsetenv("LD_PRELOAD");

	}else{ 
		//parent
        wait(NULL);  
		//count for leaks and frees
	}
    _exit(0);

	return 0;

}

