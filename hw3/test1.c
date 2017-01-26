#include <stdio.h>
#include <unistd.h>
int main()
{
	int i, pid;
	for (i=0; i<4; i++)
		pid = fork();
		if (pid) {
			printf("I forked\n");
		}
	return 0;
}
