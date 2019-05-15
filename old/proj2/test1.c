#include <stdio.h>
#include "mythreads.h"
void* func1(void* a){
    int arg = *((int*)a);
    printf("func1 args: %d\n",arg);
}


int main(){
    printf("hello TEST\n");
    threadInit();
    int a =3;
    threadCreate(func1,(void*)&a);

return 0;
}
