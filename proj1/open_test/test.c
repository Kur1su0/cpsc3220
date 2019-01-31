#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define PATH "log.txt"
int main(){

int pfd;
if ((pfd = open(PATH, O_WRONLY | O_CREAT | O_APPEND)) == -1)
    {
        perror("Cannot open output file\n"); exit(1);
    }
   
    write(pfd,"success\n",8);

return 0;
}
