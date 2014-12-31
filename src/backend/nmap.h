#ifndef _NMAP_H_
#define _NMAP_H_

char	*read_ip_pull					(const char *file);
void	release_ip_pull					(char *pull);

char	*read_filter					(const char *file);
void	release_filter					(char *filter);

int		nmap_start						(const char *ipaddr, const char *port_list, char *filter_only, char *filter_rej);
int		nmap_get_ip_and_port			(char *buf, char *filter_only, char *filter_rej);
int		nmap_web_server_check			(char *ip, char *port_str, char *filter_only, char *filter_rej);
int		go_parse_string					(char *pFile, char *filter_only, char *filter_rej);

#endif /* _NMAP_H_ */
