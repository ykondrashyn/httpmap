#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "nmap.h"

int main(int argc, char **argv) {

	char *filter_rej = read_filter("filter_reject.txt");
	char *filter_only = read_filter("filter_only.txt");

	char *res = read_ip_pull("../../scan.txt");
	if (res) {
		go_parse_string(res, filter_only, filter_rej);
	} else {
		printf("No scan diapasone!\n");
	}
	return 0;
}
