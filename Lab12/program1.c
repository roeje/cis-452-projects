#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>

/**
 * CIS 452 - Lab 12: ls - Directory Listing Implementation in C
 * Implementation of a ls command.
 *
 * Usage: ./a.out <command> <pathname>
 * Valid commands: -l, -i
 *
 * Both parameters are optional, if none are specified, default dir '.' is used
 *
 * @author  Jesse Roe
 * @author Michael Kolarik
 * @version 04/06/2017
 */
int main(int argc, char *argv[]) {

   /* Various variables for use later */
   DIR *dirPtr;
   struct dirent *entryPtr;
   struct stat statBuf;
   int file_count = 0;
   char *pathname;
   char *commands;
   char *command;

   /* Check for valid number of parameters */
   if (argc > 3) {
     printf("Too many parameters\n");
     exit(1);
  } else if (argc == 3) {

      /* Store command line arguments */
      pathname = argv[argc - 1];
      commands = argv[argc - 2];
      command = &commands[1];
      // printf("Commands: %s\n", commands);
      // printf("Command %s\n", command);
      // printf("Path: %s\n", pathname);
   } else if (argc == 1) {

      /* Store command line arguments for default with no commands or path */
      pathname = ".";
      command = "NONE";
   }

   // Open directory streams for provided path
   dirPtr = opendir (pathname);

   /* Check for vaild path */
   if (dirPtr == NULL) {
      printf("ERROR: directory not found or permission denied...\n");
      exit(1);
   }

   /* Iterate over files in directory */
   while ((entryPtr = readdir (dirPtr))) {

      /* Concat each file name with the path to the file */
      char full_path[255];
      sprintf(full_path, "%s/%s", pathname, entryPtr->d_name);

      /* Populate stat struct with file data */
      if (stat (full_path, &statBuf) < 0) {
        perror ("huh?  there is ");
        exit(1);
      }

      /* Check for 'l' command */
      if (strcmp(command, "l") == 0) {
         // char * file_type = (int)((statBuf.st_mode)&77000) + '0';
         printf ("%-8.3o %-8d %-8d %-8s %-8s %-8s\n", (statBuf.st_mode)&0777,
                                                         (int)statBuf.st_size,
                                                         (int)statBuf.st_nlink,
                                                         getpwuid(statBuf.st_uid)->pw_name,
                                                         getgrgid(statBuf.st_gid)->gr_name,
                                                         entryPtr->d_name);
         file_count++;
      }

      /* Check for 'i' command */
      else if (strcmp(command, "i") == 0) {
         printf ("%lu %-10s ", statBuf.st_ino, entryPtr->d_name);
         if ((file_count%3) == 0 && file_count != 0) {
            printf("\n");
         }
         file_count++;
      } else {
         printf ("%s    ls", entryPtr->d_name);
         if ((file_count%8) == 0 && file_count != 0) {
            printf("\n");
         }
         file_count++;
      }
   }

   printf("\nTotal: %d\n", file_count);

   /* Close directory stream */
   closedir (dirPtr);
   return 0;
}
