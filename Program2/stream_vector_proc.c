
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>

/**
 * CIS 452 - Project 2: Streamed Vector Processing
 * @author  Jesse Roe
 * @version 02/10/2017
 */

#define READ 0
#define WRITE 1
#define MAX 1024

int initial_pause = 1;

void pause_handler(int sigNum);
void exit_handler(int sigNum);

void complement_processor(char *number, int binary_len);
void increment_processor(char *number, int binary_len);
void add_processor(char *number_one, char *number_two, char *total, int binary_len);

int main(int argc, char **argv) {

   signal (SIGINT, pause_handler);

   // Verify command line parameters
   if (argc != 4) {
      fprintf(stdout, "3 arguments must be provided... Please try again.\n");
      fprintf(stdout, "Usage: input_file_A, input_file_B, binary_number_length\n");
      exit(1);
   }

   // if (!isdigit(argv[3])) {
   //    fprintf(stdout, "The 3rd argument must be a integer (ie. 8, 10, 16)... Please try again.\n");
   //    exit(1);
   // }

   /**
    * pid_t var to store output of fork
    */
   pid_t pid;

   // Setup Pipes
   int pipe_one[2];
   int pipe_two[2];


   // int binary_length = 8;
   int binary_length = atoi(argv[3]);

   // char fileA[] = "8-input_A.dat";
    char *fileA = argv[1];
   // char fileB[] = "8-input_B.dat";
    char *fileB = argv[2];

    fprintf(stdout, "FileA is: %s\n", fileA);
    fprintf(stdout, "FileB is: %s\n", fileB);
    fprintf(stderr, "Binary number length: %d\n", binary_length);

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

            char pipe_buffer[binary_length + 1];

            close(pipe_one[WRITE]);
            close(pipe_two[READ]);

            while(1) {

               int success = read(pipe_one[READ], pipe_buffer, sizeof(pipe_buffer) + 1);

               if (success == 0) {
                 break;
               }

               fprintf(stdout, "Incrementer: Read binary string from PIPE: %s    ", pipe_buffer);
               // fflush(stdout);

               increment_processor(pipe_buffer, binary_length);

               fprintf(stdout, "Incrementer: Wrote binary string to PIPE: %s\n", pipe_buffer);

               write(pipe_two[WRITE], pipe_buffer, sizeof(pipe_buffer) + 1);
            }

            close(pipe_one[READ]);
            close(pipe_two[WRITE]);


         } else {

            // Spawn Adder Process
            FILE *input_file, *output_file;

            char pipe_buffer[binary_length + 1];
            char input_file_buffer[binary_length + 1];

            close(pipe_one[WRITE]);
            close(pipe_one[READ]);
            close(pipe_two[WRITE]);

            input_file = fopen(fileA, "r");
            output_file = fopen("output.dat", "w");

            while(1) {

               read(pipe_two[READ], pipe_buffer, sizeof(pipe_buffer) + 1);

               if (fgets(input_file_buffer, binary_length + 3, input_file) == NULL) {
                  break;
               }

               // clean input of newlines and CR
               input_file_buffer[strcspn(input_file_buffer, "\r\n")] = 0;

               fprintf(stdout, "Adder: Read binary string from Pipe: %s    ", pipe_buffer);
               fprintf(stdout, "Adder: Read binary string from FileB: %s   ", input_file_buffer);
               // fflush(stdout);

               char total[binary_length + 1];

               add_processor(pipe_buffer, input_file_buffer, total, binary_length);

               fprintf(stdout, "Adder: Write binary string to Output: %s\n", pipe_buffer);
               // char *binary_output = pipe_buffer;
               fprintf(output_file, "%s\n", pipe_buffer);
            }

            close(pipe_two[READ]);
         }

         exit(0);
      }
   }

   // Parent Process - Complementer

   FILE *input_file;

   char file_buffer[binary_length + 1];

   close(pipe_one[READ]);
   close(pipe_two[READ]);
   close(pipe_two[WRITE]);

   input_file = fopen(fileB, "r");

   fprintf(stdout, "Waiting for Ctrl^C confirmation...\n");
   while (initial_pause) {
      ;
   }

   // signal (SIGINT, exit_handler);

   while(fgets(file_buffer, binary_length + 3, input_file) != NULL) {

      // remove newline and any carrage returns (windows)
      file_buffer[strcspn(file_buffer, "\r\n")] = 0;

      fprintf(stdout, "Complementer: Read binary string from FileB: %s     ", file_buffer);
      // fflush(stdout);

      complement_processor(file_buffer, binary_length);

      fprintf(stdout, "Complementer: Write binary string to PIPE: %s\n", file_buffer);

      write(pipe_one[WRITE], file_buffer, sizeof(file_buffer) + 1);

   }

   close(pipe_one[WRITE]);

   signal (SIGINT, exit_handler);
   waitpid(-1, &status, 0);
   return 0;
}

void complement_processor(char *number, int binary_len) {

   int i;
   for (i = 0; i < binary_len; i++) {
      if (number[i] == '1') {
         number[i] = '0';
      } else {
         number[i] = '1';
      }
   }
}

void increment_processor(char *number, int binary_len) {

   int i;
   for (i = binary_len - 1; i >= 0; i--) {
      if (number[i] == '0') {
         number[i] = '1';
         break;
      } else {
         number[i] = '0';
      }
   }
}

void add_processor(char *number_one, char *number_two, char *total, int binary_len){

   int carry = 0;

   int i;
   for (i = binary_len; i >= 0; i--) {

      int digit_one = number_one[i] - '0';
      int digit_two = number_two[i] - '0';
      int tmp_total = carry + digit_one + digit_two;

      if (tmp_total == 0 ) {
         carry = 0;
      } else if (tmp_total == 1) {
         number_one[i] = '1';
         carry = 0;
      } else if (tmp_total == 2) {
         number_one[i] = '0';
         carry = 1;
      } else if (tmp_total == 3) {
         number_one[i] = '1';
         carry = 1;
      }
   }
}

void pause_handler (int sigNum) {
    // printf (" received an interrupt.\n");
   if (initial_pause == 1) {
      fprintf (stdout, " Starting Processsing...\n");
      initial_pause = 0;
   }
   // else {
   //    fprintf(stdout, "Terminating...\n");
   //    exit(0);
   // }
}

void exit_handler (int sigNum) {
   printf(" received.  That's it, I'm shutting you down...\n");
   exit(0);
}
