#ifndef _NMAP_H_
#define _NMAP_H_

char	*read_ip_pull					(const char *file);
void	release_ip_pull					(char *pull);
int		nmap_start						(const char *ipaddr, const char *port_list);
int		nmap_get_ip_and_port			(char *buf);
int		nmap_web_server_check			(char *ip, char *port_str);
int		go_parse_string					(char *pFile);

#endif /* _NMAP_H_ */
