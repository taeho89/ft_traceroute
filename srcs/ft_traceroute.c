#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

int	main(int ac, char **av) {
	t_tr_rts	rts;

	ft_memset(&rts, 0, sizeof(t_tr_rts));
	rts.origin_dest = av[1];
	parse_options(&rts, ac, av);
	
	init(&rts);

	rts.sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (rts.sockfd == -1) {
		perror("error in creating socket");
		return 1;
	}
	rts.recv_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rts.recv_sockfd == -1) {
		perror("error in creating socket");
		return 1;
	}
	struct sockaddr_in	local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(55000);
	if (bind(rts.sockfd, (struct sockaddr *)&local, sizeof(local)) < 0) {
		perror("bind");
		return 1;
	}

	char source[256];
	struct sockaddr_in	sa = rts.dest_addr;
	if (!inet_ntop(AF_INET, &sa.sin_addr, source, INET_ADDRSTRLEN)) {
		perror("inet_ntop");
		return 1;
	}
	printf("dest ip: %s\n", source);

	main_loop(&rts);

	// TODO: clear()
}

