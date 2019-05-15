#include <stdio.h>
#include "mythreads.h"


int main(){
    threadInit();
    addList(0,0);
    addList(1,0);
    addList(2,0);
    print_list(0);
    
    delList(0);
    print_list(0);
    delList(0);
    print_list(0);
    delList(0);
    print_list(0);
    return 0;
}
