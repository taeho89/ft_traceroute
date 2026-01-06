#include "../includes/ft_traceroute.h"

static int	get_opt(int argc, char *argv);
static void	move_back(int ac, char **av, int target);

int	parse_options(t_tr_rts *rts, int ac, char **av) {
	int	cnt;
	int	c;

	cnt = 0;
	for (int i = 1; av[i]; i++) {
		if (av[i][0] == '-') {
			c = get_opt(i, av[i]);
			switch (c) {
				case 'h':
					print_help();
					exit(0);
				default:
					exit(2);
			}
			cnt++;
		}
		else
		move_back(ac, av, i);
	}
	return cnt;
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
	fprintf(stderr, "Bad option `%s' (argc %d)\n", argv, argc);
	return -1;
}

static void	move_back(int ac, char **av, int target) {
	char	*tmp;

	tmp = av[target];
	for (int i = target; i < ac - 1; i++) {
		av[i] = av[i + 1];
	}
	av[ac - 1] = tmp;
}

void	exit_with_error(int status, char *arg, int errnum, const char *errmsg) {
	print_error(arg, errnum, errmsg);
	exit(status);
}
