#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
   DIR *dirPtr;
   struct dirent *entryPtr;
   struct stat statBuf;

   dirPtr = opendir (".");

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
