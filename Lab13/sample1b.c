#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define SIZE 30

int main(int argc, char *argv[])
{
   struct flock fileLock;
   int fd;
   char buf[SIZE];

   if (argc < 2) {
      printf ("usage: filename\n");
      exit (1);
   }
   if ((fd = open (argv[1], O_RDWR)) < 0) {
      perror ("there is");
      exit (1);
   }

   fileLock.l_type = F_WRLCK;
   fileLock.l_whence = SEEK_SET;
   fileLock.l_start = 0;
   fileLock.l_len = 0;

   if (fcntl (fd, F_SETLKW, &fileLock) < 0) {
      perror("cant do it");
      exit(1);
   }

   if (read(fd, buf, SIZE-2)) {
      printf("%s\n", buf);
   }

   close(fd);

   return 0;
}
