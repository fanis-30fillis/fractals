#include "utils.h"
#include <math.h>

Complex mulComplex(Complex a, Complex b) {
	return Complex { .real = a.real * b.real - a.imag * b.imag, .imag = a.real * b.imag + a.imag * b.real };
}
double complexAbs(Complex c) {
	return sqrt(c.real * c.real + c.imag * c.imag);
}