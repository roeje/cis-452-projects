
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

int main(int argc, char **argv) {

   signal (SIGINT, exit_handler);

   if (argc != 2) {
      fprintf(stdout, "Need to enter reader id, try again.\n");
      exit(0);
   }

   int uid = atoi(argv[1]);


   key_t mem_key = 6666;
   key_t flag_key = 7777;

   char *string_mem;
   int *flag_mem;

   if ((string_mem_id = shmget(mem_key, MEMSIZE, 0666)) < 0) {
      perror ("Could not create string mem\n");
      exit (1);
   }

   if ((flag_mem_id = shmget (flag_key, MEMSIZE, 0666)) < 0) {
      perror ("Could not create flag mem\n");
      exit (1);
   }

   if ((string_mem = shmat (string_mem_id, NULL, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }

   if ((flag_mem = shmat (flag_mem_id, NULL, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }

   while(1) {
      while (flag_mem[uid] == 0) {
          ;
      }
      printf("String is: %s\n", string_mem);
      printf("Flag is: %d\n", *flag_mem);
      flag_mem[uid] = 0;
   }

   if (shmdt (string_mem) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }

   if (shmdt (flag_mem) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }

   if (shmctl (string_mem_id, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   if (shmctl (flag_mem_id, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

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
