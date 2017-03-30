#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

//declare functions
double* childFunction(int j,int step,double* array);
double findMininArray(double* array, int size );
double findMaxinArray(double* array, int size );


int main(int argc, char *argv[])
{	
	
 int     nbChildren = 5;   //Number of children - must be able to divide the array in same size segments
        int     status;
        pid_t   childpid;
        
        pid_t pid1=0;
        
        int     size=0;
        double     *numArray;
        double     *result,*maxArray,*minArray, sum=0.0;
        result = (double *)malloc(sizeof(double)*(3));
        maxArray = (double *)malloc(sizeof(double)*(nbChildren));
        minArray = (double *)malloc(sizeof(double)*(nbChildren));
/*
Reading the file must be done in the parent process so that
all the chidren share the same array values afterwards
*/

	//read the file from the command line
	FILE* inputFile = fopen(argv[1],"r");
	//read the first line that will be the size of the array
	fscanf(inputFile, "%d", &size);
	//dynamically allocate the array size
	numArray = (double *)malloc(sizeof(double)*(size));
	int k=0;
	while(!feof(inputFile)){ //fill up the array

		fscanf(inputFile, "%lf", &numArray[k]);
		sum += numArray[k];
		k++;
	}

	fclose(inputFile);
	    //output file
    

    int step = size/nbChildren;

	printf("Hi I am parent process %d\n", getpid());

	//Fork for the first child that will spawn all the other processes
	pid1 = fork();

	if(pid1==-1){
		perror("fork");
		exit(1);
	}
	else if(pid1==0){//child process -> this is where we create all the other children
        printf(" Hi I am the child %d and my parent is %d\n", getpid(), getppid());

  		for(int j=0;j<nbChildren;j++){
        	

        	if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        	if(childpid == 0) //grandchildren -> all the magic happens here
        {
                /* Child process closes up input side of pipe */
                
                printf(" Hi I am the child %d and my parent is %d\n", getpid(), getppid());
                result = childFunction(j, step, numArray);
                //write result in a file with the process id as a name
                FILE *p_results_file;

                char pid_string[8];
                int pid_number = getpid();
                sprintf(pid_string, "%d",pid_number);
                p_results_file = fopen(pid_string,"w+");

                fprintf(p_results_file, "%lf\n", result[0] );
                fprintf(p_results_file, "%lf\n", result[1] );

                    
                    exit(0);
        }
        else //parent
        {
               
        }
    }

    //first child reads when a signal is received that a child has finished
    
      
    


free(result);
free(maxArray);
free(minArray);
	
	}
	else{//parent process - nothing really done here

	}

wait(&status);

return 0;
}

double* childFunction(int j,int step,double* array){
        double min = (double)INT_MAX; // it is set to max in purpose so anything less than that will update the value
        double max =0 ; // it is set to zero in purpose because anything greater than that will update the value
        double *result;
        double sum=0.0;
        result = (double *)malloc(sizeof(double)*(2));

        for(int k=j*step;k<(j+1)*step ; k++){
            if(array[k]>max){
                max = array[k];
            }
            if (array[k]<min){
                min = array[k];
            }
        }
        result[0]=max;
        result[1]=min;
        return result;
    }

double findMininArray(double* array, int size ){
    double min = array[0];
    for(int i=0;i<size;i++ ){
        if (array[i]<min)   
            min = array[i];
    }
    return min;
}

double findMaxinArray(double* array, int size ){
    double max = array[0];
    for(int i=0;i<size;i++ ){
        if (array[i]>max)   
            max = array[i];
    }
    return max;
}

