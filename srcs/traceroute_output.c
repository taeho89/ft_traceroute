#include "../includes/ft_traceroute.h"

void	print_error(int errnum, char *errmsg) {
	fprintf(stderr, "ft_traceroute: ");

	if (errnum)
		perror("error");
		// fprintf(stderr, "%s", strerror(errnum));
	else
		fprintf(stderr, "%s", errmsg);
	fprintf(stderr, "\n");
}

void	print_help() {
	printf(
		"\nUsage:\n"
		"  traceroute host\n"
		"\nOptions:\n"
		"  --help        Read this help and exit\n"
		"\nArguments:\n"
		"+      host           The host to traceroute to\n"
		"       packetlen      The full packet length (default is the length of an IP\n"
		"                      header plus 40). Can be ignored or increased to a minimal\n"
		"                      allowed value\n\n");
}
