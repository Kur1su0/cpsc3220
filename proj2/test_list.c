#include <stdio.h>
#include "mythreads.h"


int main(){
    threadInit();
    threadLock(0);
    threadLock(0);
    threadLock(0);
    return 0;
}
