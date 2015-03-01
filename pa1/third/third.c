#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

FILE *file;
size_t fileSize;
size_t n = 0;
char *fContent;
int indx = 0;
char next = '\0';

typedef struct node {
	int i;
	struct node *next;
}node;

node *head = NULL;

// returns the next letter or number
char nextChar()
{
	int ifint = fContent[indx] == 'i' || fContent[indx] == 'd';
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
	if(head == NULL)
	{
		head = (node *) malloc(sizeof(node));
		head -> next = NULL;
		head -> i = in;
		return;
	}
	// make the node and some pointer variables
	node *inserted = (node *) malloc(sizeof(node));
	inserted -> i = in;
	inserted -> next = NULL;

	node *before = NULL;
	node *after = head;

	// find where the node should go
	while(1)
	{
		if(after == NULL)
			break;
		if(after -> i > inserted -> i)
			break;
		if(after -> i == inserted -> i)
			return;
		before = after;
		after = after -> next;
	}
	// insert the node
	if(before != NULL)
		before -> next = inserted;
	inserted -> next = after;
	if(after == head)
		head = inserted;
}

void delete(int in)
{
	node *temp = head;
	node *before = NULL;
	// find the node
	while(1)
	{
		if(temp == NULL)
			return;
		if(temp -> i == in)
			break;
		before = temp;
		temp = temp -> next;
	}
	// remove the node
	if(before != NULL)
		before -> next = temp -> next;
	else
		head = temp -> next;
	free(temp);
}

void printList()
{
	node *temp = head;
	while(1)
	{
		if(temp == NULL)
			return;
		printf("%i", temp -> i);
		if(temp -> next != NULL)
			printf("\t");
		temp = temp -> next;
	}
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
		return 0; //empty file - dont do anything
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
		} else if(next == 'd')
		{
			delete(nextInt());
		} else
		{
			printf("error");
			exit(1);
		}
	}
	printList();
	return 0;
}
