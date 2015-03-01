#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

FILE *file;
size_t fileSize;
size_t n = 0;
char *fContent;
char next = '\0';
int indx = 0;
int *m1;
int *m2;

// returns the next number
int nextInt()
{
	int start = indx;
	while(fContent[indx] != '\n' && fContent[indx] != '\t')
	{
		indx++;
	}
	fContent[indx] = '\0';
	indx++;
	return atoi(&fContent[start]);
}

void nextMatrix()
{
	while(fContent[indx] == '\n' || fContent[indx] == '\t')
		indx++;
}

int main(int argc, char *argv[])
{
	// check for input errors, open the file, make sure it exists
	if (argc < 2)
	{
		printf("error");
		exit(1);
	}
	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		printf("error");
		exit(1);
	}

	// allocate memory to store the contents of the file
	fseek(file, 0, SEEK_END); // seek to file end
	fileSize = ftell(file); // store indx of last character
	if(fileSize == 0)
		return 0; //we good
	rewind(file);  // seek back to beginning
	fContent = malloc(fileSize + 1); // allocate that much memory

	// copy the file to the content buffer and make sure it worked
	if(fread(fContent, fileSize, 1, file) != 1)
	{
		printf("error");
		exit(1);
	}
	
	int m = nextInt();
	int n = nextInt();
	
	// initialize the matrices as arrays of length m*n
	m1 = (int*) malloc(m*n*sizeof(int));
	m2 = (int*) malloc(m*n*sizeof(int));
	int i = 0;
	for(i = 0; i < m * n; i++)
		m1[i] = nextInt();
	nextMatrix();
	for(i = 0; i < m * n; i++)
		m2[i] = nextInt();
	int j = 0;
	for(i = 0; i < m; i++)
	{
		for(j = 0; j < n; j++)
		{
			printf("%i", m1[n*i+j] + m2[n*i+j]);
			if(j != n-1)
				printf("\t");
		}
		if(i != m-1)
			printf("\n");
	}
	return 0;
}
