#include "../includes/ft_traceroute.h"
#include <arpa/inet.h>
#include <netinet/in.h>

void	init(t_tr_rts *rts) {
	struct addrinfo *info;
	struct addrinfo hints;

	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_UDP;
	int rc = getaddrinfo(rts->origin_dest, NULL, NULL, &info);
	if (rc < 0) {
		perror("error in getaddrinfo");
	}
	// printf("ip: %s\n", rts->dest_addr)
	ft_memcpy(&rts->dest_addr, info->ai_addr, info->ai_addrlen);
	rts->port = 33434;
	rts->dest_addr.sin_port = htons(33434);

	// inet_pton(AF_INET, rts->origin_dest, &rts->dest_addr);

	rts->pid = getpid();
	rts->seq = 0;

	rts->ttl = 1;
	rts->max_ttl = DFL_MAX_TTL;
	rts->probe_per_hop = DFL_PROBE_PER_HOB;
	rts->timeout = DFL_TIMEOUT;

	rts->inflight = malloc(rts->max_ttl * rts->probe_per_hop * sizeof(t_slot));
	if (!rts->inflight) {
		perror("malloc failed");
		exit(1);
	}
	ft_memset(rts->inflight, 0, rts->max_ttl * rts->probe_per_hop * sizeof(t_slot));
}
