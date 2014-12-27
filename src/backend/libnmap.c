#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nmap.h"

#define ERROR		1
#define SUCCESS		0

int test_func(int val) {
	printf("Hello!\nThe val is: %d\n", val);
	return 0;
}

char *read_ip_pull(const char *file) {

	int size = 0;
	int offset = 0;
	char *result = NULL;
	FILE *fd = fopen(file, "r");
	if (fd) {
		/* getting size of file */
		fseek(fd, 0, SEEK_END);
		size = ftell(fd);
		fseek(fd, 0, SEEK_SET);

		/* allocating the resources */
		result = (char *)malloc(size);
		if (result) {

			while (fgets(result + offset, 20, fd)) {
				offset = strlen(result);
			}
		}

		return result;

	} else {
		printf("An error has been occurred while opening the '%s'\n", file);
		return NULL;
	}
}

int go_parse_string(char *pFile) {

	char *pch = NULL;
	char *string = NULL;

	pch = strtok (pFile,"\n");
	while (pch != NULL) {
		printf ("Checking ip -> %s <- with nmap..\n",pch);
		pch = strtok (NULL, "\n");
	}
	return SUCCESS;
}
