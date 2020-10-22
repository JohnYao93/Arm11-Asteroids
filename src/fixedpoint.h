#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

typedef _Bool bool;
#define true 1
#define false 0


#define PI 205887 //pi * 2^16
#define radtodeg 3754936
#define degtorad 1143
#define ATAN_ITERS 10
#define SQRT_ITERS 10
#define SIN_COS_ITERS 4 // Can't go higher than 8! in fixed point :/

typedef int fixed_t;

fixed_t to_fixed(int a);
int to_int(fixed_t a);

fixed_t mul(fixed_t a, fixed_t b);
fixed_t div(fixed_t a, fixed_t b);

fixed_t finvsqrt(fixed_t x);

fixed_t atan(fixed_t x);
fixed_t atan2(fixed_t y, fixed_t x);

fixed_t sin(fixed_t x);
fixed_t cos(fixed_t x);

fixed_t abs(fixed_t x);

#endif // FIXEDPOINT_H
