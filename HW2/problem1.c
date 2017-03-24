#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#define NUM_THREADS  2

/* TO COMPILE USE command 
   gcc -pthread problem1.c -o problem1 -lm
 */
struct thread_data{
	int thread_id;
	int size;
	int *array;
};

struct thread_data thread_data_array[NUM_THREADS];


/*returns the reverse of a number*/
int reverse(int n){
	int r;
	for (r=0;n;n/=10)
		r=r*10 + (n%10);
	return r;
}

void *FindPrime(void *threadData){
	struct thread_data *my_data;
	my_data =(struct thread_data *)threadData;
	int n = my_data->size;
	long tid;
	for(int i=2;i<sqrt(n);i++){
		if(my_data->array[i]==1){
			for(int j=i*i;j<n;j=j+i){
				my_data->array[j]=0;
			}
		}
	}
	/*for(int i=2;i<my_data->size;i++){
		if(my_data->array[i]==1)
			printf("%d\n", i );
	}*/
	printf("Thread %ld done.\n",tid);
    pthread_exit((void*) threadData);
}
void *FindSubset(void *threadData){
	struct thread_data *my_data;
	my_data =(struct thread_data *)threadData;
	int n = my_data->size;
	long tid;
	
	//find the subset
	printf("the numbers in the subset are \n");
	for(int i=2;i<my_data->size;i++){
		if(reverse(i)<my_data->size && my_data->array[i]==1 && my_data->array[reverse(i)]==1 && i!=reverse(i))
			printf("%d \n", i );


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
       printf("Main: completed join with thread \n");
       printf("The prime numbers less than the user input are:\n");
       
       for(int i=2;i<thread_data_array[0].size;i++){
		if(thread_data_array[0].array[i]==1)
			printf("%d\n", i );
	}

	  /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    rc = pthread_join(threads[0], &status);

	/*Create thread to find the subset of the prime numbers
	that the number that is derived by 
	reversing the digits is also prime */
	thread_data_array[1].thread_id = 0;
	thread_data_array[1].size = n+1;
	thread_data_array[1].array = primeArray;

	int rc2 = pthread_create(&threads[1], &attr, FindSubset, (void*) &thread_data_array[1]);
       if (rc2){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }

	  /* Free attribute and wait for the other threads */
   	
   	pthread_attr_destroy(&attr);
    rc = pthread_join(threads[1], &status);


 	printf("Main: program completed. Exiting.\n");
 	pthread_exit(NULL);
 	return 0;
 }

   

   
