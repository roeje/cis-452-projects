
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>

#define MAXLINE 80
#define MAXARG 20

/**
 * CIS 452 - Lab 2: Simple Shell Example
 * @author  Jesse Roe
 * @author  Michael Kolarik
 * @version 01/22/2016
 */

// Declaration of background function
void background (char *cmd);

int main() {

   /**
    * pid_t var to store output of fork
    */
   pid_t pid;

   /**
    * Store success of getrusage() and status of wait()
    */
   int child_status;

   /**
    * Command argument string to hold value of fgets()
    */
   char cmd[MAXLINE];

   /**
    * Holds results from getrusage()
    */
   struct rusage usage;
   int who = RUSAGE_CHILDREN;
   long int time_sec = 0;
   long int time_msec = 0;
   long icw = 0;

   for (; ;) {

      // print terminal prompt
      printf("mysh$ ");

      // read a command from the user
      fgets(cmd, MAXLINE, stdin);

      // if the command in cmd is “exit\n”, terminate program;
      if (strcmp(cmd,"quit\n") == 0) {
         printf("Exiting...\n");
         exit(0);
      }

      // otherwise, create a child process to handle that command.
      pid = fork();

      // if in parent process
      if (pid) {

         // Wait for child process to terminate
         waitpid(-1, &child_status, 0);
         getrusage(who, &usage);

         // Calculate time for each unique process and output to user
         long int tmp_time_sec = usage.ru_utime.tv_sec - time_sec;
         long int tmp_time_msec = usage.ru_utime.tv_usec - time_msec;
         time_sec = usage.ru_utime.tv_sec;
         time_msec = usage.ru_utime.tv_usec;
         printf("User CPU time used: %ld.%06ld\n", tmp_time_sec, tmp_time_msec);

         long tmp_icw = usage.ru_nivcsw - icw;
         icw = usage.ru_nivcsw;
         printf("Number of involuntary context switches: %li\n", tmp_icw);

      // if error spawing child
      } else if (pid < 0) {
         printf("An error occured spawning a child process. Exiting.\n");
         exit(-1);

      // if child
      } else {
         background(cmd);
         exit(0);
      }
   }
	return 0;
}

/**
 * Run by chlid process to execute command
 * <p>
 * Builds vector of commands and passes into execvp()
 *
 * @param  command string from user
 * @return none
 */
void background (char *cmd) {

   // Interator and char pointer/string for use in strtok()
   int i = 0;
   char *tmp;
   char *argv[MAXARG];

   // read in fisrt element of command array
   tmp = strtok(cmd, "\t \n");

   // interate over rest of array and store results in argument array
   while(tmp != NULL) {
      argv[i] = tmp;
      tmp = strtok(NULL, "\t \n");
      i = i + 1;
   }

   // Add NULL to end of array based on execvp documentation
   argv[i] = tmp;

   // Execute command
   execvp(argv[0], &argv[0]);
}
