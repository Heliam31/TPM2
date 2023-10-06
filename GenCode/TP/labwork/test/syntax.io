
const BASE = 1000
var x
reg ODR @ 0x80000000

sig BIT0 @ ODR [ 0 ]

auto blink
	state OFF:
		x = 0 + 1
		stop
		when BIT0:
			stop
		when !BIT0:
			stop
	state ON:
		goto OFF

