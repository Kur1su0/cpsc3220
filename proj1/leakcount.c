
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define MAXSIZE 80

int main(int argc, char **argv)
{
	//read command line and compose.
	char command[MAXSIZE];
	sprintf(command,"%s",argv[1]);
	if (argc > 2){
		for(int i=2;  i<argc; i++ ){
			sprintf(command,"%s %s",command,argv[i]);
	    }	
	}

    //read to fork.
    if(!fork()){ //child
	    if(argc==2){
			;
			printf("%s",argv[1]);
		}
		else{
			//printf()
		}


	}else{ //parent


	}

	printf("%s\n",command);
	return 0;

}

