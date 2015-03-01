#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("error\n");
		return 1;
	}
	long n = atol(argv[1]);
	int i = 2;
	for (; i < n; i++)
		if (n % i == 0)
		{
			printf("no\n");
			return 0;
		}
	printf("yes\n");
	return 0;
}
