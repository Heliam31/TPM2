/* Test for declarations */

const C = 3
var x
var y = 5
reg GPIOA_SR @ 0x80000010
sig UP @ GPIOA_SR[3]

auto A
	state DOWN:
		when UP:
			goto DOWN
	state UP:
		when !UP:
			goto UP
