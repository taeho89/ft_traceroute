#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

void	print_error(char *arg, int errnum, const char *errmsg) {
	if (arg) 
		fprintf(stderr, "%s: ", arg);
	else
		fprintf(stderr, "ft_traceroute: ");

	if (errnum)
		fprintf(stderr, "%s", strerror(errnum));
	else
		fprintf(stderr, "%s", errmsg);

	fprintf(stderr, "\n");
}

void	print_help() {
	printf(
		"\nUsage:\n"
		"  traceroute host\n"
		"\nOptions:\n"
		"  --help        Read this help and exit\n"
		"\nArguments:\n"
		"+      host           The host to traceroute to\n"
		"       packetlen      The full packet length (default is the length of an IP\n"
		"                      header plus 40). Can be ignored or increased to a minimal\n"
		"                      allowed value\n\n");
}

/*
 *	pph : probe_per_hop
 *	ni	: next_index
 *	*/
int	print_log(t_slot *slots, int size, int pph, int ni, int dest_ttl) {
	char	buffer[512];
	char	host[256];
	char	ip[256];
	int		pos = 0;
	float	rtt;
	int		print_count = 0;

	ft_memset(buffer, 0, sizeof(buffer));
	for (int i = ni; i < MAX_INFLIGHT + ni && i < size; i++) {

		if (slots[i].is_active) break ;
		if (dest_ttl && slots[i].ttl > dest_ttl) break ;

		if (!(slots[i].seq % pph)) {
			pos += sprintf(buffer + pos, "%2d ", slots[i].ttl);

			if (!slots[i].is_timeout) {
				inet_ntop(AF_INET, &slots[i].ip_address.sin_addr, ip, INET_ADDRSTRLEN);
				getnameinfo((struct sockaddr *)&slots[i].ip_address, sizeof(slots[i].ip_address), host, sizeof(host), NULL, 0, 0);

				pos += sprintf(buffer + pos, "%s (%s) ", host, ip);
			}
		}

		if (!slots[i].is_timeout) {
			rtt = (slots[i].recv_time.tv_sec - slots[i].sent_time.tv_sec) * 1000.0;
			rtt += (slots[i].recv_time.tv_usec - slots[i].sent_time.tv_usec) / 1000.0;
			pos += sprintf(buffer + pos, " %.3f ms ", rtt);
		} else if (!slots[i].is_error) {
			pos += sprintf(buffer + pos, " !%c", slots[i].is_error);
		} else {
			pos += sprintf(buffer + pos, " *");
		}

		if (slots[i].seq % pph == pph - 1) {
			pos += sprintf(buffer + pos, "\n");
		}

		print_count++;
	}
	write(1, buffer, ft_strlen(buffer));
	return print_count;
}
