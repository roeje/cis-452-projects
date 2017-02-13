
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>

/**
 * CIS 452 - Lab 3: Communicating Process
 *
 * Creates child process and handles signals sent by child
 *
 * @author  Jesse Roe
 * @author  Michael Kolarik
 * @version 01/22/2016
 */
void sigHandler(int);
void exitHandler(int);

int main() {

   // Seed rand and establish signal handlers
   srand(time(NULL));
   signal (SIGUSR1, sigHandler);
   signal (SIGUSR2, sigHandler);


   /**
    * pid_t var to store output of fork
    */
   pid_t pid, parent_pid;

   /**
    * Store success of getrusage() and status of wait()
    */
   int status;

   // Get pid of parrent process for later
   parent_pid = getpid();

   // otherwise, create a child process to handle that command.
   pid = fork();

   // if in parent process
   if (pid) {
     signal (SIGINT, exitHandler);

      // Wait for child process to terminate
      waitpid(-1, &status, 0);
      return 0;

   // if error spawing child
   } else if (pid < 0) {
      printf("An error occured spawning a child process. Exiting.\n");
      exit(-1);

   // if child
   } else {
      printf("spawned child PID# %d\n", getpid());

      // Loop and send kill commands at random intervals
      for(; ;) {
         fprintf(stderr, "waiting...   ");
         int wait_time = (rand()%5) + 1;
         sleep(wait_time);

         int user_sig = rand()%2;

         if (user_sig) {
            kill(parent_pid, SIGUSR1);

         } else {
            kill(parent_pid, SIGUSR2);
         }
      }
   }
   return 0;
}

/**
 * Signal handler function to display data to user
 * <p>
 *
 * @param  int signal value
 * @return none
 */
void sigHandler (int sigNum) {

  //  Based on signal value revieced, output msg to user
   if (sigNum == SIGUSR1) {
      printf ("received a SIGUSR1 signal.\n");
   }
   if (sigNum == SIGUSR2) {
      printf ("received a SIGUSR2 signal.\n");
   }
}

/**
 * Signal handler function to handle termination of processes
 * <p>
 *
 * @param  int signal value
 * @return none
 */
void exitHandler (int sigNum) {
    printf(" received.  That's it, I'm shutting you down...\n");
    exit(0);
}
