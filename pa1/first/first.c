#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("error\n");
		return 1;
	}
	int n = atoi(argv[1]);
	if (n % 2 == 0)
		printf("even");
	else
		printf("odd");
	printf("\n");
	return 0;
}
