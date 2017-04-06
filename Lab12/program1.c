#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[]) {
   DIR *dirPtr;
   struct dirent *entryPtr;
   struct stat statBuf;
   int i = 0;
   char *pathname;
   char *commands;

   if (argc == 0) {
     printf("Path name required\n");
     exit(1);
   } else {
     pathname = argv[argc - 1];
     commands = argv[argc - 2];
   }
   dirPtr = opendir (*pathname);

   if (commands)

   while ((entryPtr = readdir (dirPtr))) {

      if (stat (entryPtr->d_name, &statBuf) < 0) {
        perror ("huh?  there is ");
        exit(1);
      }

      printf ("Filename: %-20s Size(bytes): %d\n", entryPtr->d_name, (int)statBuf.st_size);
    }
   closedir (dirPtr);
   return 0;
}
