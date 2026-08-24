#include "utils.h"
#include <math.h>

struct Complex mulComplex(struct Complex a, struct Complex b) {
	return (struct Complex) { .real = a.real * b.real - a.imag * b.imag, .imag = a.real * b.imag + a.imag * b.real };
}
double complexAbs(struct Complex c) {
	return sqrt(c.real * c.real + c.imag * c.imag);
}