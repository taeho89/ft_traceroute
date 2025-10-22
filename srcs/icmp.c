#include "../includes/ft_traceroute.h"
#include <netinet/ip_icmp.h>

int	checksum(void *packet, int len);

void	init_icmp_packet(t_tr_rts *rts, char *buf, int buf_size) {
	struct icmphdr	hdr;

	ft_memset(&hdr, 0, sizeof(hdr));
	hdr.type = ICMP_ECHO;
	hdr.code = 0;
	hdr.un.echo.id = htons(rts->pid);
	hdr.un.echo.sequence = htons(rts->seq);
	ft_memcpy(buf, &hdr, sizeof(hdr));
	((struct icmphdr *)buf)->checksum = checksum(buf, buf_size);
}

int	checksum(void *packet, int len) {
	uint16_t	*buf;
	int			sum;

	buf = packet;
	for (sum = 0; len > 1; len -= 2) {
		sum += *buf;
		buf++;
	}
	if (len > 0) {
		sum += *buf << 8;
	}
	while (sum >> 16) {
		sum = (sum >> 16) + (sum & 0xFFFF);
	}
	return ~sum;
}
