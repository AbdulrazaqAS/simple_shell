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
char *expand_path(char *);
int search_cmd_in_path(char *, char *);
int search_cmd_in_all_paths(char *cmd, char *);
int split_cmd_and_path(char *str, char cmd[], char path[]);

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
			free(av[0]);

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
	int status;
	char cmd[100], path[500];

	if (split_cmd_and_path(av[0], cmd, path) == 0)
	{
		strcpy(cmd, av[0]);
		status = search_cmd_in_all_paths(cmd, path);	
	}
	else
	{
		if (path[0] == '.' && (path[1] == '.' || path[1] == '/'))
			expand_path(path);

		status = search_cmd_in_path(cmd, path);	
	}
	if (status != 1)
		return;

	strcat(path, cmd);
	child_pid = fork();
	if (child_pid == -1)
	{
		perror(arg1);
		exit(EXIT_FAILURE);
	}

	if (child_pid == 0)
	{
		free(buf);
		execve(path, av, environ);
		perror(arg1);
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&wstatus);
	}
}

/**
 * search_cmd_in_all_paths - searches for cmd in all paths
 * @cmd: command
 * @buf: buffer to store the path
 *
 * Return: -1 if not present, else 1
 */
int search_cmd_in_all_paths(char *cmd, char *buf)
{
	unsigned int i = 0;
	char *path, *tmp, *paths;
	struct dirent *file;
	DIR *dir;
	int cstatus;

	paths = strdup(getenv("PATH"));
	if (!paths)
	{
		free(paths);
		return (-1);
	}
	tmp = paths;
	for (i = 0; ; paths = NULL, i++)
	{
		path = strtok(paths, ":\n");

		if (path == NULL)
			break;
		dir = opendir(path);
		if (dir == NULL)
		{
			perror(arg1);
			free(tmp);
			return (-1);
		}

		do {
			file = readdir(dir);
			if (file != NULL && strcmp(file->d_name, cmd) == 0)
			{
				cstatus = closedir(dir);
				if (cstatus == -1)
					perror(arg1);
				free(tmp);
				strcat(path, "/");
				strcpy(buf, path);

				return (1);
			}
		} while (file != NULL);

		cstatus = closedir(dir);
		if (cstatus == -1)
		{
			perror(arg1);
			break;
		}
	}
	free(tmp);
	return (-1);
}


/**
 * split_cmd_and_path - splits str into cmd and path
 * @cmd: command
 * @path: path
 * @str: string
 *
 * Return: 0 if no need for splitting, 1 if splitted
 */
int split_cmd_and_path(char *str, char cmd[], char path[])
{
	unsigned int slen;
	int tmp = 0;

	slen = strlen(str);
	for (; slen > 0; slen--)
	{
		if (str[slen] == '/')
		{
			tmp = 1;
			/* To count out '/' */
			slen++;
			break;
		}
	}
	if (tmp == 0)
	{
		return(0);
	}
	strcpy(cmd, str + slen);
	strncpy(path, str, slen);
	path[slen] = '\0'; /* Setting the last '/' to '\0' */

	return (1);
}



/**
 * search_cmd_in_path - searches a cmd in path
 * @cmd: command
 * @path: path
 *
 * Return: 1 if present, else -1
 */
int search_cmd_in_path(char *cmd, char *path)
{
	struct dirent *file;
	DIR *dir;
	int cstatus;

	dir = opendir(path);
	if (dir == NULL)
	{
		perror(arg1);
		return (-1);
	}
	do {
		file = readdir(dir);
		if (file != NULL && strcmp(file->d_name, cmd) == 0)
		{
			cstatus = closedir(dir);
			if (cstatus == -1)
				perror(arg1);
			return (1);
		}
	} while (file != NULL);

	cstatus = closedir(dir);
	if (cstatus == -1)
		perror(arg1);
	return (-1);
}

/**
 * expand_path - expanding the '..' in a pathname
 * @path: path
 *
 * Return: expanded path
 *
 * Description: we're only considering '..' here bcoz '.'
 * is almost of no use in a path name
 */
char *expand_path(char *path)
{
	unsigned int i, goback = 0;
	char *tmp;

	if (path == NULL)
		return (NULL);
	for (i = 0; i < strlen(path); i++)
	{
		if (path[i] == '.')
		{
			if (path[i + 1] == '.' && (path[i + 2] == '/' || path[i + 2] == '\0'))
				goback++;
		}
	}
	if (goback == 0)
		return (path);
	if (getcwd(path, 500) == NULL)
	{
		perror(arg1);
		return(NULL);
	}	
	for (; goback > 0; goback--)
	{
		tmp = rindex(path, '/');
		if (tmp == NULL)
			break;
		i = 0;
		while (tmp[i])
		{
			tmp[i] = '\0';
			i++;
		}
	}
	strcat(path, "/");
	return (path);
}
