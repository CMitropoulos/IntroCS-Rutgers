#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

/*function declaration */
//int DoWorkInChild(int i, int * numArray, int size);


int main(int argc, char *argv[])
{	
	
	int size=0, i=0;
	int sum = 0;
	int min = INT_MAX; // it is set to max in purpose so anything less than that will update the value
	int max = 0; // it is set to zero in purpose because anything greater than that will update the value
	int *numArray;
	pid_t pid1=0, pid2=0, pid3=0; //these are the process id variables we will use in forking

/*
/*
Reading the file must be done in the parent process so that
all the chidren share the same array values afterwards
*/

	//read the file from the command line
	FILE* inputFile = fopen(argv[1],"r");
	//read the first line that will be the size of the array
	fscanf(inputFile, "%d", &size);
	//dynamically allocate the array size
	numArray = (int *)malloc(sizeof(int)*(size-1));
	
	while(!feof(inputFile)){ //fill up the array

		fscanf(inputFile, "%d", &numArray[i]);
		i++;
	}

	fclose(inputFile);

//START THE FORKING 
	pid1 = fork();
	if(pid1>0){ //executed by the parent
		printf("Hi I am the process0 %d and my parent is %d\n", getpid(), getppid());
		pid2 = fork();
		if(pid2>0){//executed by parent - proc0
			pid3 = fork();
			if(pid3>0){
				printf("These theparent after fork3\n");
			}
			else{ //executed by proc3
				printf("Hi I am the process3 %d and my parent is %d\n", getpid(), getppid());
				exit(0);
			}
		}
		else{ //executed by proc2
			printf("Hi I am the process2 %d and my parent is %d\n", getpid(), getppid());
			exit(0);

		}

		}
	else{//executed by proc1
		printf("Hi I am the process1 %d and my parent is %d\n", getpid(), getppid());
		exit(0);



	}


	return 0;
}

/*
int DoWorkInChild(int i, int* numArray, int size){
	int result=0, sum=0;
	int min = INT_MAX; // it is set to max in purpose so anything less than that will update the value
	int max = 0; // it is set to zero in purpose because anything greater than that will update the value
	if(i==0){ //calculate max
		for(int j=0;j<size;j++){
			if (numArray[j]>max)
				max = numArray[j];
				//result = max;
		}
		printf("max=%d\n",max );

	}
	else if(i==1){//calculate min
		for(int j=0;j<size;j++){
				if(numArray[j]<min)
					min = numArray[j];
					result = min;
			}
		printf("min=%d\n",result );
	}
	else if(i==2){ //calculate sum
		for(int j=0;j<size;j++){
					sum += numArray[j];
				}
		//result = sum;
		printf("sum=%d\n",sum );

	}
	return 0;
}*/