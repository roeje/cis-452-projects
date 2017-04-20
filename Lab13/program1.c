#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

   /* Various variables for use later */
   DIR *dirPtr;
   char *path_file;
   char *path_link;
   char *command;

   /* Check for valid number of parameters */
   // printf("%d\n", argc);
   if (argc == 1 || argc == 2) {
      printf("Not enough paraeters were provided...\n");
      exit(1);
   }
   if (argc > 4) {
     printf("Too many parameters\n");
     exit(1);
   } if (argc == 4) {

      /* Store command line arguments */
      path_file = argv[argc - 2];
      path_link = argv[argc - 1];
      command = argv[1];
      // printf("Commands: %s\n", commands);
      // printf("Command %s\n", command);
      // printf("Path: %s\n", pathname);
   } else {
      path_file = argv[argc - 2];
      path_link = argv[argc - 1];
      command = "None";
   }

   // dirPtr = opendir (path_file);

   /* Check for vaild path */
   // if (dirPtr == NULL) {
   //    printf("ERROR: directory not found or permission denied...\n");
   //    exit(1);
   // }

   if (strcmp(command, "-s") == 0) {
      if (symlink(path_file, path_link) < 0) {
         printf("ERROR: soft link creation failed...\n");
         exit(1);
      }

   } else if (strcmp(command, "-h") == 0) {
      if (link(path_file, path_link) < 0) {
         printf("ERROR: hard link creation failed...\n");
         exit(1);
      }
   } else if (strcmp(command, "None") == 0) {
      if (link(path_file, path_link) < 0) {
         printf("ERROR: hard link creation failed...\n");
         exit(1);
      }
   } else {
      printf("Invalid command was provided.\n");
   }

   printf("Path to File: %s\n", path_file);
   printf("Path for Link: %s\n", path_link);
   printf("Command: %s\n", command);



   return 0;
}
