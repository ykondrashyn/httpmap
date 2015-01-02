#ifndef _NMAP_H_
#define _NMAP_H_

/******************************************************************************/
/*							FUNCTION DECLARATIONS							  */
/******************************************************************************/

char	*read_ip_pull					(const char *file);
void	release_ip_pull					(char *pull);

char	*read_filter					(const char *file);
void	release_filter					(char *filter);

int		nmap_start						(const char *ipaddr, const char *port_list, char *filter_only, char *filter_rej);
int		nmap_get_ip_and_port			(char *buf, char *filter_only, char *filter_rej);
int		nmap_web_server_check			(char *ip, char *port_str, char *filter_only, char *filter_rej);
int		go_parse_string					(char *pFile, char *filter_only, char *filter_rej);

/******************************************************************************/
/*							MACRO AND DEFINITIONS							  */
/******************************************************************************/

#define TRUE				1
#define FALSE				0
#define ERROR				1
#define SUCCESS				0
#define MAX_FILTER_LEN		255
#define MAX_DIAPASONE_LEN	20

#define DEBUG                 /* Comment/uncomment this to enable/disable debug output mode */

int CheckDebugLevel() {
#ifdef DEBUG
	return 1;
#else
	return 0;
#endif
}

#define DBG_PRINT(fmt, ...) if (CheckDebugLevel()) fprintf(stdout, "%s(): "fmt, __FUNCTION__, ##__VA_ARGS__)
#define ERR_PRINT(fmt, ...) fprintf(stderr, "%s(): "fmt, __FUNCTION__, ##__VA_ARGS__)

#endif /* _NMAP_H_ */
