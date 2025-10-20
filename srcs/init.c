#include "../includes/ft_traceroute.h"
#include <arpa/inet.h>

void	init(t_tr_rts *rts) {
	struct addrinfo *info;
	struct addrinfo hints;

	// int rc = getaddrinfo(rts->origin_dest, NULL, &hints, &info);
	// if (rc < 0) {
	// 	perror("error in getaddrinfo");
	// }
	//
	inet_pton(AF_INET, rts->origin_dest, &rts->dest_addr);
}
