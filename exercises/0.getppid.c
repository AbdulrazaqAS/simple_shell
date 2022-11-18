#include <stdio.h>
#include <unistd.h>

int main(void)
{
	printf("PID is %u and PPID is %u\n", getpid(), getppid());

	return (0);
}
