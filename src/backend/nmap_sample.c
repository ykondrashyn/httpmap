#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nmap.h"

int main(void) {
	char *res = read_ip_pull("../../scan.txt");
	go_parse_string(res);
	return 0;
}
