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


typedef struct trace_info{
    int sys_num;
    int time;
    struct trace_info* next;   
}info;

info* HEAD = NULL;
info* TAIL = NULL;

void write_to_file(long system_call_table[MAXSIZE], char* output_file);

//int* add_syscall_info(long* info[MAXSIZE], long id){
//    add_syscall_info[(int)id]+=1;
//    return info;
//}
int main(int argc, char **argv) {
    //TODO : ptrace;
    
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
        //kill(getpid(),SIGKILL);
        
    //TODO: call ptrace;
    //      ptrace(PTRACE_TRACEME);
    //      ptrace(PTRACE_SYSCALL);
    //      ptrace(PTRACE_PEEKUSER);
    //      ptrace(PTRACE_SETOPTIONS);
    //      wait pid
    //      getpid.
    //execvp(cmd[0],cmd);

    }
    else{
        //Distinguish between sigtop and other system call.
        //ptrace(PTRACE_SETOPTIONS,child,0,PTRACE_O_TRACESYSGOOD);
        ptrace(PTRACE_SETOPTIONS,child,0,PTRACE_O_TRACEEXIT);
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
    last_call = current_call;
    for(int i=0;i < MAXSIZE;i++){ 
        if(system_call_table[i]!=0 && i!=last_call){
            system_call_table[i]/=(long)2;
        }
    }
    write_to_file(system_call_table,output_file);
    

    return 0;
}

void write_to_file(long table[MAXSIZE], char* file_path){
    int File;
    File = open(file_path,O_CREAT|O_RDWR,0666); //Not a Magic number.
    if(File<0) perror("err open file");
    char info[MAXSIZE];
    for(int i=0;i < MAXSIZE;i++){ 
        if(table[i]!=0){
            sprintf(info,"%d\t%ld\n",i,table[i]);
            write(File,info,strlen(info));
        }
    }

    close(File);
}


