#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	unsigned int index = 0;
	char *str, *dup, *delim = ": ";

	dup = strdup(getenv("PATH"));

	str = strtok(dup, delim);
	dup = NULL;

	while (str)
	{
		printf("%s\n", str);

		str = strtok(dup, delim);
	}

	return (0);
}
