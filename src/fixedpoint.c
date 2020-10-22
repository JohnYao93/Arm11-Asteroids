#include "fixedpoint.h"

fixed_t to_fixed(int a) {
	return a << 16;
}

int to_int(fixed_t a) {
	return a >> 16;
}

fixed_t mul(fixed_t a, fixed_t b) {
	long long aL = a;
	long long bL = b;
	long long res = aL * bL;
	return (fixed_t)(res >> 16);
}

fixed_t div(fixed_t a, fixed_t b) {
	return (((long long)a) << 16) / b;
}

fixed_t atan(fixed_t x) {
  // WARNING: Only accurate to +-0.041 radians (about 2.3 degrees)
  if (x < 0)         return -atan(-x);
  if (x > (1 << 16)) return (PI >> 1) - atan(div(1 << 16, x));
  
	const fixed_t negxSq = -mul(x, x);
	const fixed_t negxCb = mul(x, negxSq);
	const fixed_t xpow5  = mul(negxCb, negxSq);
	const fixed_t negx7  = mul(xpow5, negxSq);
	const fixed_t xpow9  = mul(negx7, negxSq);
	const fixed_t negx11 = mul(xpow9, negxSq);
	fixed_t res = x;
	res += div(negxCb, (3 << 16));
	res += div(xpow5,  (5 << 16));
	res += div(negx7,  (7 << 16));
	res += div(xpow9,  (9 << 16));
	res += div(negx11, (11<< 16));
	res = mul(res, (0x108 <<  8)); // res *= (1+1/32)
	return res;
}

fixed_t atan2(fixed_t y, fixed_t x) {
	fixed_t yx = div(y, x);
	if (x > 0) {
		return atan(yx);
	} else if (x < 0) {
		if (y >= 0) {
			return atan(yx) + PI;
		} else {
			return atan(yx) - PI;
		}
	} else {
		// x = 0
		if (y > 0) {
			return PI >> 1; // pi/2
		} else if (y < 0) {
			return -(PI >> 1);
		}
		return 0;
	}
}

fixed_t abs(fixed_t x) {
	return (x < 0)? -x:x;
}

fixed_t sin(fixed_t x) {
	return cos(x - (PI >> 1));
}

fixed_t cos(fixed_t x) {
	if (x > (PI >> 1)) { // x > pi/2
		return -cos(x-PI);
	} else if (x < 0) {
		return cos(-x);
	}
	const fixed_t negxSq = -mul(x, x);
	fixed_t res = (1 << 16); // res = 1
	res += div(negxSq, (2 << 16)); // res = 1 - (x^2)/2
	const fixed_t x4 = mul(negxSq, negxSq);
	res += div(x4, (24 << 16)); // res = 1 - (x^2)/2 + (x^4)/24
	res += div(mul(negxSq, x4), (720 << 16)); // res = 1 - (x^2)/2 + (x^4)/24 - (x^6)/720
	res += div(mul(x4, x4), (40320 << 16));  // res = 1 - (x^2)/2 + (x^4)/24 - (x^6)/720 + (x^8)/40320
	
	if (res < -(0xFF << 8)) res = -(1 << 16);
	if (res >  (0xFF << 8)) res =  (1 << 16);
	return res;
}

fixed_t finvsqrt(fixed_t x) {
  fixed_t y = div(to_fixed(1), x);
  for (int i = 0; i < SQRT_ITERS; ++i) {
    // y = y * (3 - x*(y*y)) / 2
    y = mul(y, ((3 << 16) - mul(x, mul(y, y)))) >> 1;
  }
  return (y > 0)? y:-y;
}
