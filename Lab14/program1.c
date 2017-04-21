#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * CIS 452 - Lab 12: [Extra Credit] Programming Assignment (Psuedo-random Number Generator)
  *
 * @author  Jesse Roe
 * @author Michael Kolarik
 * @version 04/20/2017
 */

int main() {

   int i = 0;
   unsigned int seed;
   FILE *fp;

   /* Open /dev/random file */
   fp = fopen("/dev/random", "r");

   /* read in random unsigned int */
   fread(&seed, sizeof(&seed), 1, fp);

   /*Seed random() with random value*/
   srandom(seed);

   /*Generate 10 random values*/
   for(i = 0; i < 10; i++) {
      printf("%ld\n", random());
   }

   /*Close file*/
   fclose(fp);
   return 0;
}
