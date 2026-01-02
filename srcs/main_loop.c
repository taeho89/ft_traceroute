#include "../includes/ft_traceroute.h"

void	check_timeout(t_slot *slots, int max_ttl, int probe_per_hop, int timeout);

void	print_slots(t_slot *slots, int size) {
	for (int i = 0; i < size; i++) {
		if (!slots[i].is_active) {
			continue ;
		}
		printf("[%d] ttl: %d seq: %d\n", i, slots[i].ttl, slots[i].seq);
	}
}
void	main_loop(t_tr_rts *rts) {
	int	next_to_print = 0;

	while (1) {
		// TODO: 송신
		send_packet(rts);
		// print_slots(rts->inflight, rts->probe_per_hop * rts->max_ttl);

		// TODO: 응답 대기 및 수신
		recv_packet(rts);

		check_timeout(rts->inflight, rts->max_ttl, rts->probe_per_hop, rts->timeout);

		// TODO: 출력
		next_to_print += print_log(rts->inflight, rts->probe_per_hop, next_to_print);
		if (next_to_print > 30) return ;

		usleep(1000);
	}
}

void	check_timeout(t_slot *slots, int max_ttl, int probe_per_hop, int timeout) {
	float rtt;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	for (int i = 0; i < max_ttl * probe_per_hop; i++) {
		if (!slots[i].is_active) continue ;
		rtt = (tv.tv_sec - slots[i].sent_time.tv_sec) * 1000.0;
		rtt += (tv.tv_usec - slots[i].sent_time.tv_usec) / 1000.0;
		if (rtt > timeout) {
			slots[i].is_active = 0;
			slots[i].is_timeout = 1;
		}
	}
}

