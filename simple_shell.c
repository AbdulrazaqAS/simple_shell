#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(void);

char *av[100];
pid_t child_pid;
int wstatus;
char *file;
extern char **environ;

/**
 * main - entry
 * @argc: arg count
 * @argv: arg vector
 *
 * Return: 0 or -1
 */
int main(__attribute__((unused))int argc, char *argv[])
{
	size_t len = 0;
	ssize_t count;
	char *buf = NULL;

	file = argv[0];
	do {
		count = getline(&buf, &len, stdin);

		if (count != -1)
		{
			av[0] = strtok(buf, " \n\t");
			av[1] = strtok(NULL, " \n\t");
			av[2] = strtok(NULL, " \n\t");
			av[3] = strtok(NULL, " \n\t");
			av[4] = strtok(NULL, " \n\t");

			execute();
		}
	} while (count != -1);

	free(buf);

	return (0);
}

/**
 * execute - exec a given command
 */
void execute(void)
{
	child_pid = fork();
	if (child_pid == -1)
	{
		perror(file);
		exit(EXIT_FAILURE);
	}

	if (child_pid == 0)
	{
		execve(av[0], av, environ);
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&wstatus);
	}
}
