#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(char *);

char *av[] = {NULL, NULL};
char *env[] = {"user=abdulrazaqAS", "logname=abdulrazaqAS", NULL};
pid_t child_pid;
int wstatus;

int main(int argc, char **argv)
{
	size_t len = 0;
	ssize_t count;
	char *buf = NULL;
	
	do {
		printf("#abdulrazaqAS$ ");
		count = getline(&buf, &len, stdin);

		buf[count - 1] = '\0';
		if (count != -1)
			execute(buf);

	} while (count != -1);

	putchar('\n');

	free(buf);

	return (0);
}

void execute(char *buf)
{
	child_pid = fork();
	if (child_pid == -1)
       	{
               	perror("Error(fork):");
               	exit(EXIT_FAILURE);
       	}
       	if (child_pid == 0)
       	{
		av[0] = buf;
               	execve(av[0], av, NULL);
               	perror("Error(execve):");
               	exit(EXIT_FAILURE);
       	}
       	else
       	{
		wait(&wstatus);
       	}

	return;
}
