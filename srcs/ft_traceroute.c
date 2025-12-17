#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

int		recv_packet(t_tr_rts *rts);
int	parse_response(char *buf, int buf_size, double rtt);

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
	// TODO: main_loop()
	char packet[64];
	for (int ttl = 1; ttl <= DFL_TTL; ttl++) {
		printf("%d: ", ttl);
		int finish = 0;
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
			if (res < 0) {
				perror("setsockopt");
			}
			
			// send_packet();
			rts.dest_addr.sin_port = htons(rts.port);
			int	socklen = sizeof(rts.dest_addr);
			res = sendto(rts.sockfd, packet, sizeof(packet), 0,
				(struct sockaddr *)&rts.dest_addr, socklen);
			if (res < 0) {
				perror("sendto");
				continue ;
			}
			// printf("Success to sendto(%d): %s\n", rts.seq, source);
			rts.seq++;
			gettimeofday(&rts.send_time, NULL);
			// recv_packet();
			int recv_res;
			recv_res = recv_packet(&rts);
			if (recv_res == 0) {
				finish = 1;
			}
			usleep(500);
			rts.port++;
		}
		printf("\n");
		if (finish) {
			printf("Success to reach destination\n");
			break ;
		}
	}
	
	// TODO: clear()
}

int	recv_packet(t_tr_rts *rts) {
	fd_set	r;
	char	recv_packet[256];

	FD_ZERO(&r);

	struct timeval	timeout;
	struct timeval	recv_time;
	while (1) {
		FD_SET(rts->recv_sockfd, &r);
		timeout.tv_sec = 0;
		timeout.tv_usec= 50000;
		int	count = select(rts->recv_sockfd + 1, &r, NULL, NULL, &timeout);
		// printf("count: %d\n", count);
		if (count > 0) {
			gettimeofday(&recv_time, NULL);
			double	rtt = (recv_time.tv_sec - rts->send_time.tv_sec) * 1000.0;
			rtt += (recv_time.tv_usec - rts->send_time.tv_usec) / 1000.0;
			struct sockaddr_in	client;
			int					addrlen;
			int	sender = recvfrom(
				rts->recv_sockfd,
				recv_packet,
				sizeof(recv_packet),
				0,
				(struct sockaddr *)&client,
				 (socklen_t *)&addrlen);

			char source[256];
				if (!inet_ntop(AF_INET, &client.sin_addr, source, INET_ADDRSTRLEN)) {
					perror("inet_ntop");
					return 1;
			}

			// printf("recvfrom source: %s\n", source);

			int res = parse_response(recv_packet, sizeof(recv_packet), rtt);
			if (res) {
				return 0;
			}
			break ;
			// print_response(&recv_packet, sizeof(recv_packet));
		}
		if (count <= 0) {
			printf("* ");
			return -1;
		}
	}
	return 1;
}

int	parse_response(char *buf, int buf_size, double rtt) {
	struct iphdr	ip;
	struct icmphdr	icmp;

	ft_memcpy(&ip, buf, sizeof(ip));
	ft_memcpy(&icmp, buf + ip.ihl * 4, sizeof(icmp));
	if (icmp.type == ICMP_ECHO) return 0;
	// printf("ttl: %d ", ip.ttl);
	if (icmp.type == ICMP_DEST_UNREACH || icmp.code == ICMP_PORT_UNREACH) {
		print_recv_result(&ip, &icmp, rtt);
		// printf("\n");
		return 1;
	}
	else if (icmp.type == ICMP_TIME_EXCEEDED) {
		// printf("Echo Time Exceeded accepted | ");
	} else {
		printf("ICMP type: %d, code: %d ", icmp.type, icmp.code);
	}
	print_recv_result(&ip, &icmp, rtt);
	return 0;
}

