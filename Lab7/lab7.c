#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <Windows.h>


#define PRECISION 15

/**
* CIS 452 - Lab 7: CPU Scheduling Under Windows
*
* @author  Jesse Roe
* @author  Michael Kolarik
* @version 03/14/2017
*/

int main()
{

   /*Various variables to store HR counter data*/
   __int64 perf_freq = 0;
   __int64 perf_count_start = 0;
   __int64 perf_count_end = 0;
   __int64 perf_count_diff = 0;
   double perf_period = 0;
   double duration = 0;
   int i = 0;

   // HANDLE main_process_handle = NULL;
   // DWORD main_process_priority;

   /*Get HR counter frequency*/
   QueryPerformanceFrequency((LARGE_INTEGER *) &perf_freq);

   if (perf_freq != 0) {
      printf("High-Resolution Counter Frequency: %I64d tick/sec\n", perf_freq);
   } else {
      printf("Error: High-res counter freq could not be fetched\n");
   }

   /*Calculate period of HR-counter converted to msec*/
   perf_period = ((double)1/perf_freq) * 1000;

   printf("High-Res Counter Period: %.10f msec/tick\n", perf_period);

   /*Get HR counts before and after loop*/
   QueryPerformanceCounter((LARGE_INTEGER *) &perf_count_start);
   for(i = 0; i < 1000000; i++){};
   QueryPerformanceCounter((LARGE_INTEGER *) &perf_count_end);

   /*Calculate number of ticks that occured during loop*/
   perf_count_diff = perf_count_end - perf_count_start;
   printf("Ticks for 1 mil loop: %I64d ticks\n", perf_count_diff);

   /*Calculate duration of loop*/
   duration = perf_period * perf_count_diff;

   printf("Duration of 1 mil loop: %.10f msec\n", duration);


   /*Get HR counts before and after call to HR-counter*/
   QueryPerformanceCounter((LARGE_INTEGER *) &perf_count_start);
   /*Call to HR-counter*/
   QueryPerformanceCounter((LARGE_INTEGER *) &perf_count_end);
   QueryPerformanceCounter((LARGE_INTEGER *) &perf_count_end);

   perf_count_diff = perf_count_end - perf_count_start;
   printf("Ticks for call to HR-counter: %I64d ticks\n", perf_count_diff);

   /*Calculate duration of loop*/
   duration = perf_period * perf_count_diff;

   printf("Duration of call to HR-counter: %.10f msec\n", duration);

   /*Extra Credit Segment: Process Priority*/

   // main_process_handle = GetCurrentProcess();

   return 0;
}
