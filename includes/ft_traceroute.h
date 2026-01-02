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
# include "utils.h"

# define DFL_MAX_TTL 30
# define DFL_PROBE_PER_HOB 3
# define MAX_INFLIGHT 16
# define DFL_TIMEOUT 5000

typedef struct s_traceroute_rts t_tr_rts;
typedef struct s_slot	t_slot;

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
	int	ttl;
	int	max_ttl;
	int	probe_per_hop;
	int	timeout;
	int	port;
	int	sockfd;
	int	recv_sockfd;
	int	pid;
	int	seq;
	t_slot	*inflight;

	char	*origin_dest;
	struct addrinfo		dest_info;
	struct sockaddr_in	dest_addr;

	struct timeval	send_time;
};

/* traceroute_output.c */
void	print_help();
void	print_error(int errnum, char *errmsg);
void	print_recv_result(struct iphdr *ip, struct icmphdr *icmp, double rtt);
int		print_log(t_slot *slots, int probe_per_hop, int next_to_print);

/* utils.c */
int	parse_options(t_tr_rts *rts, int ac, char **av);

/* init.c */
void	init(t_tr_rts *rts);

/* icmp.c */
void	init_icmp_packet(t_tr_rts *rts, char *buf, int buf_size);

void	main_loop(t_tr_rts *rts);
void	send_packet(t_tr_rts *rts);
void	recv_packet(t_tr_rts *rts);
#endif
