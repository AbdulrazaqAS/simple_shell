#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(char *);

char *av[] = {NULL, NULL};
__attribute__((unused))char *env[] = {"USER=abdulrazaq", "LOGNAME=abdulrazaq",
	"HOME=/simple_shell", NULL};
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
int main(int argc, char *argv[])
{
	size_t len = 0;
	ssize_t count;
	char *buf = NULL;

	file = argv[0];
	argc++;
	argc--;

	do {
		count = getline(&buf, &len, stdin);

		if (count != -1)
		{
			execute(strtok(buf, " \n"));
		}
	} while (count != -1);

	free(buf);

	return (0);
}

/**
 * execute - exec a given command
 * @buf: command to exec
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
		av[0] = buf;
		execve(av[0], av, environ);
		perror(file);
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&wstatus);
	}
}
