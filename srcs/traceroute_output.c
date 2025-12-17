#include "../includes/ft_traceroute.h"
#include <netinet/ip_icmp.h>

void	print_error(int errnum, char *errmsg) {
	fprintf(stderr, "ft_traceroute: ");

	if (errnum)
		perror("error");
		// fprintf(stderr, "%s", strerror(errnum));
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

void	print_recv_result(struct iphdr *ip, struct icmphdr *icmp, double rtt) {
	char	source[256];
	char	source_name[256];
	ft_memset(source_name, 0, sizeof(source_name));
	inet_ntop(AF_INET, &ip->saddr, source, INET_ADDRSTRLEN);

	struct sockaddr_in	sa;
	sa.sin_addr.s_addr = ip->saddr;
	sa.sin_family = AF_INET;
	getnameinfo((struct sockaddr *)&sa, sizeof(sa), source_name, sizeof(source_name), NULL, 0, 0);
	if (*source_name)
		printf("%s (%s) ", source_name, source);
	else
		printf("%s ", source);
	printf("%.3fms ", rtt);
	fflush(stdout);
}
