#include "random.h"

unsigned rand(unsigned long long* rand_seed) {
	*rand_seed = (*rand_seed * 0x5DEECE66DL + 0xBL) & ((1LL << 48) - 1);
	return (unsigned) (*rand_seed >> 16);
}
