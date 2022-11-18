#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *buf = NULL;
	size_t bytesToRead = 0;
	ssize_t count;
	
	do {
		printf("$ ");
		count = getline(&buf, &bytesToRead, stdin);
		printf("%s", buf);
	} while (count != -1);

	free(buf);

	return (0);
}
