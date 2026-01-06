#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

int	main(int ac, char **av) {
	t_tr_rts	rts;
	t_arg		arg;
	int			host_pos;
	int			packetlen_pos;
	char 		source[256];

	ft_memset(&rts, 0, sizeof(t_tr_rts));
	arg = parse_options(&rts, ac, av);
	if (!arg.host_pos) {
		print_help();
		return 0;
	}

	if (init(&rts, av, arg) < 0) {
		return 2;
	}

	struct sockaddr_in	sa = rts.dest_addr;
	if (!inet_ntop(AF_INET, &sa.sin_addr, source, INET_ADDRSTRLEN)) {
		print_error(NULL, errno, NULL);
		close(rts.recv_sockfd);
		free(rts.inflight);
		return 1;
	}

	printf("traceroute to %s (%s), %d hops max, %d byte packets\n", \
		av[1], source, rts.max_ttl, rts.packetlen);
	main_loop(&rts);

	close(rts.recv_sockfd);
	free(rts.inflight);
	free(rts.packet);
	return 0;
}
