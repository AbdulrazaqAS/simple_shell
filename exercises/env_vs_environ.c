#include <stdio.h>

int main(int ac, char *av[], char **env)
{
	extern char **environ;

	printf("Address of env: %p\n", &env);
	printf("Address of environ: %p\n", &environ);

	return (0);
}
