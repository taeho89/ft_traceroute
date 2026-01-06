#include "../includes/utils.h"

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
	for (int i = 0; i < n; i++) {
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
	int	i;

	n = 0;
	i = 0;
	if (s[i] == '+') {
		i++;
	}
	while (s[i] && s[i] >= '0' && s[i] < '9') {
		n = n * 10 + s[i];
	}
	return n;
}
