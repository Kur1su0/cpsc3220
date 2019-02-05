Zice Wei

## Project 1.
### KNOWN PROBLEMS: 
                1.program 2 : additional exit_group(231).
                              missed execve.
### DESIGN:
                prog 1  1. designed with link list.
                        2. fork+execvp.
                
                prog 2  1. designed wirh array.
                        2. fork+execvp.
#### reference:
[64-bit syscall table](http://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/)
[sample code for ptrace](https://stackoverflow.com/questions/11081859/how-to-trace-a-process-for-system-calls)
