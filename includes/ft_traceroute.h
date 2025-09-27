#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include "../libft/libft.h"

# define DFL_TTL 30
# define DFL_PROBE_PER_HOB 3

typedef struct s_traceroute_rts t_tr_rts;

struct s_traceroute_rts {
	int	ttl;
	int	probe_per_hop;
	int	port;
	int	sockfd;

};

/* traceroute_output.c */
void	print_help();
void	print_error(int errnum, char *errmsg);

/* utils.c */
int	parse_options(t_tr_rts *rts, int ac, char **av);
#endif
