#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "nmap.h"

#define ERROR		1
#define SUCCESS		0

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

void release_ip_pull(char *pull) {
	free(pull);
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

	FILE *fd					= NULL;
	
	snprintf(nmap_param_list, sizeof(nmap_param_list), "nmap -p %s --open -oG - %s", port_list, ipaddr);
	fd = popen(nmap_param_list, "r");
	if (fd) {
		while (fgets(buf, sizeof(buf), fd)) {
			if (strstr(buf, "open/")) {
//				printf("buf: %s", buf);
				nmap_get_ip_and_port(buf);
			}
		}
	} else {
		return ERROR;
	}

	pclose(fd);

	return SUCCESS;
}

int nmap_get_ip_and_port(char *buf) {

	char ip_to_check[20]		= {0};
	char port_to_check[20]		= {0};
	if (strchr(buf, '(')) {
		/* obtaining the ip address */
		int ip_length = strchr(buf, '(') - (buf + 6) - 1;
		memcpy(ip_to_check, buf + 6, ip_length);
		ip_to_check[ip_length] = 0;

		/********************************************************************************/
		/* Obtaining the port															*/
		/* Please note, that we CAN have multiple port values for one IP address		*/
		/* Such we should check them all												*/
		/********************************************************************************/

		/* getting the first port value */
		char *pch			= NULL;
		char *_ptr			= strstr(buf, "Ports:");
		int port_length		= 0;

		_ptr += strlen("Ports: ");
		port_length = strchr(_ptr, '/') - _ptr;
		memcpy(port_to_check, _ptr, port_length);
		port_to_check[port_length] = 0;

//		printf("IP: %s, PORT: %s\n", ip_to_check, port_to_check);
		nmap_web_server_check(ip_to_check, port_to_check);	/* web server check for the first port */
//		nmap_web_server_check("ya.ru", "80");	/* web server check for the first port */

		/* Another port values are comma separated */
		while ((pch = strchr(_ptr, ',')) != NULL) {
			port_length = strchr(pch, '/') - (pch + 2);
			memcpy(port_to_check, pch + 2, port_length);
//			printf("\t\t\tPORT: %s\n", port_to_check);
//			nmap_web_server_check(ip_to_check, port_to_check);	/* web server check for the another ports */
			_ptr = pch + 1;
		}

	} else {
		printf("An error has been occured: invalid nmap version\n");
		return ERROR;
	}

	return SUCCESS;
}

int nmap_web_server_check(char *ip, char *port_str) {

	int sock			= 0;
	int port			= 0;
	int num_bytes		= 0;
	char buf[1024]		= {0};

	struct sockaddr_in serv_addr;

	port = (int)strtol(port_str, NULL, 10);

	printf("IP: %s, PORT: %d\n", ip, port);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return ERROR;
	}

	/* build the server's Internet address */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_port			= htons(port);
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr(ip);

	/* connect: create a connection with the server */
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Connect error:%s\n", strerror(errno));
		return ERROR;
	}

	snprintf(buf, sizeof(buf), "GET / HTTP/1.0\r\n\r\n");

	/* send the message line to the server */
	num_bytes = write(sock, buf, strlen(buf));
	if (num_bytes < 0) {
		return ERROR;
	}

	struct timeval tv;
	tv.tv_sec = 1;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&tv,sizeof(struct timeval));

	/* print the server's reply */
	buf[0] = 0;
	num_bytes = read(sock, buf, sizeof(buf));
	if (num_bytes < 0) {
		return ERROR;
	}

/* PLEASE PAY YOUR ATTENTION! */
/* The code below should be tested as well as we can */
	if (strstr(buf, "OK") || 
		strstr(buf, "Ok") ||
		strstr(buf, "ok") ||
		strstr(buf, "200")) {
		printf("===============> IP: %s\n", ip);
		}

	close(sock);
}
