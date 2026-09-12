#pragma once
#include "julia.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>

uint16_t *calculateJulia(struct Complex c, struct Range* xRange, struct Range* yRange, Dimensions *dims, int maxIterations) {
	const int finalSize = dims->height * dims->width;
	uint16_t* data = (uint16_t*)calloc(finalSize, sizeof(uint16_t));
	if (data == NULL) {
		fprintf(stderr, "Failed to allocate memory for data array of the julia set\n");
		return NULL;
	}

	uint16_t iteration = 0;
	struct Complex z = { .real = 0.0, .imag = 0.0 };

	for (int row = 0; row < dims->height; row++) {
		for (int col = 0; col < dims->width; col++) {
			z.real = xRange->min + col * xRange->step;
			z.imag = yRange->min + row * yRange->step;
			iteration = 0;
			double real = z.real * z.real;
			double imag = z.imag * z.imag;
			while (real + imag < 4 && iteration < maxIterations) {
				double xtemp = real - imag;
				z.imag = 2 * z.real * z.imag + c.imag;
				z.real = xtemp + c.real;
				real = z.real * z.real;
				imag = z.imag * z.imag;
				iteration++;
			}
			data[row * dims->width + col] = iteration;
		}
	}
	return data;
}

void drawJuliaSet(uint16_t* res, int rows, int cols, int maxIterations) {
	for (int row = 0; row < GetScreenHeight(); row++) {
		for (int col = 0; col < GetScreenWidth(); col++) {
			if (res[row * GetScreenWidth() + col] >= maxIterations) {
				DrawPixel(col, row, BLACK);
			}
			else {
				DrawPixel(col, row, getColorsBasedOnInvocations(res[row * GetScreenWidth() + col], maxIterations));
			}
		}
	}
}
