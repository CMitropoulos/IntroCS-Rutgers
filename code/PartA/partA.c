#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>


int main(int argc, char *argv[])
{	
	
	int size=0, i=0;
	float sum = 0;
	float min = INT_MAX; // it is set to max in purpose so anything less than that will update the value
	float max = 0; // it is set to zero in purpose because anything greater than that will update the value
	float *numArray;
	

	//read the file from the command line
	FILE* inputFile = fopen(argv[1],"r");
	//read the first line that will be the size of the array
	fscanf(inputFile, "%d", &size);
	//dynamically allocate the array size
	numArray = (float *)malloc(sizeof(float)*(size-1));
	
	while(!feof(inputFile)){ //fill up the array

		fscanf(inputFile, "%f", &numArray[i]);
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
   	

	printf("min=%f\n",min );
	printf("max=%f\n",max );
	printf("sum=%f\n",sum );

	//clock stops 
	

	return 0;
}