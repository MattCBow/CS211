#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct Circuit
{
	char trials;
	char in;
	char out;
	char **input;	// ([trials] x [in])
	char **output;	// ([trials] x [out])
	char **temp;	// ([trials] x [26])
} circuit;

FILE 	*test_FILE, *input_FILE;
fpos_t 	*POS;
char 	*BUFFER;

char* getVar(char trial, char var);

char getVal(char trial, char var);
void setVal(char trial, char var, char val);

void loadFiles(int argc, char **argv);
void closeFiles();

void initCircuit();
void runCircuit(char trial);
void printCircuit();

char grayCode(char *selectors, char count, char index);

void runTest();

int main(int argc, char **argv)
{
	loadFiles(argc, argv);
	initCircuit();
	runTest();
	closeFiles();
	printCircuit();
}

char grayCode(char *selectors, char count, char index)
{
	if(count == 0)
	{
		return 1;
	}
	if(index<pow(2,count-1))
	{
		char a = grayCode(selectors+1, count-1, index);
		char b = !selectors[0];
		
		return (a && b);
	}
	if(index>=pow(2,count-1))
	{
		char a = grayCode(selectors+1, count-1, (pow(2,count)-index-1));
		char b = selectors[0];
		return (a && b);
	}

}

void runTest()
{
	char i;
	for(i=1;i<=circuit.trials;i++)
	{
		runCircuit(i);
	}
}


void loadFiles(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("USAGE: comb [circuit] [test]\n");
		exit(1);
	}
	test_FILE = fopen(argv[1], "r");
	input_FILE = fopen(argv[2], "r");
	
	if(test_FILE == NULL || input_FILE == NULL)
	{
		printf("ERROR: Could not open text files\n");
		exit(1);	
	}
	POS = (fpos_t*) malloc(sizeof(fpos_t));
	BUFFER = (char*) malloc(16);
}

void closeFiles()
{
	fclose(test_FILE);
	fclose(input_FILE);
	free(BUFFER);
	free(POS);
}

void initCircuit()
{
	char x,y;
	//------------------------Set Trials-------
	circuit.trials = 0;
	while(fscanf(input_FILE,"%c",BUFFER)!=EOF)
	{
		if(BUFFER[0]=='\n')
		{
			circuit.trials++;	
		}
	}	
	rewind(input_FILE);
	//------------------------Init temp[][]----
	circuit.temp = (char**) malloc((1+circuit.trials)*sizeof(void*));
	circuit.temp[0] = (char*) malloc(26);
	for(y=0;y<26;y++)
	{
		circuit.temp[0][y] = y+97;	
	}
	for(x=1;x<=circuit.trials;x++)
	{
		circuit.temp[x] = (char*) malloc(26);
		for(y=0;y<26;y++)
		{
			circuit.temp[x][y]=0;
		}
	}
	//-----------------------Init input[][]------
	fscanf(test_FILE, "%s ", BUFFER);
	if(strcmp(BUFFER,"INPUTVAR")!=0)
	{
		printf("ERROR: Invalid Circuit Line(%s)\n", BUFFER);
		exit(1);
	}
	fscanf(test_FILE, "%d ", BUFFER);
	circuit.in = BUFFER[0];
	circuit.input = (char**) malloc((1+circuit.trials)*sizeof(void*));
	circuit.input[0] = (char*) malloc(circuit.in);
	for(y=0;y<circuit.in;y++)
	{
		fscanf(test_FILE,"%c ", BUFFER);
		circuit.input[0][y] = BUFFER[0];
	}
	for(x=1; x<=circuit.trials; x++)
	{
		circuit.input[x] = (char*) malloc(circuit.in);
		for(y=0; y<circuit.in; y++)
		{
			fscanf(input_FILE, "%d ",BUFFER);
			circuit.input[x][y] = BUFFER[0];
		}
	}
	
	//--------------------Init Output[][]------
	fscanf(test_FILE, "%s ", BUFFER);
	if(strcmp(BUFFER, "OUTPUTVAR")!=0)
	{
		printf("ERROR: Invalid Circuit Line(%s)\n", BUFFER);
		exit(1);
	}
	fscanf(test_FILE, "%d ", BUFFER);
	circuit.out = BUFFER[0];
	circuit.output = (char**) malloc((1+circuit.trials)*sizeof(void*));
	circuit.output[0] = (char*) malloc(circuit.in);
	for(y=0;y<circuit.out;y++)
	{
		fscanf(test_FILE,"%c ",BUFFER);
		circuit.output[0][y] = BUFFER[0];
	}	
	for(x=1; x<=circuit.trials;x++)
	{
		circuit.output[x] = (char*) malloc(circuit.out);
		for(y=0;y<circuit.out;y++)
		{
			circuit.output[x][y] = 0;
		}
	}
	//--------------Set Circuit Position--------
	fgetpos(test_FILE,POS);
	rewind(test_FILE);
}

char* getVar(char trial, char var)
{
	if(var>96 && var<123){	 // a is Lowercase 
		return &(circuit.temp[trial][var-97]);
	}
	else if(var>64 && var<91)	// a is Uppercase
	{
		char i;
		for(i=0;i<circuit.in;i++)
		{
			if(var==circuit.input[0][i])
			{
				return &(circuit.input[trial][i]);
			}
		}
		for(i=0;i<circuit.out;i++)
		{
			if(var==circuit.output[0][i])
			{
				return &(circuit.output[trial][i]);
			}
		}
	}else{
		printf("ERROR: Invalid Variable [A-Z,a-z]\n");
		exit(1);
	}
	return 0;

}


char getVal(char trial, char var)
{
	if(var=='0')
	{
		return 0;
	}
	if(var=='1')
	{
		return 1;
	}
	char *address = getVar(trial, var);
	if(address!=0)
	{
		return *address;
	}
	return 0;
}

void setVal(char trial, char var,char val)
{
	char *address = getVar(trial, var);
	if(address!=0)
	{
		*address = val;	
	}
	return;	
}

void runCircuit(char trial)
{
	if(fsetpos(test_FILE,POS)!=0)
	{
		printf("ERROR: Could Not Set File Position");
		exit(1);
	}
	int i;
	while(fscanf(test_FILE,"%s ",BUFFER)!=EOF)
	{
		if(strcmp(BUFFER,"NOT")==0)
		{
			fscanf(test_FILE,"%c %c",&BUFFER[0],&BUFFER[1]);
			char not;
			not  = getVal(trial, BUFFER[0]);
			setVal(trial,BUFFER[1],!not);
		}
		else if(strcmp(BUFFER,"AND")==0)
		{
			fscanf(test_FILE,"%c %c %c", &BUFFER[0], &BUFFER[1], &BUFFER[2]);
			char and[2];
			and[0] = getVal(trial,BUFFER[0]);
			and[1] = getVal(trial,BUFFER[1]);
			setVal(trial,BUFFER[2],(and[0]&&and[1]));
		} 
		else if(strcmp(BUFFER,"OR")==0)
		{
			fscanf(test_FILE,"%c %c %c", &BUFFER[0], &BUFFER[1], &BUFFER[2]);
			char or[2];
			or[0] = getVal(trial,BUFFER[0]);
			or[1] = getVal(trial,BUFFER[1]);
			setVal(trial,BUFFER[2],(or[0]||or[1])); 
		}
		else if(strcmp(BUFFER,"DECODER")==0)
		{
			char i,n;
			fscanf(test_FILE,"%d ", BUFFER);
			n = BUFFER[0];
			char input[n];
			for(i=0;i<n;i++)
			{
				fscanf(test_FILE,"%d ",BUFFER);
				input[n] = getVal(trial,BUFFER[0]);
			}
			for(i=0;i<pow(2,n);i++)
			{
				fscanf(test_FILE,"%d ",BUFFER);
				setVal(trial, BUFFER[0], grayCode(input,n,i));
			}
		} 
		else if(strcmp(BUFFER,"MULTIPLEXER")==0)
		{
			char i,n,log2, output;
			fscanf(test_FILE,"%d ", BUFFER);
			n = BUFFER[0];
			char input[n];
			for(i=0;i<n;i++)
			{
				fscanf(test_FILE,"%c ",BUFFER);
				input[i] = getVal(trial,BUFFER[0]);
			}
			log2 = (char)(log(n)/log(2));//log2(n)
			char selector[log2];
			for(i=0;i<log2;i++)
			{
				fscanf(test_FILE,"%c ",BUFFER);
				selector[i] = getVal(trial,BUFFER[0]);
			}
			output = 0;
			for(i=0;i<n;i++)
			{
				input[i] = (input[i] && grayCode(selector,log2,i));
				output+= input[i];
			}
			
			fscanf(test_FILE,"%c ",BUFFER);
			setVal(trial,BUFFER[0], output);
			
		} 
		else
		{
			printf("WTF!!!!!!!!!!!!!\n");
		} 
		
	}
	
}

void printCircuit()
{
	char x,y;
	for(x=1;x<=circuit.trials;x++)
	{	
		for(y=0;y<circuit.out;y++)
		{
			printf("%d ",circuit.output[x][y]);
		}
		printf("\n");
	}
}


