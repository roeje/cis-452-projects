
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
 * @author  Jesse Roe
 * @author  Michael Kolarik
 * @version 01/22/2016
 */


void sigHandler(int);
void exitHandler(int);

int main() {

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

   // otherwise, create a child process to handle that command.
   parent_pid = getpid();

   pid = fork();

  //  signal (SIGUSR1, sigHandler);
  //  signal (SIGUSR2, sigHandler);
  //  signal (SIGINT, exitHandler);


   // if in parent process
   if (pid) {
     signal (SIGINT, exitHandler);

      // Wait for child process to terminate
      // printf("waiting...   ");
      // fflush(stdout);
      waitpid(-1, &status, 0);
      return 0;

   // if error spawing child
   } else if (pid < 0) {
      printf("An error occured spawning a child process. Exiting.\n");
      exit(-1);

   // if child
   } else {
      printf("spawned child PID# %d\n", getpid());

      for(; ;) {
         fprintf(stderr, "waiting...   ");
         int wait_time = (rand()%5) + 1;
         sleep(wait_time);

         int user_sig = rand()%2;

         if (user_sig) {
            kill(parent_pid, SIGUSR1);

         }else {
            kill(parent_pid, SIGUSR2);
         }
        //  fprintf(stderr, "waiting...   ");
        //  fflush(stdout);
      }
   }
   return 0;
}

void sigHandler (int sigNum) {
    // printf (" received an interrupt.\n");
   if (sigNum == SIGUSR1) {
      printf ("received a SIGUSR1 signal.\n");

      // signal (SIGUSR1, sigHandler);
      // signal (SIGINT, exitHandler);
   }
   if (sigNum == SIGUSR2) {
      printf ("received a SIGUSR2 signal.\n");

      // signal (SIGUSR2, sigHandler);
      // signal (SIGINT, exitHandler);
   }
}


void exitHandler (int sigNum) {
    printf(" received.  That's it, I'm shutting you down...\n");
    exit(0);
}
