#include <time.h>
#include <stdio.h>



main() {
   FILE *fp;

   fp = fopen("/input/100numbers.txt", "w");
   fprintf(fp, "This is testing for fprintf...\n");
   fputs("This is testing for fputs...\n", fp);
   fclose(fp);
}
