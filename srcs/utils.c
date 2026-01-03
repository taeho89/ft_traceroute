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

void	exit_with_error(int status, char *arg, int errnum, const char *errmsg) {
	print_error(arg, errnum, errmsg);
	exit(status);
}

void	ft_memset(void *s, int c, int n) {
	char	*p = s;

	for (int i = 0; i < n; i++) {
		p[i] = c;
	}
}

void	ft_memcpy(void *s1, void *s2, int n) {
	char	*p1 = s1;
	char	*p2 = s2;

	for (int i = 0; i < n; i++) {
		p1[i] = p2[i];
	}
}

int	ft_strncmp(char *s1, char *s2, int n) {
	int	i;

	for (i = 0; i < n; i++) {
		if (s1[i] != s2[i]) {
			return s1[i] - s2[i];
		}
	}
	return 0;
}

int	ft_strlen(char *str) {
	int	len;
	for (len = 0; str[len]; len++);
	return len;
}

int	ft_atoi(char *s) {
	int	n;

	n = 0;
	for (int i = 0; s[i]; i++) {
		// TODO: 예외 처리
		n = n * 10 + s[i];
	}
	return n;
}
