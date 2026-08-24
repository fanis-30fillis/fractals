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

void calculateMandelbrot(struct Result* res, int rows, int cols, struct Range* xRange, struct Range* yRange) {
	int rowNum = 0, colNum = 0;

	for (double row = yRange->min; row < yRange->max && rowNum < rows; row += yRange->step, rowNum++) {
		double col = xRange->min;
		#pragma omp parallel for
		for (colNum = 0; colNum < cols; colNum++) {
			double col = xRange->min + colNum * xRange->step;
			struct Complex c = { .real = col, .imag = row };
			struct Result result = checkValue(c, 400);
			res[rowNum * cols + colNum] = result;
		}
	}
}

