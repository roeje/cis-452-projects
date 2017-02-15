
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

#define MEMSIZE 4096
#define MAXLINE 80

/**
 * CIS 452 - Lab 5: Communicating Process
 * @author  Jesse Roe
 * @author  Michael Kolarik
 * @version 02/13/2017
 */

void exit_handler(int);

int string_mem_id;
int flag_mem_id;

int main() {

   signal (SIGINT, exit_handler);


   /**
    * pid_t var to store output of fork
    */
   pid_t pid, parent_pid;


   key_t mem_key;
   mem_key = 6666;
   key_t flag_key;
   flag_key = 7777;
   char cmd[MAXLINE];

   char *string_mem;
   int *flag_mem;

   if ((string_mem_id = shmget(mem_key, MEMSIZE, IPC_CREAT | 0666)) < 0) {
      perror ("Could not create string mem\n");
      exit(1);
   }

   if ((flag_mem_id = shmget (flag_key, MEMSIZE, IPC_CREAT | 0666)) < 0) {
      perror ("Could not create flag mem\n");
      exit(1);
   }

   if ((string_mem = shmat (string_mem_id, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit(1);
   }

   if ((flag_mem = shmat (flag_mem_id, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit(1);
   }

   while(1) {

      fprintf(stdout, "Enter Text: \n");

      // read a command from the user
      fgets(cmd, MAXLINE, stdin);
      cmd[strcspn(cmd, "\n")] = 0;
      strcpy(string_mem, cmd);
      *flag_mem = 0;
      printf("String is: %s\n", string_mem);
      printf("Flag is: %d\n", *flag_mem);
   }

   if (shmdt (string_mem) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }

   if (shmdt (flag_mem) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }

   // printf ("value a: %lu\t value b: %lu\n", (unsigned long) shmPtr, (unsigned long) shmPtr + FOO);




   return 0;
}

void exit_handler (int sigNum) {
   printf(" received.  That's it, I'm shutting you down...\n");

   if (shmctl (string_mem_id, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   if (shmctl (flag_mem_id, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   exit(0);
}
