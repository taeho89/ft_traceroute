#include "../includes/ft_traceroute.h"
#include <netinet/ip_icmp.h>
#include <sys/time.h>

void	recv_packet(t_tr_rts *rts);
void	parse_response(char *buf, int buf_size);

int	main(int ac, char **av) {
	t_tr_rts	rts;

	ft_memset(&rts, 0, sizeof(t_tr_rts));
	parse_options(&rts, ac, av);
	
	rts.origin_dest = av[1];
	// init(&rts);

	rts.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rts.sockfd == -1) {
		perror("error in creating socket");
		return 1;
	}

	// TODO: main_loop()
	char packet[64];
	for (int ttl = 1; ttl <= DFL_TTL; ttl++) {
		printf("%d: ", ttl);
		for (int j = 0; j < 3; j++) {
			ft_memset(packet, 0, sizeof(packet));
			init_icmp_packet(&rts, packet, sizeof(packet));
			
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
			sendto(rts.sockfd, packet, sizeof(packet), 0,
				(struct sockaddr *)&rts.dest_addr, socklen);
			rts.seq++;
			gettimeofday(&rts.send_time, NULL);
			// recv_packet();
			recv_packet(&rts);
		}
		printf("\n");
	}
	
	// TODO: clear()
}

void	recv_packet(t_tr_rts *rts) {
	fd_set	r;
	char	recv_packet[256];

	FD_ZERO(&r);
	FD_SET(rts->sockfd, &r);

	struct timeval	timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec= 0;
	struct timeval	recv_time;
	int	count = select(4, &r, NULL, NULL, &timeout);
	if (count > 0) {
		gettimeofday(&recv_time, NULL);
		double	rtt = (recv_time.tv_sec - rts->send_time.tv_sec) * 1000.0;
		rtt += (recv_time.tv_usec - rts->send_time.tv_usec) / 1000.0;
		int	sender = recvfrom(
			rts->sockfd,
			recv_packet,
			sizeof(recv_packet),
			0,
			NULL,
			NULL);
		// print apcket result;
		printf("%.2f ", rtt);
		parse_response(recv_packet, sizeof(recv_packet));
		// print_response(&recv_packet, sizeof(recv_packet));
	}
}

void	parse_response(char *buf, int buf_size) {
	struct iphdr	ip;
	struct icmphdr	icmp;

	ft_memcpy(&ip, buf, sizeof(ip));
	ft_memcpy(&icmp, buf + ip.ihl * 4, sizeof(icmp));
	if (icmp.type == ICMP_ECHOREPLY) {
		printf("Echo Reply accepted | ");
	} else if (icmp.type == ICMP_TIME_EXCEEDED) {
		printf("Echo Time Exceeded accepted | ");
	} else {
		printf("ICMP type: %d | ", icmp.type);
	}
}

