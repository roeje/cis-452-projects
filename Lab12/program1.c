#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char *argv[]) {
   DIR *dirPtr;
   struct dirent *entryPtr;
   struct stat statBuf;
   int i = 0;
   int file_count = 0;
   char *pathname;
   char *commands;
   char *command;

   if (argc == 0) {
     printf("Path name required\n");
     exit(1);
   } else {
      pathname = argv[argc - 1];
      commands = argv[argc - 2];
      command = &commands[1];
      // printf("Commands: %s\n", commands);
      // printf("Command %s\n", command);
      // printf("Path: %s\n", pathname);
   }
   dirPtr = opendir (pathname);

   while ((entryPtr = readdir (dirPtr))) {
      char full_path[255];
      sprintf(full_path, "%s/%s", pathname, entryPtr->d_name);

      if (stat (full_path, &statBuf) < 0) {
        perror ("huh?  there is ");
        exit(1);
      }
      if (strcmp(command, "l") == 0) {
         // char * file_type = (int)((statBuf.st_mode)&77000) + '0';
         printf ("%-8.3o %-8d %-8d %-8s %-8s %-8s\n", (statBuf.st_mode)&0777, (int)statBuf.st_size, (int)statBuf.st_nlink, getpwuid(statBuf.st_uid)->pw_name, getgrgid(statBuf.st_gid)->gr_name, entryPtr->d_name);
      }
      if (strcmp(command, "i") == 0) {
         printf ("%d %-20s\n",(int)statBuf.st_ino, entryPtr->d_name);
      }

   }
   closedir (dirPtr);
   return 0;
}
