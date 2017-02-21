#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{	
	
	int size=0, i=0;
	int sum = 0;
	int min = INT_MAX; // it is set to max in purpose so anything less than that will update the value
	int max = 0; // it is set to zero in purpose because anything greater than that will update the value
	int *numArray;
	pid_t pid1=0, pid2=0, pid3=0; //these are the process id variables we will use in forking

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

	//FORKING FOR PROC1 - max calculation
	pid1 = fork();

	if(pid1==0){ //executed by the parent
		printf("Hi I am process %d and my parent is %d\n", getpid(), getppid());
		wait();
	}
	else{ //executed by proc1 which is the child
		for(int i=0;i<size;i++){
			if (numArray[i]>max)
				max = numArray[i];
		}
		//FORKING FOR PROC2 - min calculation
		pid2 = fork();

		if (pid2==0){//executed by the child - proc1
			printf("Hi I am process %d and my parent is %d\n", getpid(), getppid());
			printf("max=%d\n",max );
			wait();
			exit(0);
		}
		else{ //executed by proc2 which is the grandchild
			for(int i=0;i<size;i++){
				if(numArray[i]<min)
					min = numArray[i];
			}
		
			//FORKING FOR PROC3 - sum calculation
			pid3 = fork();

			if(pid3==0){//executed by the grandchild - proc2
				printf("Hi I am process %d and my parent is %d\n", getpid(), getppid());
				printf("min=%d\n",min );
				wait();
				exit(0);
			}
			else{ //executed by proc3 - GreatGrandChild
				for(int i=0;i<size;i++){
					sum += numArray[i];
				}
				printf("Hi I am process %d and my parent is %d\n", getpid(), getppid());
				printf("sum=%d\n",sum );
				exit(0);

			}
		}

	}


	

	











	

	return 0;
}