#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//declare functions
float* childFunction(int j,int step,int* array);
float findMininArray(float* array, int size );
float findMaxinArray(float* array, int size );


int main(int argc, char *argv[])
{       int     nbChildren = 5;   //Number of children - must be able to divide the array in same size segments
        int     fd[nbChildren][2], nbytes=1;
        pid_t   childpid;
        char    string[] = "Hello\n";
        char    readbuffer[100];

        
        int     size=0;
        int     *numArray;
        float     *result,*maxArray,*minArray, sum=0.0, tempSum=0.0;
        result = (float *)malloc(sizeof(int)*(3));
        maxArray = (float *)malloc(sizeof(int)*(nbChildren));
        minArray = (float *)malloc(sizeof(int)*(nbChildren));





        //read the file from the command line
    FILE* inputFile = fopen(argv[1],"r");
    //read the first line that will be the size of the array
    fscanf(inputFile, "%d", &size);
    //dynamically allocate the array size
    numArray = (int *)malloc(sizeof(int)*(size-1));
    int k=0;
    while(!feof(inputFile)){ //fill up the array
        fscanf(inputFile, "%d", &numArray[k]);
        k++;
    }

    fclose(inputFile);



    //output file
    FILE* outputFile ;

    int step = size/nbChildren;


  printf("Hi I am the parent %d\n", getpid());
        
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
                    

                    sprintf(string, "%f",result[0]);
                    strcat(string,"\n");
                    write(fd[j][1], string,  (strlen(string)));

                    sprintf(string, "%f",result[1]);
                    strcat(string,"\n");
                    write(fd[j][1], string,  (strlen(string)));
                    
                    sprintf(string, "%f",result[2]);
                    strcat(string,"\n");
                    write(fd[j][1], string,  (strlen(string)));
                    
                    close(fd[j][1]);
                    exit(0);
        }
        else //parent
        {
               
        }
    }

    //parent reads from all children after they are all done
    
    wait();
    outputFile = fopen("partial_results.txt", "w+");
   
    for(int i=0;i<nbChildren;i++){
        close(fd[i][1]); //close the writing end of the pipe
        
        while(read(fd[i][0], readbuffer,1)>0 ){
           //float number = atof(readbuffer);
            
            //printf("%.0f\n", number );
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
        fscanf(partial_results, "%f", &maxArray[m]);
        fscanf(partial_results, "%f", &minArray[m]);
        fscanf(partial_results, "%f", &tempSum);
        sum+=tempSum;
        m++;
    }

    float finalMax=findMaxinArray(maxArray,nbChildren);
    float finalMin=findMininArray(minArray, nbChildren);
    float finalSum = sum;

    printf("MAX=%f\n MIN=%f\n SUM=%f\n", finalMax, finalMin, finalSum);


        return(0);
}

float* childFunction(int j,int step,int* array){
        float min = (float)INT_MAX; // it is set to max in purpose so anything less than that will update the value
        float max =0 ; // it is set to zero in purpose because anything greater than that will update the value
        float *result;
        float sum=0.0;
        result = (float *)malloc(sizeof(float)*(3));

        for(int k=j*step;k<(j+1)*step ; k++){
            if(array[k]>max){
                max = array[k];
            }
            if (array[k]<min){
                min = array[k];
            }
            
            sum+=(float)array[k];
        }
        result[0]=max;
        result[1]=min;
        result[2]=sum;
        return result;
    }

float findMininArray(float* array, int size ){
    float min = array[0];
    for(int i=0;i<size;i++ ){
        if (array[i]<min)   
            min = array[i];
    }
    return min;
}

float findMaxinArray(float* array, int size ){
    float max = array[0];
    for(int i=0;i<size;i++ ){
        if (array[i]>max)   
            max = array[i];
    }
    return max;
}