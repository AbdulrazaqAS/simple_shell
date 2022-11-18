#include <stdio.h>

int main(int ac, char **av)
{
	int a = 0;
	while(av[a])
	{
		printf("%s\n", av[a]);
		a++;
	}
}
