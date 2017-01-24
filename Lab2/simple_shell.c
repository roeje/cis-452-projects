/*

	CIS 452 - Lab 2: Simple Shell Example
	Jesse Roe
	01/22/2017

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

#define MAXLINE 80
#define MAXARG 20

// Declaration of background function
void background (char * cmd);

int main() {

   pid_t childPID;
   int childStatus;
   char cmd[MAXLINE];
   struct rusage *stats;
   // printf("mysh$ ");
   for (; ;) {

      printf("mysh$ ");

      // read a command from the user
      fgets(cmd, MAXLINE, stdin);

      // if the command in cmd is “exit\n”, then terminate this program;
      if (strcmp(cmd,"exit\n") == 0) {
         printf("Exit...\n");
         exit(0);
      }

      // otherwise, create a child process to handle that command.
      int value = fork();
      if (value == 0) {
         background(cmd);
         exit(0);
      }

      // Wait for child process to terminate
      childPID = wait(&childStatus);
      printf("%s\n", childPID);
      int result = getrusage(childPID, stats);
      printf("CSWITCH TIME: %ld\n", stats->ru_nivcsw);
   }

	return 0;
}

void background (char *cmd) {

   int i = 0;
   char *tmp;
   char *argv[MAXARG];

   tmp = strtok(cmd, "\t \n");
   while(tmp != NULL && tmp != "\0") {
      argv[i] = tmp;
      tmp = strtok(NULL, "\t \n");
      i = i + 1;
   }

   // Add NULL to end of array based on execvp documentation
   argv[i] = tmp;

   // Execute command
   execvp(argv[0], &argv[0]);
}
