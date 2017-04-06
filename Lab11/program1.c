#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <Windows.h>

/**
* CIS 452 - Lab 11: Memory Management Under Windows
*
* @author  Jesse Roe
* @author  Michael Kolarik
* @version 04/1/2017
*/

void displayState(MEMORY_BASIC_INFORMATION data);

int main() {


   /*Variables to Store Memory data*/
   SYSTEM_INFO info;
   MEMORY_BASIC_INFORMATION data;
   int one_meg = 1048576;
   char *big_data;

   // Get System info
   GetSystemInfo(&info);

   printf("Page Size is: %lu\n", info.dwPageSize);

   /*Allocate large chunk of mem*/
   big_data = (char *)malloc(one_meg);

   /*Just to experiment, we fill the malloced data, had no effect on the results*/
   // for (int i = 0; i < one_meg; i++) {
   //    big_data[i] = (int) rand();
   // }

   /*Get info on big_data*/
   VirtualQuery(big_data, &data, sizeof(data));
   displayState(data);
   free(big_data);
   VirtualQuery(big_data, &data, sizeof(data));
   displayState(data);

   return 0;
}

/**
 * Determines mem State and prints
 * <p>
 *
 * @param  MEMORY_BASIC_INFORMATION data
 * @return none
 */
void displayState(MEMORY_BASIC_INFORMATION data) {

   /*Match State to appropriate Category*/
   if(data.State == 4096) {
      printf("Memory State is: Committed.\n");

   } else if (data.State == 65536) {
      printf("Memory State is: Free.\n");

   } else if (data.State == 8192) {
      printf("Memory State is: Reserved.\n");

   } else {
      printf("The State: %ld is not Recognized...\n", data.State);

   }
}
