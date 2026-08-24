#include <mandelbrot.h>

struct Result checkValue(struct Complex c, int maxIterations) {
	struct Complex z = (struct Complex){ .real = 0, .imag = 0 };
	for (int cnt = 0; cnt < maxIterations; cnt++) {
		if (complexAbs(z) > 2.0) {
			return (struct Result) { .bounded = false, .tillInfty = cnt };
		}
		z = mulComplex(z, z);
		z = (struct Complex){ .real = z.real + c.real, .imag = z.imag + c.imag };
	}
	return (struct Result) { .bounded = true, .tillInfty = maxIterations };
}
