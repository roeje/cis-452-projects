/*

	CIS 452 - Lab 2: Simple Shell Example
	Jesse Roe
	01/22/2017

*/

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

// Declaration of background function
void background (char * cmd);

int main() {

   pid_t child_pid, pid;
   int child_status, result;
   char cmd[MAXLINE];
   struct rusage usage;
   int who = RUSAGE_CHILDREN;
   // printf("mysh$ ");
   for (; ;) {

      printf("mysh$ ");

      // read a command from the user
      fgets(cmd, MAXLINE, stdin);

      // if the command in cmd is “exit\n”, then terminate this program;
      if (strcmp(cmd,"quit\n") == 0) {
         printf("Exiting...\n");
         exit(0);
      }

      // otherwise, create a child process to handle that command.
      pid = fork();
      if (pid) {

         /*struct timeval {
             time_t      tv_sec;
             suseconds_t tv_usec;
         }*/

         // Wait for child process to terminate
         child_pid = waitpid(-1, &child_status, 0);
         result = getrusage(who, &usage);
         printf("User CPU time used: %ld.%06ld\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
         printf("Number of involuntary context switches: %li\n", usage.ru_nivcsw);

      } else if (pid < 0) {
         printf("An error occured spawning a child process. Exiting.\n");
         exit(-1);
      } else {
         background(cmd);

         exit(0);
      }
   }
	return 0;
}

void background (char *cmd) {

   int i = 0;
   char *tmp;
   char *argv[MAXARG];

   tmp = strtok(cmd, "\t \n");
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
