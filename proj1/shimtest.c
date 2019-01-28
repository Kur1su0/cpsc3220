
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv)
{
	srand(argc);
	for (int i=0; i < 5; i++)
	{
		int r = rand();
		printf("#%d -> %d\n",i, r);
	}
	return 0;
}