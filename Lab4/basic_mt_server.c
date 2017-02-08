
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

int file_request_count;
float file_access_time;

pthread_mutex_t count_lock;
pthread_mutex_t avg_lock;

int main() {

   pthread_t thread1;
   int status;
   char cmd[MAXLINE];

   srand(time(NULL));

   signal (SIGINT, exit_handler);

   while(1) {

      fprintf(stdout, "Enter Filename: \n");

      // read a command from the user
      fgets(cmd, MAXLINE, stdin);

      cmd[strcspn(cmd, "\n")] = 0;

      status = pthread_create (&thread1, NULL,  fetch_data, &cmd);

      if (status != 0) {
         fprintf (stderr, "Thread spawn error %d: %s\n", status, strerror(status));
         exit (1);
      } else {

         pthread_mutex_lock(&count_lock);
         file_request_count++;
         pthread_mutex_unlock(&count_lock);
      }

      status = pthread_detach (thread1);
      if (status != 0) {
        fprintf (stderr, "Detach error %d: %s\n", status, strerror(status));
        exit (1);
      }
   }

   return 0;
}

void *fetch_data(void *filename) {
   char *fname = (char *) filename;

   int wait_time = (rand()%10) + 1;
   int sleep_time = 0;

   if (wait_time <= 2) {
      sleep_time = 10 - (rand()%4);

      pthread_mutex_lock(&avg_lock);
      file_access_time += sleep_time;
      pthread_mutex_unlock(&avg_lock);

      sleep(sleep_time);
   } else {
      sleep_time = 1;

      pthread_mutex_lock(&avg_lock);
      file_access_time += sleep_time;
      pthread_mutex_unlock(&avg_lock);

      sleep(sleep_time);
   }
   fprintf(stdout, "\nFile \"%s\" located after %d seconds\n", fname, sleep_time);


   return 0;
}

void exit_handler (int sigNum) {
    fprintf(stdout, " received.  That's shutting down...\n");
    fprintf(stdout, "Files Requested: %d\n", file_request_count);
    fprintf(stdout, "Average File Request Time: %f seconds\n", (float)(file_access_time/file_request_count));
    exit(0);
}
