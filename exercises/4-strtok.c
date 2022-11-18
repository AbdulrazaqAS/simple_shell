#include <stdio.h>
#include <string.h>

char **strsplit(char *str, char *delimiters)
{
	int index;
	char *tokens[100];
	
	for (index = 0;;str = NULL, index++)
	{
		tokens[index] = strtok(str, delimiters);

		if (tokens[index] == NULL)
			break;
	}

	return (tokens);
}

int main(int argc, char *argv[])
{
	char **tmp = strsplit(argv[1], argv[2]);
	int index = 0;

	while (tmp[index])
	{
		printf("%s\n", tmp[index]);
		index++;
	}

	return (0);
}
