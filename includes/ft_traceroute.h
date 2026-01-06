#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# include <netinet/ip.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <string.h>
# include <errno.h>
# include "utils.h"

# define DFL_MAX_TTL 10
# define DFL_PROBE_PER_HOB 3
# define DFL_TIMEOUT 3000
# define DFL_PACKLEN 60
# define MAX_INFLIGHT 16

typedef struct s_traceroute_rts t_tr_rts;
typedef struct s_slot			t_slot;

struct s_slot {
	// 송신 시 채우는 정보
	int		ttl;
	int		seq;
	struct timeval	sent_time;

	// 수신 시 채우는 정보
	struct sockaddr_in	ip_address;
	struct timeval		recv_time;

	// 상태 관리
	int		is_active;
	int		is_timeout;
};

struct s_traceroute_rts {
	int	max_ttl;
	int	pph;	// probe_per_hop
	int	timeout;
	int	packetlen;

	int	ttl;
	int	port;
	int	pid;
	int	seq;

	char	*origin_dest;
	struct addrinfo		dest_info;
	struct sockaddr_in	dest_addr;

	int	sockfd;
	int	recv_sockfd;

	t_slot	*inflight;
	int		slot_size;
};

/* traceroute_output.c */
void	print_help();
void	print_error(char *arg, int errnum, const char *errmsg);
int		print_log(t_slot *slots, int size, int pph, int ni, int dest_ttl);

/* utils.c */
int		parse_options(t_tr_rts *rts, int ac, char **av);
void	exit_with_error(int status, char *arg, int errnum, const char *errmsg);

/* init.c */
void	init(t_tr_rts *rts);

/* icmp.c */
void	init_icmp_packet(t_tr_rts *rts, char *buf);
int		checksum(void *packet, int len);

/* packet.c */
void	main_loop(t_tr_rts *rts);
void	send_packet(t_tr_rts *rts);
int		recv_packet(t_tr_rts *rts);
#endif
