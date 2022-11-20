#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(char *buf);

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
	unsigned int i;

	file = argv[0];
	do {
		count = getline(&buf, &len, stdin);

		if (count != -1)
		{
			for (i = 0; ; buf = NULL, i++)
			{
				av[i] = strtok(buf, " \n\t");

				if (av[i] == NULL)
					break;
			}


			if (av[0])
				execute(buf);

			free(av[0]);
		}
	} while (count != -1);

	free(buf);

	return (0);
}

/**
 * execute - exec a given command
 */
void execute(char *buf)
{
	child_pid = fork();
	if (child_pid == -1)
	{
		perror(file);
		exit(EXIT_FAILURE);
	}

	if (child_pid == 0)
	{
		free(buf);
		execve(av[0], av, environ);
		perror(file);
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&wstatus);
	}
}
