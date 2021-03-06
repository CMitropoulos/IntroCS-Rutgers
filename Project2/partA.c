#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/prctl.h>

//declare functions
double* childFunction(int j,int step,double* array);
double findMininArray(double* array, int size );
double findMaxinArray(double* array, int size );

//GLOBAL VARIABLES
double max = 0,min=999999999999;
int     nbChildren = 4;   //Number of children - must be able to divide the array in same size segments

int processIds[6];

static void signalHandler(int sig, siginfo_t *siginfo, void *context){
    char pid_string[8];
    switch(sig){
      case SIGUSR1:
        printf ("(SIGUSR1)Sending PID: %ld\n",(long)siginfo->si_pid);
        //start function to execute the merging of the results
        int p_id = (int)siginfo->si_pid;
        double temp_min=0, temp_max=0;
        sprintf(pid_string, "%d",p_id);

        //read from permanent files     
        FILE* inputFile = fopen(pid_string,"r");
        fscanf(inputFile, "%lf", &temp_max);
        fscanf(inputFile,"%lf", &temp_min);
        fclose(inputFile);
        int ret= remove(pid_string);
        //printf("Max=%lf\n MIN=%lf\n", temp_max,temp_min);

        if(temp_min<min)
            min = temp_min;
        if(temp_max>max)
            max=temp_max;

        break;
      case SIGALRM: //a process is delayed so we kill it
        printf ("(SIGALRM)Sending PID: %ld THIS DIES\n",(long)getpid());
        kill(getpid(), SIGKILL);

        break;
      case SIGINT:
        printf ("(SIGINT - Ctr+C)Sending PID: %ld\n",(long)siginfo->si_pid);
        for(int i=0;i<nbChildren;i++){
            kill(processIds[i],SIGKILL);
        }
        break;
      case SIGTSTP:
         printf ("(SIGSTP - Ctr+Z)Sending PID: %ld\n",(long)siginfo->si_pid);
        for(int i=0;i<nbChildren;i++){
            kill(processIds[i],SIGKILL);
        }
        break;

    }
}

int main(int argc, char *argv[])
{   
    
        int     status;
        pid_t   childpid;
        
        pid_t pid1=0;
        
        int     size=0;
        double     *numArray;
        double     *result,*maxArray,*minArray, sum=0.0;
        result = (double *)malloc(sizeof(double)*(3));
        maxArray = (double *)malloc(sizeof(double)*(nbChildren));
        minArray = (double *)malloc(sizeof(double)*(nbChildren));
        processIds[0]=getpid();


  //REGISTER THE HANDLER FOR THE USER ACTIONS
    struct sigaction user_act;
            memset (&user_act, '\0', sizeof(user_act));
            /* Use the sa_sigaction field because the handles has two additional parameters */
            user_act.sa_sigaction = &signalHandler;
            /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
            user_act.sa_flags = SA_SIGINFO;
            if (sigaction(SIGINT, &user_act, NULL) < 0) {
                perror ("sigint");
                return 1;
            }
            if (sigaction(SIGTSTP, &user_act, NULL) < 0) {
                perror ("sigint");
                return 1;
            }
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
  
            

    
    signal(SIGALRM, SIG_IGN);
    //Fork for the first child that will spawn all the other processes
    pid1 = fork();

    if(pid1==-1){
        perror("fork");
        exit(1);
    }
    else if(pid1==0){//child process -> this is where we create all the other children
        printf(" Hi I am the child %d and my parent is %d\n", getpid(), getppid());
        processIds[1]=getpid();
       
        for(int j=0;j<nbChildren;j++){
            


            if((childpid = fork()) == -1) //this is where the grandchildren are forked
        {
                perror("fork");
                exit(1);
        }

            if(childpid == 0) //grandchildren -> all the magic happens here
        {       
                switch(j){
                    case 0:
                        processIds[2]=getpid();
                    case 1:
                        processIds[3]=getpid();
                    case 2:
                        processIds[4]=getpid();
                    case 3:
                        processIds[5]=getpid();
                }
                //Alarm so that if some process takes more than 3 seconds it is terminated
                alarm(3); //after 3 seconds
                if(j==3)
                    sleep(10);
                printf(" Hi I am the grandchild %d and my parent is %d\n", getpid(), getppid());
                result = childFunction(j, step, numArray);
                //write result in a file with the process id as a name
                FILE *p_results_file;

                char pid_string[8];
                int pid_number = getpid();
                sprintf(pid_string, "%d",pid_number);
                p_results_file = fopen(pid_string,"w+");

                fprintf(p_results_file, "%lf\n", result[0] );
                fprintf(p_results_file, "%lf\n", result[1] );

                fclose(p_results_file);
                //TODO: send signal that the results are ready to read
                kill(getppid(),SIGUSR1);
                exit(0);
        }
        else //parent
        {
            
            printf("THE CHILD AFTER THE FORKING\n");
           // signal(SIGALRM, SIG_IGN);
            struct sigaction act;
            memset (&act, '\0', sizeof(act));
            /* Use the sa_sigaction field because the handles has two additional parameters */
            act.sa_sigaction = &signalHandler;
            /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
            act.sa_flags = SA_SIGINFO;
            if (sigaction(SIGUSR1, &act, NULL) < 0) {
                perror ("sigusr1");
                return 1;
            }
              if (sigaction(SIGALRM, &act, NULL) < 0) {
                perror ("sigalarm");
                return 1;
            }

                sleep(5);
                


            


        }
    }

    //first child reads when a signal is received that a child has finished
    
      
    

printf("Max=%lf\n MIN=%lf\n SUM=%lf\n", max,min,sum);
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
