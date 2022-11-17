#include "shell.h"

/**
 * main - entry point
 * @argc: arguments count
 * @argv: arguments vector
 *
 * Return: 0 always
 */
int main(int argc, char *argv[])
{
	while (argc)
		printf("%s\n", argv[argc--]);

	exit(0);
}
