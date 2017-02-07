
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


void* fetch_data(char *filename);

void exitHandler (int sigNum);

int main() {

   pthread_t thread1;
   int status;
   char cmd[MAXLINE];

   signal (SIGINT, exitHandler);

   while(1) {

      printf("mysh$ ");

      // read a command from the user
      fgets(cmd, MAXLINE, stdin);

      status = pthread_create (&thread1, NULL,  fetch_data, &val[0])

      if (status != 0) {
         fprintf (stderr, "thread spawn error %d: %s\n", status, strerror(status));
         exit (1);
      }


   }





   return 0;
}


void* fetch_data(char *filename) {


   return 0
}

void exitHandler (int sigNum) {
    printf(" received.  That's it, I'm shutting you down...\n");
    exit(0);
}
