#include "../includes/ft_traceroute.h"
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
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
		t_slot	*slot;
		if (rts->ttl > rts->max_ttl) return ;

		int	probe = rts->seq % rts->probe_per_hop;
		while (probe < rts->probe_per_hop) {
			// 매번 포트 번호를 다르게 보내기 위하여 재생성
			rts->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (rts->sockfd == -1) {
				perror("error in creating socket");
				exit(1);
			}
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

			rts->dest_addr.sin_port = htons(rts->port++);
			res = sendto(rts->sockfd, packet, sizeof(packet), 0,
				(struct sockaddr *)&rts->dest_addr, sizeof(rts->dest_addr));
			if (res < 0) {
				perror("sendto");
				continue ;
			}

			// i % MAX_INFLIGHT와 같은 연산
			slot = &rts->inflight[rts->seq];
			slot->ttl = rts->ttl;
			slot->seq = rts->seq;
			gettimeofday(&slot->sent_time, NULL);
			slot->is_active = 1;

			// printf("Success to sendto(%d): %d\n", rts->seq, rts->port);
			rts->seq++;
			probe++;
			inflight_count++;
			close(rts->sockfd);
		}
		rts->ttl++;
	}
}

int	recv_packet(t_tr_rts *rts) {
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
		return 0;
	}

	int	dest_probe = 0;
	int	dest_ttl = 0;
	for (int i = 0; i < count; i++) {
		// 수신
		struct sockaddr_in	client;
		socklen_t addrlen = sizeof(struct sockaddr_in);
		while(recvfrom(rts->recv_sockfd, recv_packet, sizeof(recv_packet), \
		   MSG_DONTWAIT, (struct sockaddr *)&client, &addrlen) > 0) {
			// 수신 데이터 파싱해서 정보 저장
			struct timeval	recv_time;
			gettimeofday(&recv_time, NULL);

			struct iphdr	*ip = (struct iphdr *)(recv_packet);
			// 1. ICMP 헤더 시작점
			struct icmphdr *icmp = (struct icmphdr *)(recv_packet + ip->ihl * 4);
			// 2. ICMP 페이로드에 담긴 '원본 IP 헤더' 시작점 (ICMP 헤더 8바이트 뒤)
			struct iphdr *orig_ip = (struct iphdr *)((char *)icmp + 8);
			// 3. 원본 UDP 헤더 시작점 (원본 IP 헤더의 IHL 뒤)
			struct udphdr *orig_udp = (struct udphdr *)((char *)orig_ip + (orig_ip->ihl * 4));
			int	index;
			index = ntohs(orig_udp->dest) - 33434;
			// printf("index: %d\n", index);

			// printf("Index: %d\n", index);
			if (icmp->type == ICMP_ECHO) continue ;
			if (icmp->type == ICMP_DEST_UNREACH || icmp->code == ICMP_PORT_UNREACH) {
				// 목적지 도착
				// printf("Destination, index: %d, ttl: %d\n", index, rts->inflight[index].ttl);
				// printf("Timeout: %d\n", rts->inflight[index].is_timeout);
				dest_ttl = rts->inflight[index].ttl;
				dest_probe++;
				rts->inflight[index].is_active = 0;
				rts->inflight[index].ip_address = client;
				rts->inflight[index].recv_time = recv_time;
				if (dest_probe == rts->probe_per_hop) return dest_ttl;
			}
			if (icmp->type == ICMP_TIME_EXCEEDED) {
				// ttl 만료
				rts->inflight[index].is_active = 0;
				rts->inflight[index].ip_address = client;
				rts->inflight[index].recv_time = recv_time;
			}
		}
	}
	return 0;
}

