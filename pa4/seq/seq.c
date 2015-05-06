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

char 	CLOCK;

void loadFiles(int argc, char **argv);
void closeFiles();

char* getVar(char trial, char var);

char getVal(char trial, char var);
void setVal(char trial, char var, char val);

char grayCode(char *selectors, char count, char index);

void initTrials();
void initTemp();
void initInput();
void initOutput();
void initClock();

void runNOT(char trial);
void runAND(char trial);
void runOR(char trial);
void runDEC(char trial);
void runMUX(char trial);
void runFLOP(char trial);

void runTrial(char trial);
void printCircuit();

void runCircuit();

int main(int argc, char **argv)
{
	loadFiles(argc, argv);
	initTrials();
	initTemp();	
	initInput();
	initOutput();
	initClock();
	runCircuit();
	closeFiles();
	printCircuit();
}

void runCircuit()
{
	char i;
	for(i=1;i<=circuit.trials;i++)
	{
		runTrial(i);
	}
}

void runTrial(char trial)
{
	if(fsetpos(test_FILE,POS)!=0)
	{
		printf("ERROR: Could Not Set File Position");
		exit(1);
	}
	char c = getVal(trial,CLOCK);
	while(fscanf(test_FILE,"%s ",BUFFER)!=EOF)
	{
		if(strcmp(BUFFER,"NOT")==0)
		{
			if(c==0)
			{
				runNOT(trial);
			}
		}
		else if(strcmp(BUFFER,"AND")==0)
		{
			if(c==0)
			{
				runAND(trial);
			}		
		} 
		else if(strcmp(BUFFER,"OR")==0)
		{
			if(c==0)
			{
				runOR(trial);
			}		
		}
		else if(strcmp(BUFFER,"DECODER")==0)
		{
			if(c==0)
			{
				runDEC(trial);
			}
		} 
		else if(strcmp(BUFFER,"MULTIPLEXER")==0)
		{
			if(c==0)
			{
				runMUX(trial);	
			}
		} 
		else if(strcmp(BUFFER,"DFLIPFLOP")==0)
		{
			if(c==1)
			{
				runFLOP(trial);
			}
				
		}
		else
		{
			//printf("WTF!!!!!!(%s)\n",BUFFER);
		} 
		
	}
	
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
void initTrials()
{
	fscanf(input_FILE,"%s ",BUFFER);
	fscanf(input_FILE,"%d ",BUFFER);
	circuit.trials = BUFFER[0];
	rewind(input_FILE);
}

void initInput()
{
	char x,y;
	fscanf(test_FILE, "%s ", BUFFER);
	if(strcmp(BUFFER,"INPUTVAR")!=0)
	{
		printf("ERROR: Invalid Circuit Line(%s)\n", BUFFER);
		exit(1);
	}
	fscanf(test_FILE, "%d ", BUFFER);
	circuit.in = BUFFER[0];
	circuit.input = (char**) malloc((1+circuit.trials)*sizeof(void*));
	for(x=0;x<=circuit.trials;x++)
	{
		circuit.input[x] = malloc(circuit.in*sizeof(char));
	}	
	for(y=0;y<circuit.in;y++)
	{
		fscanf(test_FILE, "%c ", BUFFER);
		circuit.input[0][y]=BUFFER[0];
		fscanf(input_FILE, "%s ", BUFFER);
		fscanf(input_FILE, "%d ", BUFFER);	
		for(x=1;x<=circuit.trials;x++)
		{
			fscanf(input_FILE, "%d ", BUFFER);
			circuit.input[x][y] = BUFFER[0];
		}
	}
	rewind(input_FILE);
}

void initOutput()
{
	char x,y;
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
}


void initTemp()
{
	char x,y;
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
}

void initClock()
{
	fscanf(test_FILE, "%s ", BUFFER);
	if(strcmp(BUFFER, "CLOCK")!=0)
	{
		printf("ERROR: Invalid Circuit Line(%s)\n", BUFFER);
		exit(1);
	}
	fscanf(test_FILE, "%c ", BUFFER);
	CLOCK = BUFFER[0];
	fgetpos(test_FILE,POS);
	while(fscanf(test_FILE, "%s ",BUFFER)!=EOF)
	{
		if(strcmp(BUFFER, "DFLIPFLOP")==0)
		{
			char init,var;
			fscanf(test_FILE, "%d",BUFFER);
			init = BUFFER[0];
			fscanf(test_FILE, "%c %c %c %c", &BUFFER[0], &BUFFER[1], &BUFFER[2], &BUFFER[3]);
			var = BUFFER[3];
			setVal(1,var,init);
		}
	}
	rewind(test_FILE);
}

char* getVar(char trial, char var)
{
	if(var>96 && var<123){	 // a is Lowercase 
		return &(circuit.temp[trial][var-97]);
	}
	else if(var>64 && var<91)// a is Uppercase
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
	}
	printf("ERROR: Invalid Variable [A-Z,a-z]\n");
	exit(1);
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
	return *address;
}

void setVal(char trial, char var,char val)
{
	char *address = getVar(trial, var);
	*address = val;	
	return;	
}

void runNOT(char trial)
{
	fscanf(test_FILE,"%c %c",&BUFFER[0],&BUFFER[1]);
	char not;
	not  = getVal(trial, BUFFER[0]);
	setVal(trial,BUFFER[1],!not);
}
void runAND(char trial)
{
	fscanf(test_FILE,"%c %c %c", &BUFFER[0], &BUFFER[1], &BUFFER[2]);
	char and[2];
	and[0] = getVal(trial,BUFFER[0]);
	and[1] = getVal(trial,BUFFER[1]);
	setVal(trial,BUFFER[2],(and[0]&&and[1]));	
}
void runOR(char trial)
{
	fscanf(test_FILE,"%c %c %c", &BUFFER[0], &BUFFER[1], &BUFFER[2]);
	char or[2];
	or[0] = getVal(trial,BUFFER[0]);
	or[1] = getVal(trial,BUFFER[1]);
	setVal(trial,BUFFER[2],(or[0]||or[1])); 
}
void runDEC(char trial)
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
void runMUX(char trial)
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
	
void runFLOP(char trial)
{
	fscanf(test_FILE,"%c %c %c %c",&BUFFER[0], &BUFFER[1], &BUFFER[2], &BUFFER[3]);
	char a = getVal(trial-1, BUFFER[1]);
	setVal(trial,BUFFER[3],a);
	if(trial!=circuit.trials)
	{
		setVal(trial+1,BUFFER[3],a);
	}	
}


void printCircuit()
{
	char x,y;
	for(y=0;y<circuit.in;y++)
	{	
		printf("%c: ",circuit.input[0][y]);
		for(x=1;x<=circuit.trials;x++)
		{
			printf("%d ",circuit.input[x][y]);
		}
		printf("\n");
	}
	for(y=0;y<circuit.out;y++)
	{
		printf("%c: ",circuit.output[0][y]);
		for(x=1;x<=circuit.trials;x++)
		{
			printf("%d ",circuit.output[x][y]);
		}
		printf("\n");
	}	
}


