#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

int	main(int ac, char **av) {
	t_tr_rts	rts;
	int			opt;

	if (ac == 1) {
		print_help();
		return 0;
	}
	ft_memset(&rts, 0, sizeof(t_tr_rts));
	opt = parse_options(&rts, ac, av);
	if (ac - opt == 2) {
		rts.origin_dest = av[ac - 1];
		rts.packetlen = DFL_PACKLEN;
	} else if (ac - opt == 1) {
		rts.origin_dest = av[ac - 2];
		rts.packetlen = ft_atoi(av[ac - 1]);
	} else {
		print_help();
		return 0;
	}
	printf("opt count: %d\n", opt);
	
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
