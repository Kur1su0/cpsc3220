/* sctracer.c
 * purpose: trace system calls using ptrace and
 * save into a givin file path.
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <fcntl.h>
#include <string.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <signal.h>

#define MAXSIZE 1000

/* write_to_file: write sys calls info into given path
 * input        : table of sys calls, output file path.
 * no return val.
 *
 */
void write_to_file(long system_call_table[MAXSIZE], char* output_file);

int main(int argc, char **argv) {
    
    /* Split command in " ".  */
    char*ptr = strtok(argv[1]," ");
    char* cmd[MAXSIZE] ={NULL};
    char* output_file = argv[2];
    int len = 0;
    long current_call;
    long last_call;
    int status = 0;
    long system_call_table[MAXSIZE]= {(long)0};
    pid_t child;
    
    /* split the second arg */
    while(ptr!=NULL){
        cmd[len++] = ptr; 
        ptr = strtok(NULL," ");
    }
    cmd[len] = NULL;
    len++;
    
    child = fork();  
    if(child==0){
        ptrace(PTRACE_TRACEME,getpid(),NULL,NULL);
        execvp(cmd[0],cmd);
        

    }
    else{
    //Note: call ptrace;
    //      ptrace(PTRACE_TRACEME);
    //      ptrace(PTRACE_SYSCALL);
    //      ptrace(PTRACE_PEEKUSER);
    //      ptrace(PTRACE_SETOPTIONS);
    //      wait pid
    //      getpid.
        //Distinguish between sigtop and other system call.
        //ptrace(PTRACE_SETOPTIONS,child,0,PTRACE_O_TRACESYSGOOD);
        ptrace(PTRACE_SETOPTIONS,child,0,PTRACE_O_EXITKILL);
        while(1){
            //wait(&status);
            waitpid(child,&status,0);
            if(WIFEXITED(status) || WIFSIGNALED(status)){
                break;
            }
            if(WIFSTOPPED(status) && WSTOPSIG(status)==SIGTRAP){
                current_call = ptrace(PTRACE_PEEKUSER, child,sizeof(long)*ORIG_RAX,NULL);
                system_call_table[(int)current_call]++;
            }
            ptrace(PTRACE_SYSCALL,child,0,0);
        }

    }

    /* clean duplicated info */
    last_call = current_call;
    for(int i=0;i < MAXSIZE;i++){ 
        if(system_call_table[i]!=0 && i!=last_call){
            system_call_table[i]/=(long)2;
        }
    }
    
    /* write to file*/
    write_to_file(system_call_table,output_file);
    
    return 0;
}

void write_to_file(long table[MAXSIZE], char* file_path){
    int File;
    //chmod 666: All user can read and write 
    if(File<0) perror("err open file");
    File = open(file_path,O_CREAT|O_RDWR,0666); //Not a Magic number.
    char info[MAXSIZE];
    for(int i=0;i < MAXSIZE;i++){ 
        if(table[i]!=0){
            sprintf(info,"%d\t%ld\n",i,table[i]);
            write(File,info,strlen(info));
        }
    }

    close(File);
}


