
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
 * CIS 452 - Project 2
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

   // Parent Process - Complementer

   FILE *input_file, *write, *read, *output_file;

   char file_buffer[binary_length];
   char pipe_buffer[binary_length];

   // int file_write = 0, file_read = 0;
   //
   // dup2(pipe_one[WRITE], file_write);
   // dup2(pipe_one[READ], file_read);
   //
   // close(pipe_one[WRITE]);
   // close(pipe_one[READ]);

   //
   // write = fdopen(file_write, "w");
   // read = fdopen(file_read, "r");


   write = fdopen(pipe_one[WRITE], "w");
   read = fdopen(pipe_one[READ], "r");

   input_file = fopen(fileA, "r");
   output_file = fopen("test.out", "w");


   fprintf(stdout, "Waiting for Ctrl^C confirmation...\n");
   while (initial_pause) {
      ;
   }
   signal (SIGINT, exit_handler);





   while(fgets(file_buffer, binary_length + 3, input_file) != NULL) {

      file_buffer[strcspn(file_buffer, "\r\n")] = 0;
      // fgets(file_buffer, binary_length + 3, input_file) != NULL

      // int success = fread(file_buffer, binary_length + 1, 1, input_file);

      // if (success == 0) {
      //    return 0;
      // }

      fprintf(stdout, "TEST: Read binary string from FILE: %s\n", file_buffer);


      fwrite(file_buffer, sizeof(file_buffer) + 1, 1, write);
      // fwrite("\n", 1, 1, write);
      fflush(write);

      fprintf(stdout, "Data written to PIPE...\n");

      fread(pipe_buffer, sizeof(pipe_buffer) + 1, 1, read);


      fprintf(stdout, "TEST: Read binary string from PIPE: %s\n", pipe_buffer);

      char *binary_output = file_buffer;

      fwrite(pipe_buffer, binary_length, 1, output_file);
      fwrite("\n", 1, 1, output_file);
   }

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
