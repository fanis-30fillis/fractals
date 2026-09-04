#include <mandelbrot.h>

void checkValue(struct Complex c, struct Result *res, int maxIterations) {
	struct Complex z{.real = 0, .imag = 0};
	for (int cnt = 0; cnt < maxIterations; cnt++) {
		if (z.real * z.real + z.imag * z.imag > 4.0) {
			res->bounded = false;
			res->tillInfty = cnt;
			return;
		}
		const double real = z.real * z.real - z.imag * z.imag + c.real;
		z.imag = z.real * z.imag + z.imag * z.real + c.imag;
		z.real = real;
	}
	res->bounded = true;
}

void calculateMandelbrot(struct Result* res, int rows, int cols, struct Range* xRange, struct Range* yRange) {
	int rowNum = 0, colNum = 0;

	for (double row = yRange->min; row < yRange->max && rowNum < rows; row += yRange->step, rowNum++) {
		double col = xRange->min;
		#pragma omp parallel for
		for (colNum = 0; colNum < cols; colNum++) {
			double col = xRange->min + colNum * xRange->step;
			struct Complex c = { .real = col, .imag = row };
			checkValue(c, &res[rowNum * cols + colNum], 800);
		}
	}
}
