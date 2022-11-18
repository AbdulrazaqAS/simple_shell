#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t child_pid;
	int count, wstatus;
	char *argv[] = {"/usr/bin/ls", "-l", "/tmp", NULL};

	for (count = 0; count < 5;count++)
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Error(fork):");
			exit(EXIT_FAILURE);
		}
		if (child_pid == 0)
		{
			break;
		}
		printf("\n\nChild %d is created\n", count + 1);
		printf("Parent id: %u, Child id: %u\n", getpid(), child_pid);
		wait(&wstatus);
	}

	if (child_pid == 0)
	{
		execve(argv[0], argv, NULL);
		perror("Error(execve):");
		fprintf(stderr, "Child execve error\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("\nParent is about to terminate\n");
	}

	exit(EXIT_SUCCESS);
}
