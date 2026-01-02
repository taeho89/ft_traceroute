#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
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

int	print_log(t_slot *slots, int probe_per_hop, int next_to_print) {
	int	print_count = 0;

	for (int i = next_to_print; i < MAX_INFLIGHT + next_to_print; i++) {
	// while (!(slots + next_to_print)->is_active) {
		// printf("is active: %d\n", (slots + next_to_print)->is_active);
		if (slots[i].is_active) break ;
		if (!(slots[i].seq % probe_per_hop)) {
			printf("%2d ", slots[i].ttl);

			if (!slots[i].is_timeout) {
				char	host[256];
				char	ip[256];

				inet_ntop(AF_INET, &slots[i].ip_address, ip, INET_ADDRSTRLEN);
				getnameinfo((struct sockaddr *)&slots[i].ip_address, sizeof(slots[i].ip_address), host, sizeof(host), NULL, 0, 0);

				// 주소 출력
				printf("%s (%s) ", host, ip);
			}
		}
		if (!slots[i].is_timeout) {
			float	rtt;

			rtt = (slots[i].recv_time.tv_sec - slots[i].sent_time.tv_sec) * 1000.0;
			rtt += (slots[i].recv_time.tv_usec - slots[i].sent_time.tv_usec) / 1000.0;
			printf(" %.3f ms ", rtt);
			print_count++;
		} else {
			printf(" *");
			print_count++;
		}
		if (slots[i].seq % probe_per_hop == probe_per_hop - 1) {
			printf("\n");
		}
	}
	// printf("printC: %d\n", print_count);
	return print_count;
}
