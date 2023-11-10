var cnt
reg GPIOA_IDR	@ 0x80002000
sig BUTTON @ GPIOA_IDR[3]

auto A
	cnt = 0

	state UP:
		when BUTTON:
			goto DOWN

	state DOWN:
		when !BUTTON:
			cnt = cnt + 1

