#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS  2

struct thread_data{
	int thread_id;
	int size;
	int *array;
};

struct thread_data thread_data_array[NUM_THREADS];


void *FindPrime(void *threadData){
	struct thread_data *my_data;
	my_data =(struct thread_data *)threadData;
	
	long tid;

	for(int i=0;i<my_data->size;i++){
		printf("Array %d\n", my_data->array[i] );
	}
	printf("Thread %ld done.\n",tid);
    pthread_exit((void*) threadData);
}


int main(int argc, char const *argv[])
{

	//read user input
	int n;
	scanf("%d", &n);

	int *primeArray;
	
	void *status;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	/* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	

//Allocate memory for the primeArray and initialize
	//all values to 1
	primeArray =(int*)malloc(sizeof(int)*(n+1));
	for(int i=0;i<n+1;i++){
		primeArray[i]=1;

	}

	//Create thread to find all the prime numbers
	//set the arguments we will pass to the thread
	thread_data_array[0].thread_id = 0;
	thread_data_array[0].size = n+1;
	thread_data_array[0].array =(int*)malloc(sizeof(int)*(n+1));
	thread_data_array[0].array = primeArray;

	int rc = pthread_create(&threads[0], &attr, FindPrime, (void*) &thread_data_array[0]);
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }

   /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    rc = pthread_join(threads[0], &status);
	if (rc) {
          printf("ERROR; return code from pthread_join() is %d\n", rc);
          exit(-1);
          }
       printf("Main: completed join with thread ");
       



	//Create thread to find the subset of the prime numbers
	//that the number that is derived by 
	//reversing the digits is also prime 

 	printf("Main: program completed. Exiting.\n");
 	pthread_exit(NULL);
 	return 0;
 }

   

   
