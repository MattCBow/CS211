#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

FILE *file;
size_t fileSize;
size_t n = 0;
char *fContent;
char next = '\0';
int indx = 0;
int *table;

// returns the next letter or number
char nextChar()
{
	int ifint = fContent[indx] == 'i' || fContent[indx] == 's';
	if(ifint)
	{
		indx += 2;
		return fContent[indx - 2];
	} else
	{
		printf("error");
		exit(1);
	}
}

int nextInt()
{
	int start = indx;
	while(fContent[indx] != '\n')
	{
		indx++;
	}
	fContent[indx] = '\0';
	indx++;
	return atoi(&fContent[start]);
}

void insert(int in)
{
	int spot = in % 1000;
	if(table[spot] == 0)
	{
		table[spot] = in;
		printf("inserted\n");
		return;
	}
	while(table[spot] != 0)
		spot++;
	table[spot] = in;
	printf("duplicate\n");
}

void search(int in)
{
	int spot = in % 1000;
	while(1)
	{
		if(spot == in % 1000 - 1)
		{
			printf("absent\n");
			return;
		}
		if(table[spot] == in)
		{
			printf("present\n");
			return;
		}
		spot++;
		spot %= 1000;
	}
}

int main(int argc, char *argv[])
{
	// init the table to all zeroes
	table = (int*) malloc(1000*sizeof(int));
	int wgwe = 0;
	for(wgwe = 0; wgwe < 1000; wgwe++)
		table[wgwe] = 0;
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
	
	// go through tokens and execute functions using token values
	while(fContent[indx] != '\0')
	{
		next = nextChar();
		if(next == 'i')
		{
			insert(nextInt());
		} else if(next == 's')
		{
			search(nextInt());
		} else
		{
			printf("error");
			exit(1);
		}
	}
	return 0;
}
