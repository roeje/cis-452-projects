
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
 #define READ 0
 #define WRITE 1
 #define MAX 1024

int initial_pause = 1;

void pause_handler(int sigNum);
void exit_handler(int sigNum);

int main() {

   signal (SIGINT, pause_handler);

   /**
    * pid_t var to store output of fork
    */
   pid_t pid;
   int pipe_one[2];
   int pipe_two[2];

   ssize_t num;
   char cmd[MAX];

   // TEMP SET FILE SIZES and FILE NAMES
   int binary_length = 8;

   char fileA[] = "8-input_A.dat";
   char fileB[] = "8-input_B.dat";

   /**
    * Store status of wait()
    */
   int status;

   if (pipe (pipe_one) < 0) {
      perror ("Pipe 1 Failed.");
      exit(1);
   }
   if (pipe (pipe_two) < 0) {
      perror ("Pipe 2 Failed.");
      exit(1);
   }


   int i;
   for (i = 0; i < 2; i++) {
      if ((pid = fork()) < 0) {
         printf("An error occured spawning a child process. Exiting.\n");
         exit(-1);

      } else if (pid == 0) {
         printf("spawned child PID# %d\n", getpid());
         printf("interator = %d\n", i);

         if (i == 0) {
            // Spawn Increment Process
            FILE *read, *write;

            char pipe_buffer[binary_length];

            read = fdopen(pipe_one[READ], "r");
            write = fdopen(pipe_two[WRITE], "w");

            while(1) {
               fread(pipe_buffer, binary_length, 1, read);

               fprintf(stdout, "Incrementer: Read binary string: %s\n", pipe_buffer);
               fflush(stdout);

               char *binary_output = pipe_buffer;

               fwrite(pipe_buffer, binary_length, 1, write);
            }


         } else {
            // Spawn Adder Process
            FILE *read, *input_file, *output_file;

            char pipe_buffer[binary_length];
            char input_file_buffer[binary_length];

            read = fdopen(pipe_two[READ], "r");
            input_file = fopen(fileB, "r");
            output_file = fopen("output.dat", "w");

            while(1) {
               fread(pipe_buffer, binary_length, 1, read);
               int success = fread(input_file_buffer, binary_length, 1, input_file);

               if (success == 0) {
                  break;
               }

               fprintf(stdout, "Adder: Read binary string from Pipe: %s\n", pipe_buffer);
               fprintf(stdout, "Adder: Read binary string from fileB: %s\n", input_file_buffer);
               fflush(stdout);

               char *binary_output = pipe_buffer;
               fwrite(pipe_buffer, binary_length, 1, output_file);
            }
         }

         exit(0);
      }
   }



   // Parent Process - Complementer

   FILE *input_file, *write;

   char file_buffer[binary_length];

   input_file = fopen(fileA, "r");
   write = fdopen(pipe_one[WRITE], "w");

   fprintf(stdout, "Waiting for Ctrl^C confirmation...\n");
   while (initial_pause) {
      ;
   }

   while(1) {
      int success = fread(file_buffer, binary_length, 1, input_file);

      if (success == 0) {
         break;
      }

      fprintf(stdout, "Complementer: Read binary string: %s\n", file_buffer);
      fflush(stdout);

      char *binary_output = file_buffer;

      fwrite(file_buffer, binary_length, 1, write);
   }

   // signal (SIGINT, exit_handler);
   waitpid(-1, &status, 0);
   return 0;
}



void pause_handler (int sigNum) {
    // printf (" received an interrupt.\n");
   if (initial_pause == 1) {
      fprintf (stdout, "Starting Processsing...\n");
      initial_pause = 0;

   }
   // else {
   //    fprintf(stdout, "Terminating...\n");
   //    exit(0);
   // }
}

// void pause_handler_child (int sigNum) {
//     // printf (" received an interrupt.\n");
//    if (initial_pause == 1) {
//       fprintf (stdout, "Starting Processsing...\n");
//       initial_pause = 0;
//
//    } else {
//       fprintf(stdout, "Terminating...\n");
//       exit(0);
//    }
// }


void exit_handler (int sigNum) {
    printf(" received.  That's it, I'm shutting you down...\n");
    exit(0);
}
