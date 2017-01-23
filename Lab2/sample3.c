#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
  pid_t pid, child;
  int status;
  if ((pid = fork()) < 0) {
    perror("fork failure");
    exit(1);
  }
    else if (pid == 0) {
        printf("I am child PID %ld\n", (long) getpid());
        exit(0);
    }
    else {
        child = waitpid(pid, &status, 0);
        /* insert an appropriate form of the wait() system call here */
        printf("Child PID %ld terminated with return status %d\n", (long) child, status);
    }
    return 0;
}
