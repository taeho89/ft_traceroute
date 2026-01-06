#include "../includes/ft_traceroute.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

int	init(t_tr_rts *rts, char **argv, t_arg arg) {
	// 목적지 정보 받아오기
	struct addrinfo *info;
	struct addrinfo hints;

	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_UDP;
	rts->origin_dest = argv[arg.host_pos];
	int rc = getaddrinfo(rts->origin_dest, NULL, NULL, &info);
	if (rc < 0) {
		print_error(rts->origin_dest, 0, gai_strerror(rc));
		fprintf(stderr, "Cannot handle \"%s\" cmd line arg `%s\' on position %d (argc %d)\n", \
		  "host", rts->origin_dest, 1, arg.host_pos);
		return -1;
	}
	ft_memcpy(&rts->dest_addr, info->ai_addr, info->ai_addrlen);
	rts->dest_addr.sin_port = htons(33434);

	freeaddrinfo(info);

	printf("host_pos: %d, packetlen_pos: %d\n", arg.host_pos, arg.packetlen_pos);
	if (arg.packetlen_pos) {
		rts->packetlen = ft_atoi(argv[arg.packetlen_pos]);
		if (!rts->packetlen) {
			fprintf(stderr, "Cannot handle \"%s\" cmd line arg `%s\' on position %d (argc %d)\n", \
		   		"packetlen", argv[arg.packetlen_pos], 2, arg.packetlen_pos);
			return -1;
		}
		if (rts->packetlen > MAX_PACKLEN) {
			fprintf(stderr, "too big packetlen %d specified\n", rts->packetlen);
			return -1;
		}
		if (rts->packetlen < MIN_PACKLEN) {
			rts->packetlen = MIN_PACKLEN;
		}
		if (rts->packetlen < 0) {
			rts->packetlen = DFL_PACKLEN;
		}
	}
	else {
		rts->packetlen = DFL_PACKLEN;
	}
	// 기본 값 설정
	rts->max_ttl = DFL_MAX_TTL;
	rts->pph = DFL_PROBE_PER_HOB;
	rts->timeout.tv_sec = DFL_TIMEOUT;
	rts->timeout.tv_usec = 0;
	rts->pid = getpid();

	rts->port = 33434;
	rts->seq = 0;
	rts->ttl = 1;

	// Inlfight 배열 초기화
	rts->slot_size = rts->max_ttl * rts->pph;
	rts->inflight = malloc(rts->slot_size * sizeof(t_slot));
	rts->packet = malloc(rts->packetlen * sizeof(char));
	if (!rts->inflight || !rts->packet) {
		exit_with_error(rts, 1, NULL, errno, NULL);
	}
	ft_memset(rts->inflight, 0, rts->slot_size * sizeof(t_slot));

	// 응답 패킷 초기화
	rts->recv_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rts->recv_sockfd == -1) {
		exit_with_error(rts, 1, NULL, errno, NULL);
	}
	return 0;
}
