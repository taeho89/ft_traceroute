#include "../includes/ft_traceroute.h"

int	main(int ac, char **av) {
	t_tr_rts	rts;

	parse_options(&rts, ac, av);
	
	rts.origin_dest = av[1];
	// init(&rts);

	rts.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rts.sockfd == -1) {
		perror("error in creating socket");
		return 1;
	}

	// TODO: main_loop()
	for (int ttl = 0; ttl < DFL_TTL; ttl++) {
		for (int j = 0; j < 3; j++) {
			// init_icmp_packet();
			char buf[4096];
			
			// set socket option;
			int res = setsockopt(
				rts.sockfd,
				IPPROTO_IP,
				IP_TTL,
				&ttl,
				sizeof(ttl)
			);
			
			// send_packet();
			int	socklen = sizeof(rts.dest_addr);
			sendto(rts.sockfd, buf, sizeof(buf), 0,
				(struct sockaddr *)&rts.dest_addr, socklen);
			
			// recv_packet();
		}
	}
	
	// TODO: clear()
}
