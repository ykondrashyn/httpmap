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
		printf("Checking ip -> %s <- with nmap..\n", pch);
		nmap_start(pch, "80,8080,8000");
		pch = strtok (NULL, "\n");
	}
	return SUCCESS;
}

int nmap_start(const char *ipaddr, const char *port_list) {

	char buf[1024]				= {0};
	char nmap_param_list[255]	= {0};

	FILE *fd = NULL;
	
	snprintf(nmap_param_list, sizeof(nmap_param_list), "nmap -p %s --open -oG - %s", port_list, ipaddr);
	fd = popen(nmap_param_list, "r");
	if (fd) {
		while (fgets(buf, sizeof(buf), fd)) {
			if (strstr(buf, "Ports")) {
				printf("%s", buf);
			}
		}
	} else {
		return ERROR;
	}

	return SUCCESS;
}

