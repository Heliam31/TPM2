int main();

void _exit(int r);

void _start(void) {
	int r = main();
	_exit(r);
}


int one(void) {
	return 1;
}

int zero(void) {
	return 0;
}

int main(int argc, char **argv) {
	int r;
	int (*f)(void);

	/* exit example */
	if(argc == 0)
		_exit(1);

	/* branch example */
	if(argc <= 1)
		f = one;
	else
		f = zero;
	r = f();
	return r;
}
