
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
 * Implementation of a basic multi process binary subtractor.
 * Binary strings are read from input files and individually processed by
 * unique processes. Pipes are utilized to communicate between processes.
 *
 * @author  Jesse Roe
 * @version 02/10/2017
 */

#define READ 0
#define WRITE 1
#define MAX 1024

void pause_handler(int sigNum);
void exit_handler(int sigNum);

void complement_processor(char *number, int binary_len);
void increment_processor(char *number, int binary_len);
void add_processor(char *number_one, char *number_two, int binary_len);

int main(int argc, char **argv) {

   // Setup pause signal handler for ctrl^c
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

   // Specified length of binary strings
   int binary_length = atoi(argv[3]);

   // char fileA[] = "8-input_A.dat";
   char *fileA = argv[1];
   // char fileB[] = "8-input_B.dat";
   char *fileB = argv[2];

   // Log command line inputs
   fprintf(stdout, "FileA is: %s\n", fileA);
   fprintf(stdout, "FileB is: %s\n", fileB);
   fprintf(stderr, "Binary number length: %d\n", binary_length);

   /**
    * Store status of wait()
    */
   int status;

   // Create pipes and check for failure
   if (pipe (pipe_one) < 0 || pipe (pipe_two) < 0) {
      perror ("Pipe Creation Failed.");
      exit(1);
   }

   // Create two child processes to handle processing
   int i;
   for (i = 0; i < 2; i++) {
      if ((pid = fork()) < 0) {
         printf("An error occured spawning a child process. Exiting.\n");
         exit(-1);

      } else if (pid == 0) {
         printf("Spawned child PID# %d\n", getpid());
         printf("Interator = %d\n", i);

         // If we are in the first child process
         if (i == 0) {

            // Spawn Increment Process

            char pipe_buffer[binary_length + 1];

            // Close unneeded pipe discriptors
            close(pipe_one[WRITE]);
            close(pipe_two[READ]);

            while(1) {

               // Read from pipe_one untill pipe is closed
               int success = read(pipe_one[READ], pipe_buffer, sizeof(pipe_buffer) + 1);

               // Check for closure of pipe_one
               if (success == 0) {
                  break;
               }

               fprintf(stdout, "Incrementer: Read binary string from PIPE: %s\t", pipe_buffer);

               // Call increment logic
               increment_processor(pipe_buffer, binary_length);

               fprintf(stdout, "Incrementer: Wrote binary string to PIPE: %s\n", pipe_buffer);

               // Write incremented binary number to pipe_two
               write(pipe_two[WRITE], pipe_buffer, sizeof(pipe_buffer) + 1);
            }

            // After all data is read, close pipe to signal that process is finished writing to pipe_one
            close(pipe_one[READ]);
            close(pipe_two[WRITE]);

            return 0;

         // If we are in the second child process
         } else {

            // Spawn Adder Process

            // Define file pointers for FileA and Output file.
            FILE *input_file, *output_file;

            // Create buffers for pipe_two read, and fileA read
            char pipe_buffer[binary_length + 1];
            char input_file_buffer[binary_length + 1];

            // Close unnecessary pipe file descriptors
            close(pipe_one[WRITE]);
            close(pipe_one[READ]);
            close(pipe_two[WRITE]);

            // Open files for reading/writing
            input_file = fopen(fileA, "r");
            output_file = fopen("output.dat", "w");

            while(1) {

               // Read each line from fileA until there is no more data
               if (fgets(input_file_buffer, binary_length + 3, input_file) == NULL) {
                  break;
               }

               // Read from pipe_two
               read(pipe_two[READ], pipe_buffer, sizeof(pipe_buffer) + 1);

               // Clean input of newlines and CR
               input_file_buffer[strcspn(input_file_buffer, "\r\n")] = 0;

               // Log binary strings that have been read in
               fprintf(stdout, "Adder: Read binary string from Pipe: %s\t", pipe_buffer);
               fprintf(stdout, "Adder: Read binary string from FileB: %s\t", input_file_buffer);

               // Call adder logic
               add_processor(pipe_buffer, input_file_buffer, binary_length);

               fprintf(stdout, "Adder: Write binary string to Output: %s\n", pipe_buffer);

               // Log results of addition to output file
               fprintf(output_file, "%s\n", pipe_buffer);
            }

            // Close pipe after pocess is finished
            close(pipe_two[READ]);

            // Close file pointers
            fclose(input_file);
            fclose(output_file);

            return 0;
         }

         return 0;
      }
   }

   // Parent Process - Complementer

   // Define file pointers for fileB
   FILE *input_file;

   // Create buffer for reading from fileB
   char file_buffer[binary_length + 1];

   // Close unnecessary file descriptors for pipe_one and pipe_two
   close(pipe_one[READ]);
   close(pipe_two[READ]);
   close(pipe_two[WRITE]);

   // Open fileB for reading
   input_file = fopen(fileB, "r");

   fprintf(stdout, "Waiting for Ctrl^C confirmation...\n");

   // Pause main process and wait for ctrl^c signal to be handled
   pause();

   // Process each line of file
   while(fgets(file_buffer, binary_length + 3, input_file) != NULL) {

      // Remove newline and any carrage returns (windows)
      file_buffer[strcspn(file_buffer, "\r\n")] = 0;

      fprintf(stdout, "Complementer: Read binary string from FileB: %s     ", file_buffer);

      // Call complement logic
      complement_processor(file_buffer, binary_length);

      fprintf(stdout, "Complementer: Write binary string to PIPE: %s\n", file_buffer);

      // Write processed binary string to pipe_one
      write(pipe_one[WRITE], file_buffer, sizeof(file_buffer) + 1);
   }

   // Close pipe to signal that process is complete
   close(pipe_one[WRITE]);

   // Close FILE pointer
   fclose(input_file);

   // Establish exit_handler for ctrl^c
   signal (SIGINT, exit_handler);

   // wait for child processed to terminate before exiting main process
   waitpid(-1, &status, 0);
   return 0;
}

/**
 * Implements the logic required to invert the bits of a specified binary number
 * string.
 * <p>
 *
 * @param  number - binary string to be processed
 * @param  binary_len - length of binary string
 * @return none
 */
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

/**
 * Implements the logic required to increment a binary string by 1
 * <p>
 *
 * @param  number binary string to be processed
 * @param  binary_len - length of binary string
 * @return none
 */
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

/**
 * Implements the logic required to add two binary strings
 * <p>
 *
 * @param  number_one binary string to be processed
 * @param  number_two binary string to be processed
 * @param  binary_len - length of binary string
 * @return none
 */
void add_processor(char *number_one, char *number_two, int binary_len){

   int carry = 0;

   int i;
   for (i = binary_len; i >= 0; i--) {

      // Convert each binary character to an int
      int digit_one = number_one[i] - '0';
      int digit_two = number_two[i] - '0';

      // Create runningn sum of digits
      int tmp_total = carry + digit_one + digit_two;

      // Based on the sum, the result is saved and the carry is saved
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

/**
 * Signal handler to break out of pause()
 * <p>
 *
 * @param  int sigNum - signal value
 * @return none
 */
void pause_handler (int sigNum) {
   fprintf (stdout, " Starting Processsing...\n");
}

/**
 * Signal handler to handle basic ctrl^c
 * <p>
 *
 * @param  int sigNum - signal value
 * @return none
 */
void exit_handler (int sigNum) {
   printf(" received.  Shutting down...\n");
   exit(0);
}
