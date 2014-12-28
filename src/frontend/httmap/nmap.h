#ifndef _NMAP_H_
#define _NMAP_H_

extern "C" char	*read_ip_pull					(const char *file);
extern "C" void	release_ip_pull					(char *pull);
extern "C" int		nmap_start						(const char *ipaddr, const char *port_list);
extern "C" int		nmap_get_ip_and_port			(char *buf);
extern "C" int		nmap_web_server_check			(char *ip, char *port_str);
extern "C" int		go_parse_string					(char *pFile);

#endif /* _NMAP_H_ */
