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
        int     fd[nbChildren][2], nbytes=1, status;
        pid_t   childpid;
        char    string[] = "Hello\n";
        char    readbuffer[100];
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
    FILE* outputFile ;

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
        	pipe(fd[j]);

        	if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        	if(childpid == 0)
        {
                /* Child process closes up input side of pipe */
                close(fd[j][0]);
                printf(" Hi I am the child %d and my parent is %d\n", getpid(), getppid());
                result = childFunction(j, step, numArray);
                /* Send "string" through the output side of pipe */
                    

                    sprintf(string, "%lf",result[0]);
                    strcat(string,"\n");
                    write(fd[j][1], string,  (strlen(string)));

                    sprintf(string, "%lf",result[1]);
                    strcat(string,"\n");
                    write(fd[j][1], string,  (strlen(string)));
                    
                   
                    
                    close(fd[j][1]);
                    exit(0);
        }
        else //parent
        {
               
        }
    }

    //first child reads from all its children after they are all done
    
    wait(&status);
    outputFile = fopen("partial_results.txt", "w+");
   
    for(int i=0;i<nbChildren;i++){
        close(fd[i][1]); //close the writing end of the pipe
        
        while(read(fd[i][0], readbuffer,1)>0 ){
            fprintf(outputFile, "%s", readbuffer);  
        }
        close(fd[i][0]); //close the writing end of the pipe
    
    }
        printf("\n");
        printf("\n");
    
    fclose(outputFile);

    //Find the final results - read from partial_results.txt

    FILE* partial_results = fopen("partial_results.txt","r");
    int m=0;
    while(!feof(partial_results)){ //fill up the array
        fscanf(partial_results, "%lf", &maxArray[m]);
        fscanf(partial_results, "%lf", &minArray[m]);
        m++;
    }

    double finalMax=findMaxinArray(maxArray,nbChildren);
    double finalMin=findMininArray(minArray, nbChildren);
    double finalSum = sum;
    printf("MAX=%lf\n MIN=%lf\n SUM=%lf\n", finalMax, finalMin, finalSum);


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

