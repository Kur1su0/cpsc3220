
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
    //char* cmd[argc];
    //int i;
    //for(i=0;i<argc-1;i++) cmd[i]=argv[i+1];
    
    //cmd[argc]=NULL;
    //for(i=0;i<argc-1;i++) printf("%s ",cmd[i]); printf("\n");

    //char* envs[]={"LD_PRELOAD=./memory_shim.so",NULL};
    //read to fork.
    if(!fork()){ //child
        setenv("LD_PRELOAD","./memory_shim.so",0);
		execvp(argv[1],&argv[1]);
		//execve(cmd[0],cmd,envs);
        unsetenv("LD_PRELOAD");

	}else{ 
		//parent
        wait(NULL);  
		//count for leaks and frees
	}
    _exit(0);

	return 0;

}

