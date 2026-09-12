#include "utils.h"
#include <math.h>
#include <raylib.h>

Complex mulComplex(Complex a, Complex b) {
	return Complex { .real = a.real * b.real - a.imag * b.imag, .imag = a.real * b.imag + a.imag * b.real };
}
double complexAbs(Complex c) {
	return sqrt(c.real * c.real + c.imag * c.imag);
}
Color getColorsBasedOnInvocations(int numOfInvocations, int maxInvocations) {
	constexpr int NUM_COLORS = 7;
	static const Color colors[NUM_COLORS] = {
		RED,
		ORANGE,
		YELLOW,
		GREEN,
		BLUE,
		PURPLE,
		WHITE
	};
	if (maxInvocations <= 0) {
		return BLACK; // or whatever error/default color you prefer
	}

	if (numOfInvocations >= maxInvocations) {
		return BLACK;
	}

	int colorIndex = (numOfInvocations * NUM_COLORS) / maxInvocations;

	return colors[colorIndex];
}

