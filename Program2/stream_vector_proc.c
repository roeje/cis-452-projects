
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


void sigHandler(int sigNum, siginfo_t *siginfo, void *context);
void exitHandler(int sigNum);

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

   // pid = fork();

  //  signal (SIGUSR1, sigHandler);
  //  signal (SIGUSR2, sigHandler);
  //  signal (SIGINT, exitHandler);

   int i;
   for (i = 0; i < 3; i++) {
      if ((pid = fork()) < 0) {
         printf("An error occured spawning a child process. Exiting.\n");
         exit(-1);

      } else if (pid == 0) {
         printf("spawned child PID# %d\n", getpid());
         
         // call function for this process
         
         exit(0);
      }
   }


   // if in parent process

   // signal (SIGINT, exitHandler);

   // Wait for child process to terminate
   // printf("waiting...   ");
   // fflush(stdout);
   waitpid(-1, &status, 0);
   return 0;
}

void sigHandler (int sigNum, siginfo_t *siginfo, void *context) {
    // printf (" received an interrupt.\n");
   if (sigNum == SIGUSR1) {
      printf ("received a SIGUSR1 signal from PID: %ld\n", (long)siginfo->si_pid);

   }
   if (sigNum == SIGUSR2) {
      printf ("received a SIGUSR2 signalfrom PID: %ld\n", (long)siginfo->si_pid);

   }
}


void exitHandler (int sigNum) {
    printf(" received.  That's it, I'm shutting you down...\n");
    exit(0);
}
