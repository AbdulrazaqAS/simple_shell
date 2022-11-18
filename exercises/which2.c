#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main(int ac, char **av)
{
	unsigned int i;
	struct stat st;

	if (ac < 2)
	{
		dprintf(STDERR_FILENO, "Usage: %s path_to_file ...\n", av[0]);
		exit(EXIT_FAILURE);
	}

	i = 1;
	while (av[i])
	{
		printf("%s: ", av[i]);
		if (stat(av[i], &st) == 0)
		{
			printf("FOUND\n");
		}
		else
		{
			printf("NOT FOUND\n");
		}
		i++;
	}
	exit(EXIT_SUCCESS);
}
