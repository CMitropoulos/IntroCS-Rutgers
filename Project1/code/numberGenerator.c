#include <time.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
   FILE *fp;
   int r=0;
 //WRITE 10numbers
   fp = fopen("input/10MNumbers.txt", "w");
   srand(time(NULL));
   if(fp == NULL){
   	fprintf(stderr, "Cannot open file");
   } 
   else{ //this is where we write the random numbers
      fprintf(fp, "%d", 10000000 );
   	
      for(int i=0;i<10000000;i++){
   		r = rand();
		fprintf(fp, "\n%d", r );
   	}    	
   }  
   
   fclose(fp);

/*
 //WRITE 100numbers
   fp = fopen("input/100Numbers.txt", "w");
   if(fp == NULL){
   	fprintf(stderr, "Cannot open file");
   }
   else{ //this is where we write the random numbers
      fprintf(fp, "%d", 100 );
   	
      for(int i=0;i<100;i++){
   		r = rand();
		fprintf(fp, "\n%d", r );
   	}   
	 }
   fclose(fp);

//WRITE 1knumbers
   fp = fopen("input/1kNumbers.txt", "w");
   if(fp == NULL){
   	fprintf(stderr, "Cannot open file");
   }
else{ //this is where we write the random numbers
      fprintf(fp, "%d", 1000 );
   	
      for(int i=0;i<1000;i++){
   		r = rand();
		fprintf(fp, "\n%d", r );
   	 }  
	}    
   fclose(fp);

 //WRITE 10knumbers
   fp = fopen("input/10kNumbers.txt", "w");
   if(fp == NULL){
   	fprintf(stderr, "Cannot open file");
   }
else{ //this is where we write the random numbers
      fprintf(fp, "%d", 10000 );
   	

      for(int i=0;i<10000;i++){
   		r = rand();
		fprintf(fp, "\n%d", r );
   		}
   }      
   fclose(fp);

 //WRITE 100knumbers
   fp = fopen("input/100kNumbers.txt", "w");
   if(fp == NULL){
   	fprintf(stderr, "Cannot open file");
   }
else{ //this is where we write the random numbers
   	fprintf(fp, "%d",100000 );

      for(int i=0;i<100000;i++){
   		r = rand();
		fprintf(fp, "\n%d", r );
   	}      
   }
   fclose(fp);

//WRITE 1M numbers
fp = fopen("input/1MNumbers.txt", "w");
   srand(time(NULL));
   if(fp == NULL){
      fprintf(stderr, "Cannot open file");
   } 
   else{ //this is where we write the random numbers
      fprintf(fp, "%d", 1000000 );
      
      for(int i=0;i<1000000;i++){
         r = rand();
      fprintf(fp, "\n%d", r );
      }     
   }  
   
   fclose(fp);

*/




   return 0;
}
