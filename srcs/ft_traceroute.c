#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

int	main(int ac, char **av) {
	t_tr_rts	rts;

	ft_memset(&rts, 0, sizeof(t_tr_rts));
	rts.origin_dest = av[1];
	rts.packetlen = ft_atoi(av[2]);
	parse_options(&rts, ac, av);
	
	init(&rts);

	char source[256];
	struct sockaddr_in	sa = rts.dest_addr;
	if (!inet_ntop(AF_INET, &sa.sin_addr, source, INET_ADDRSTRLEN)) {
		perror("inet_ntop");
		return 1;
	}

	printf("traceroute to %s (%s), %d hops max, %d byte packets\n", \
		av[1], source, rts.max_ttl, 60);
	main_loop(&rts);

	close(rts.recv_sockfd);
	free(rts.inflight);
	return 0;
}

