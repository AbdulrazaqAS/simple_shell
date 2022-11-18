#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct path_h {
	char *value;
	struct path_h *next;
} path_t;

path_t *create_list(char *dirs)
{
	path_t *head, *tmp, *tmp2;
	unsigned int index = 0;
	char *str, *dup = strdup(dirs), *delim = "=: ";

	head = malloc(sizeof(path_t));
	if (head == NULL)
		return (NULL);

	str = strtok(dup, delim);
	head->value = str;
	tmp = head;
	dup = NULL;

	while (str)
	{
		str = strtok(dup, delim);

		tmp2 = malloc(sizeof(path_t));
		if (tmp2 == NULL)
			return (NULL);
		tmp2->value = str;
		tmp->next = tmp2;
		tmp = tmp2;
	}

	tmp->next = NULL;

	free(tmp);

	return (head);
}

int main(void)
{
	path_t *li;

	li = create_list(getenv("PATH"));

	while (li)
	{
		printf("%s\n", li->value);
		li = (*li).next;
	}

	return (0);
}
