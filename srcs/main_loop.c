#include "../includes/ft_traceroute.h"

void	check_timeout(t_slot *slots, int slot_size, struct timeval timeout);

void	main_loop(t_tr_rts *rts) {
	int	ni;	// next_index
	int	dst_ttl;

	ni = 0;
	while (1) {
		// 송신
		send_packet(rts);

		// 응답 대기 및 수신
		dst_ttl = recv_packet(rts);

		check_timeout(rts->inflight, rts->slot_size, rts->timeout);

		// 출력
		ni += print_log(rts->inflight, rts->slot_size, rts->pph, ni, dst_ttl);
		if (dst_ttl > 0 || rts->ttl > rts->max_ttl) return ;
	}
}

void	check_timeout(t_slot *slots, int slot_size, struct timeval timeout) {
	struct timeval	tv;
	float 			rtt;

	gettimeofday(&tv, NULL);
	for (int i = 0; i < slot_size; i++) {
		if (!slots[i].is_active) continue ;
		rtt = (tv.tv_sec - slots[i].sent_time.tv_sec) * 1000.0;
		rtt += (tv.tv_usec - slots[i].sent_time.tv_usec) / 1000.0;
		if (rtt > timeout.tv_sec * 1000.0 + timeout.tv_usec / 1000.0) {
			slots[i].is_active = 0;
			slots[i].is_timeout = 1;
		}
	}
}
