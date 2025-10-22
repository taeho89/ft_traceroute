#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include "utils.h"

# define DFL_TTL 30
# define DFL_PROBE_PER_HOB 3

typedef struct s_traceroute_rts t_tr_rts;

struct s_traceroute_rts {
	int	ttl;
	int	probe_per_hop;
	int	port;
	int	sockfd;
	int	pid;
	int	seq;

	char	*origin_dest;
	struct addrinfo		dest_info;
	struct sockaddr_in	dest_addr;

	struct timeval	send_time;
};

/* traceroute_output.c */
void	print_help();
void	print_error(int errnum, char *errmsg);

/* utils.c */
int	parse_options(t_tr_rts *rts, int ac, char **av);

/* init.c */
void	init(t_tr_rts *rts);

/* icmp.c */
void	init_icmp_packet(t_tr_rts *rts, char *buf, int buf_size);

#endif
