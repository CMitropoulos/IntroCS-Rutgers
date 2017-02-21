#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>


int main(int argc, char *argv[])
{	
	//clock begins here
	clock_t begin = clock();
	
	int size=0, i=0;
	int sum = 0;
	int min = INT_MAX; // it is set to max in purpose so anything less than that will update the value
	int max = 0; // it is set to zero in purpose because anything greater than that will update the value
	int *numArray;
	

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

	//for loop for calculating the max, min and sum
	for(int i=0;i<size;i++){
		if(numArray[i]<min)
			min = numArray[i];
		else if (numArray[i]>max)
			max = numArray[i];
		sum += numArray[i];
	}


//print process number and parent process number
	 printf("Hi I am process %d and my parent is %d\n", getpid(), getppid());
   	

	printf("min=%d\n",min );
	printf("max=%d\n",max );
	printf("sum=%d\n",sum );

	//clock stops 
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

		printf("time spent=%f\n",time_spent);

	return 0;
}