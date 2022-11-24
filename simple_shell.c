#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shell.h"

int search_path(char *cmd);
void execute(char *buf);

char *av[100];
pid_t child_pid;
int wstatus;
char *arg1;

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

	arg1 = argv[0];
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

		}
	} while (count != -1);

	free(buf);

	return (0);
}

/**
 * execute - exec a given command
 * @buf: buffer (from main getline func
 * so that the child can free its own)
 */
void execute(char *buf)
{
	if (search_path(av[0]) == -1)
		return;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror(arg1);
		exit(EXIT_FAILURE);
	}

	if (child_pid == 0)
	{
		free(buf);
		execve(av[0], av, environ);
		perror(arg1);
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&wstatus);
	}
}

/**
 * search_path - searches all path for cmd
 * @cmd: command
 *
 * Return: 0 on success, else -1
 */
int search_path(char *cmd)
{
	unsigned int i = 0;
	char *path, *tmp, *tmp_tmp2, *paths, *tmp2;
	struct dirent *file;
	DIR *dir;
	int cstatus;

	paths = strdup(getenv("PATH"));
	if (!paths)
	{
		free(paths);
		return (-1);
	}

	/*
	 * checking if the cmd starts with a '/'.
	 * First checking if it's like '/ls' which is
	 * incorrect or '/bin/ls' which is correct.
	 */
	if (cmd[0] == '/')
	{
		tmp2 = strdup(cmd);
		tmp_tmp2 = tmp2;
		for (i = 0; ; tmp2 = NULL, i++)
		{
			path = strtok(tmp2, "/");
			if (path == NULL)
			{
				/*
				 * If we have only one token
				 * i.e. i < 2 (bcoz loop 0 = token,
				 * and loop 1 = NULL), then the cmd
				 * is in '/ls' format which is incorect
				 * so we return -1.
				 */
				if (i < 2)
				{
					free(paths);
					free(tmp_tmp2);
					return (-1);
				}
				/*
				 * Means the cmd is in '/bin/ls' format
				 * NOTE: just using '/ls' and '/bin/ls'
				 * for example, it can be '/pwd'
				 * and '/usr/bin/ls'
				 */
				cmd = tmp;
				break;
			}
			else
				/*
				 * Saving path so that it can be used
				 * in the next looping (i.e i++) when
				 * path == NULL
				 */
				tmp = path;
		}
		free(tmp_tmp2);
	}

	tmp_tmp2 = paths;
	for (i = 0; ; paths = NULL, i++)
	{
		path = strtok(paths, ":\n");

		if (path == NULL)
			break;

		dir = opendir(path);
		if (dir == NULL)
		{
			perror(arg1);
			free(tmp_tmp2);
			return (-1);
		}

		do {
			file = readdir(dir);
			if (file != NULL && strcmp(file->d_name, cmd) == 0)
			{
				strcat(path, "/");
				strcat(path, cmd);
				free(av[0]);
				strcpy(av[0], path);

				cstatus = closedir(dir);
				if (cstatus == -1)
					perror(arg1);
				free(tmp_tmp2);

				return (0);
			}
		} while (file != NULL);

		cstatus = closedir(dir);
		if (cstatus == -1)
		{
			perror(arg1);
			break;
		}
	}
	free(tmp_tmp2);
	return (-1);
}
