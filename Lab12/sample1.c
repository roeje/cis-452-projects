#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[])
{
   struct stat statBuf;

   if (argc < 2) {
      printf ("Usage: filename required\n");
      exit(1);
   }

   if (stat (argv[1], &statBuf) < 0) {
      perror ("huh?  there is ");
      exit(1);
   }

   printf ("Is File a Directory: %d\n", S_ISDIR(statBuf.st_mode));
   return 0;
}
