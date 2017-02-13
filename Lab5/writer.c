
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>


/**
 * CIS 452 - Lab 5: Communicating Process
 * @author  Jesse Roe
 * @author  Michael Kolarik
 * @version 02/13/2017
 */

void exit_handler(int);

int main() {

   signal (SIGINT, sig_handler);


   /**
    * pid_t var to store output of fork
    */
   pid_t pid, parent_pid;


   return 0;
}

void exit_handler (int sigNum) {
    printf(" received.  That's it, I'm shutting you down...\n");
    exit(0);
}
