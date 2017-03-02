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
	numArray = (double *)malloc(sizeof(double)*(size));
	
	while(!feof(inputFile)){ //fill up the array

		fscanf(inputFile, "%lf", &numArray[i]);
		if(numArray[i]<min)
			min = numArray[i];
		else if (numArray[i]>max)
			max = numArray[i];
		sum += numArray[i];
		i++;
		
	}

	fclose(inputFile);



//Write results in file
FILE* outputFile = fopen("results_partA.txt","w+");
//print process number and parent process number and results	
	fprintf(outputFile,"Hi I am process %d and my parent is %d\n", getpid(), getppid());
   	fprintf(outputFile,"min=%lf\n",min );
	fprintf(outputFile,"max=%lf\n",max );
	fprintf(outputFile,"sum=%lf\n",sum );
	fclose(outputFile);
	free(numArray);

	return 0;
}