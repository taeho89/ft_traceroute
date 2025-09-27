#include "../includes/ft_traceroute.h"

static int	get_opt(int argc, char *argv);

int	parse_options(t_tr_rts *rts, int ac, char **av) {
	int	i = 1;
	int	c;

	while (av[i]) {
		if (av[i][0] == '-') {
			c = get_opt(ac, av[i]);
			switch (c) {
				default:
					print_help();
					exit(2);
			}
		}
		else {
			break ;
		}
		i++;
	}
	return i;
}

static int	get_opt(int argc, char *argv) {
	if (!ft_strncmp(argv, "--help", 7)) {
		return 'h';
	}
	if (!ft_strncmp(argv, "-h", 3)) {
		return 'h';
	}
	if (!ft_strncmp(argv, "-?", 3)) {
		return 'h';
	}
	fprintf(stderr, "Bad option `%s` (%d)\n", argv, argc);
	return -1;
}

void	exit_with_error(int status, int errnum, char *errmsg) {
	print_error(errnum, errmsg);
	exit(status);
}
