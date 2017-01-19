
/*****************************************************************
Program 1 - Simple system call/C libary functions demo. Displays various
system variables and other user data obtained through various system calls
and C library functions.

@author Jese Roe
@course CIS 452
@version 01/17/2017
*****************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>

/* Max input string size*/
#define MAXLINE 255

/* Start of main function */
int main() {

   /* local variables */
   char cmd[MAXLINE];
   char host_name[MAXLINE];
   char *value;
   struct passwd *pwd;

   /* Get login name */
   value = getlogin();
   if (value) {
      printf("User is: %s\n", value);
   } else {
      printf("User is: login name not found\n");
   }

   /*
    * Get User ID
    * No error checking since getuid is always successful
   */
   printf("UID is %d\n", getuid());

   /*
    * Get Group ID
    * No error checking since getgid is always successful
   */
   printf("GID is %d\n", getgid());

   /* Get host name */
   int host_success = gethostname(host_name, MAXLINE);

   /* Check for success and output appropriate message to user */
   if (host_success == 0) {
      printf("host is: %s\n", host_name);
   } else {
      printf("host is: host name not found\n");
   }

   /* For UID's 0 - 4 print out the pw_name */
   int i;
   for (i = 0; i < 5; i++) {

      /* Get passwd struct from getpwuid for each user id */
      pwd = getpwuid(i);

      /* Check for success and output appropriate message to user */
      if (pwd) {
         printf("entry %d: %s\n", i, pwd->pw_name);
      } else {
         printf("entry %d: not found\n", i);
      }
   }

   /* Add some format spacing to ouput */
   printf("\n");

   /* Start input loop prompting user for ENV command */
   for (; ;) {

      printf("Enter ENV variable (or \"exit\" to quit): ");

      /* read a command from the user */
      scanf("%s", cmd);

      /* if cmd is “exit”, then terminate the program */
      if (strcmp(cmd, "exit") == 0) {
         printf("Exiting...\n");
         exit(0);
      } else {

         /* Get ENV variable based on cmd string */
         value = getenv(cmd);

         printf("\n");
         
         /* Check if cmd is valid and return appropriate output */
         if (value) {
            printf("value is: %s\n", value);
         } else {
            printf("command not recognized... \n");
         }
      }

      /* Add some formatting */
      printf("\n");
   }
	return 0;
}
