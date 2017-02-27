#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>


int main(int argc, char *argv[])
{	
	
	int size=0, i=0;
	double sum = 0;
	double min = INT_MAX; // it is set to max in purpose so anything less than that will update the value
	double max = 0; // it is set to zero in purpose because anything greater than that will update the value
	double *numArray;
	

	//read the file from the command line
	FILE* inputFile = fopen(argv[1],"r");
	//read the first line that will be the size of the array
	fscanf(inputFile, "%d", &size);
	//dynamically allocate the array size
	numArray = (double *)malloc(sizeof(double)*(size-1));
	
	while(!feof(inputFile)){ //fill up the array

		fscanf(inputFile, "%lf", &numArray[i]);
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
   	

	printf("min=%lf\n",min );
	printf("max=%lf\n",max );
	printf("sum=%lf\n",sum );

	free(numArray);

	return 0;
}