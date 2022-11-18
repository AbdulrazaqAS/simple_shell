#include <stdio.h>

int main(void)
{
	extern char **environ;
	unsigned int index = 0;

	while (environ[index])
	{
		printf("%s\n", environ[index]);
		index++;
	}

	return (0);
}
