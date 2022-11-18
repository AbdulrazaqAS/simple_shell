#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

char *_getenv(const char *);

int main(int ac, char *av[])
{
	char *var;

	if (ac != 2)
	{
		fprintf(stderr, "Usage: %s env-var-name\n", av[0]);
		return (EXIT_FAILURE);
	}

	var = _getenv(av[1]);

	printf("%s\n", var);

	return (EXIT_SUCCESS);

}

char *_getenv(const char *name)
{
	extern char **environ;
	unsigned int index = 0;
	char *res;

	while (environ[index])
	{
		res = strcasestr(environ[index], name);

		if (res != NULL)
		{
			res = strchr(environ[index], '=');

			return(++res);
		}
		index++;
	}

	return (NULL);
}
