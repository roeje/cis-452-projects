
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>

#define MAXLINE 80

void *fetch_data(void *filename);

void exit_handler (int sigNum);

/**
 * CIS 452 - Lab 4: Introduction to pthreads and Multi-threaded Programming
 *
 * Simulates a Multi-threaded file server, after user request, a thread is spawned
 * and simulates a search by file name
 *
 * @author  Jesse Roe
 * @author  Michael Kolarik
 * @version 02/08/2017
 */

// Global vars for stats recording
int file_request_count;
float file_access_time;

// Define mutexes for global vars
pthread_mutex_t count_lock;
pthread_mutex_t avg_lock;

int main() {

   //  thread var
   pthread_t thread1;
   int status;

   //  user input string
   char cmd[MAXLINE];

   //  Seed rand
   srand(time(NULL));

   //  Setup signal for ctrl c exit
   signal (SIGINT, exit_handler);

   //  Prompt user for input and spawn off thread
   while(1) {

      fprintf(stdout, "Enter Filename: \n");

      // read a command from the user
      fgets(cmd, MAXLINE, stdin);

      // Remove newline character from end of input
      cmd[strcspn(cmd, "\n")] = 0;

      // Spawn new thread
      status = pthread_create (&thread1, NULL,  fetch_data, &cmd);

      // Verify thread was spawned correctly
      if (status != 0) {
         fprintf (stderr, "Thread spawn error %d: %s\n", status, strerror(status));
         exit (1);
      } else {

         // If thread created correctly lock count_loca and increment
         pthread_mutex_lock(&count_lock);
         file_request_count++;
         pthread_mutex_unlock(&count_lock);
      }

      // Detach from child thread
      status = pthread_detach (thread1);

      // Check for successful detach
      if (status != 0) {
        fprintf (stderr, "Detach error %d: %s\n", status, strerror(status));
        exit (1);
      }
   }

   return 0;
}

/**
 * Thread function to simulate random file access time.
 * Searches for file by passed file name and generates a random access time.
 * <p>
 *
 * @param  void* file name
 * @return void*
 */
void *fetch_data(void *filename) {

   // Create copy of file name and cast as char*
   char *fname = (char*) filename;
   char *name = (char *) malloc((sizeof(char) * MAXLINE));
   strcpy(name, fname);
   *name = *fname;

   // Generate a random wait time
   int wait_time = (rand()%10) + 1;
   int sleep_time = 0;

   //  20% of time sleep for 7 - 10 seconds, 80% sleep for 1 second
   if (wait_time <= 2) {
      sleep_time = 10 - (rand()%4);

      // Lock global sleep counter
      pthread_mutex_lock(&avg_lock);
      file_access_time += sleep_time;
      pthread_mutex_unlock(&avg_lock);

      sleep(sleep_time);
   } else {
      sleep_time = 1;

      // Lock global sleep counter
      pthread_mutex_lock(&avg_lock);
      file_access_time += sleep_time;
      pthread_mutex_unlock(&avg_lock);

      sleep(sleep_time);
   }

  //  Output file info to user after simulated file search
   fprintf(stdout, "\nFile \"%s\" located after %d seconds\n", name, sleep_time);


   return 0;
}

/**
 * Signal handler for forced termination via ctrl c
 * <p>
 *
 * @param  int signal value
 * @return none
 */
void exit_handler (int sigNum) {

    //
    fprintf(stdout, " received.  That's shutting down...\n");
    fprintf(stdout, "Files Requested: %d\n", file_request_count);
    fprintf(stdout, "Average File Request Time: %f seconds\n", (float)(file_access_time/file_request_count));
    exit(0);
}
