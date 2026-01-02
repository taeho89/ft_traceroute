#include "../includes/ft_traceroute.h"
#include <netinet/ip_icmp.h>
#include <sys/socket.h>

void	send_packet(t_tr_rts *rts) {
	char packet[64];
	int	inflight_count;

	inflight_count = 0;
	for (int i = 0; i < rts->seq; i++) {
		if (rts->inflight[i].is_active)
			inflight_count++;
	}

	while (1) {
		t_slot	slot;
		if (rts->ttl > rts->max_ttl) return ;

		int	probe = rts->seq % rts->probe_per_hop;
		while (probe < rts->probe_per_hop) {
			if (inflight_count == MAX_INFLIGHT) return ;
			ft_memset(packet, 0, sizeof(packet));
			init_icmp_packet(rts, packet, sizeof(packet));

			// set socket option;
			int res = setsockopt(
				rts->sockfd,
				IPPROTO_IP,
				IP_TTL,
				&rts->ttl,
				sizeof(rts->ttl)
			);
			if (res < 0) {
				perror("setsockopt");
			}

			// send_packet();
			rts->dest_addr.sin_port = htons(rts->port++);
			res = sendto(rts->sockfd, packet, sizeof(packet), 0,
				(struct sockaddr *)&rts->dest_addr, sizeof(rts->dest_addr));
			if (res < 0) {
				perror("sendto");
				continue ;
			}

			// i % MAX_INFLIGHT와 같은 연산
			slot.ttl = rts->ttl;
			slot.seq = rts->seq;
			gettimeofday(&slot.sent_time, NULL);
			slot.is_active = 1;
			rts->inflight[rts->seq++] = slot;

			printf("Success to sendto(%d): %d\n", rts->seq, rts->port);
			probe++;
			inflight_count++;
		}
		rts->ttl++;
	}
}

void	recv_packet(t_tr_rts *rts) {
	fd_set	r;
	char	recv_packet[256];

	FD_ZERO(&r);
	FD_SET(rts->recv_sockfd, &r);

	int	count;

	struct timeval	timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	count = select(rts->recv_sockfd + 1, &r, NULL, NULL, &timeout);
	if (count < 0) {
		// 타임아웃 처리
		// 예외처리
		return ;
	}

	printf("count: %d\n", count);
	for (int i = 0; i < count; i++) {
		// 수신
		struct sockaddr_in	client;
		int					addrlen;
		recvfrom(rts->recv_sockfd, recv_packet, sizeof(recv_packet), \
		   0, (struct sockaddr *)&client, (socklen_t *)&addrlen);

		// 수신 데이터 파싱해서 정보 저장
		struct timeval	recv_time;
		gettimeofday(&recv_time, NULL);

		struct iphdr	ip;
		struct icmphdr	icmp;
		int				index;
		ft_memcpy(&ip, recv_packet, sizeof(ip));
		ft_memcpy(&icmp, recv_packet + ip.ihl * 4, sizeof(icmp));
		index = icmp.un.echo.sequence;

		// printf("Index: %d\n", index);
		if (icmp.type == ICMP_ECHO) continue ;
		if (icmp.type == ICMP_DEST_UNREACH || icmp.code == ICMP_PORT_UNREACH) {
			// 목적지 도착
			printf("Destination\n");
			rts->inflight[index].is_active = 0;
			rts->inflight[index].ip_address = client;
			rts->inflight[index].recv_time = recv_time;
		}
		if (icmp.type == ICMP_TIME_EXCEEDED) {
			// ttl 만료
			rts->inflight[index].is_active = 0;
			rts->inflight[index].ip_address = client;
			rts->inflight[index].recv_time = recv_time;
		}
	}
}

