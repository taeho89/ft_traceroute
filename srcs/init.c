#include "../includes/ft_traceroute.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

void	init(t_tr_rts *rts) {
	// 목적지 정보 받아오기
	struct addrinfo *info;
	struct addrinfo hints;

	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_UDP;
	int rc = getaddrinfo(rts->origin_dest, NULL, NULL, &info);
	if (rc < 0) {
		print_error(rts->origin_dest, 0, gai_strerror(rc));
		printf("Cannot handle \"%s\" cmd line arg `%s\' on position %d (argc %d)\n", "host", "as", 1, 1);
		exit(2);
		// exit_with_error(2, rts->origin_dest, 0, gai_strerror(rc));
		// exit_with_error(2, errno, gai_strerror(errno));
		// perror("error in getaddrinfo");
	}
	ft_memcpy(&rts->dest_addr, info->ai_addr, info->ai_addrlen);
	rts->dest_addr.sin_port = htons(33434);

	freeaddrinfo(info);

	// 기본 값 설정
	rts->max_ttl = DFL_MAX_TTL;
	rts->pph = DFL_PROBE_PER_HOB;
	rts->timeout = DFL_TIMEOUT;
	rts->pid = getpid();

	rts->port = 33434;
	rts->seq = 0;
	rts->ttl = 1;

	// Inlfight 배열 초기화
	rts->slot_size = rts->max_ttl * rts->pph;
	rts->inflight = malloc(rts->slot_size * sizeof(t_slot));
	if (!rts->inflight) {
		exit_with_error(1, NULL, errno, NULL);
	}
	ft_memset(rts->inflight, 0, rts->slot_size * sizeof(t_slot));

	// 응답 패킷 초기화
	rts->recv_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rts->recv_sockfd == -1) {
		exit_with_error(1, NULL, errno, NULL);
	}
}
