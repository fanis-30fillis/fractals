#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "mandelbrot.h"
#include <cstdint>
#include "utils.h"
#include <iostream>
#include <complex>
#include <julia.h>

#define MAX_ITERATIONS_JULIA 100
#define MAX_ITERATIONS_MANDELBROT 600

void drawArray(struct Result* res, int rows, int cols) {
	for (int row = 0; row < GetScreenHeight(); row++) {
		for (int col = 0; col < GetScreenWidth(); col++) {
			if (res[row * GetScreenWidth() + col].bounded) {
				DrawPixel(col, row, BLACK);
			} else {
				DrawPixel(col, row, getColorsBasedOnInvocations(res[row * GetScreenWidth() + col].tillInfty, MAX_ITERATIONS_MANDELBROT));
			}
		}
	}
}

int showJulia(struct Complex constant)
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	// Create the window and OpenGL context
	InitWindow(800, 600, "Julia Set");

	double xSpan = 8.0;
	double ySpan = xSpan * GetScreenHeight() / GetScreenWidth();

	struct Range xRange = {
		.min = -xSpan / 2,
		.max = xSpan / 2,
		.step = xSpan / GetScreenWidth()
	};

	struct Range yRange = {
		.min = -ySpan / 2.0,
		.max = ySpan / 2.0,
		.step = ySpan / GetScreenHeight()
	};
	Dimensions dims = { .width = GetScreenWidth(), .height = GetScreenHeight() };
	uint16_t* array = calculateJulia(constant, &xRange, &yRange, &dims, MAX_ITERATIONS_JULIA);
	double currentZoom = 1.0;
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();
		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLUE);
		// up means 0.8 which zooms in, down means 1.2 which zooms out.
		// 0.2 is the zoom factor per mouse wheel click
		Vector2 wheel = GetMouseWheelMoveV();
		if (wheel.y != 0) {

			Vector2 mouse = GetMousePosition();
			double xSpan = xRange.max - xRange.min;
			double ySpan = yRange.max - yRange.min;
			// Complex-plane coordinate currently beneath the mouse.
			// the mouse.x / GetScreenWidth() gets the percentage of the screen width that the mouse is at
			// and then we multiply that by the xSpan to get the actual coordinate in the complex plane.
			// add the xRange.min to get the actual coordinate in the complex plane.
			double mouseReal = xRange.min + ((double)mouse.x / GetScreenWidth()) * xSpan;
			// same thought process as mouseReal
			double mouseImag = yRange.min + ((double)mouse.y / GetScreenHeight()) * ySpan;

			// Positive wheel zooms in; negative wheel zooms out.
			double factor = pow(0.8, (double)wheel.y);
			currentZoom *= factor;
			// Scale both ranges around the mouse coordinate.
			xRange.min = mouseReal + (xRange.min - mouseReal) * factor;
			xRange.max = mouseReal + (xRange.max - mouseReal) * factor;

			yRange.min = mouseImag + (yRange.min - mouseImag) * factor;
			yRange.max = mouseImag + (yRange.max - mouseImag) * factor;

			xRange.step = (xRange.max - xRange.min) / GetScreenWidth();
			yRange.step = (yRange.max - yRange.min) / GetScreenHeight();
			free(array);
			array = calculateJulia(constant, &xRange, &yRange, &dims, MAX_ITERATIONS_JULIA);
		}
		else {
			if (IsKeyDown(KEY_LEFT)) {
				xRange.min = xRange.min + 0.05 * currentZoom;
				xRange.max = xRange.max + 0.05 * currentZoom;
				calculateJulia(constant, &xRange, &yRange, &dims, MAX_ITERATIONS_JULIA);
			}
			else if (IsKeyDown(KEY_RIGHT)) {
				xRange.min = xRange.min - 0.05 * currentZoom;
				xRange.max = xRange.max - 0.05 * currentZoom;
				calculateJulia(constant, &xRange, &yRange, &dims, MAX_ITERATIONS_JULIA);
			}
			else if (IsKeyDown(KEY_UP)) {
				yRange.min = yRange.min + 0.05 * currentZoom;
				yRange.max = yRange.max + 0.05 * currentZoom;
				calculateJulia(constant, &xRange, &yRange, &dims, MAX_ITERATIONS_JULIA);
			}
			else if (IsKeyDown(KEY_DOWN)) {
				yRange.min = yRange.min - 0.05 * currentZoom;
				yRange.max = yRange.max - 0.05 * currentZoom;
				calculateJulia(constant, &xRange, &yRange, &dims, MAX_ITERATIONS_JULIA);
			}
		}
		drawJuliaSet(array, dims.height, dims.width, MAX_ITERATIONS_JULIA);
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}


int showMandelbrot()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	// Create the window and OpenGL context
	InitWindow(800, 600, "Hello Raylib");
	struct Result* screen = (struct Result*)calloc(GetScreenWidth() * GetScreenHeight(), sizeof(struct Result));
	if (screen == NULL) {
		fprintf(stderr, "Failed to allocate memory for screen buffer\n");
		return 1;
	}

	double xSpan = 4.0;
	double ySpan = xSpan * GetScreenHeight() / GetScreenWidth();

	struct Range xRange = {
		.min = -2.5,
		.max = -2.5 + xSpan,
		.step = xSpan / GetScreenWidth()
	};

	struct Range yRange = {
		.min = -ySpan / 2.0,
		.max = ySpan / 2.0,
		.step = ySpan / GetScreenHeight()
	};
	calculateMandelbrot(screen, GetScreenHeight(), GetScreenWidth(), &xRange, &yRange);
	double currentZoom = 1.0;
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();
		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLUE);
		// up means 0.8 which zooms in, down means 1.2 which zooms out.
		// 0.2 is the zoom factor per mouse wheel click
		Vector2 wheel = GetMouseWheelMoveV();
		if (wheel.y != 0) {

			Vector2 mouse = GetMousePosition();
			double xSpan = xRange.max - xRange.min;
			double ySpan = yRange.max - yRange.min;
			//if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			//	handleClick(mouse, &xRange, &yRange);
			//}
			// Complex-plane coordinate currently beneath the mouse.
			// the mouse.x / GetScreenWidth() gets the percentage of the screen width that the mouse is at
			// and then we multiply that by the xSpan to get the actual coordinate in the complex plane.
			// add the xRange.min to get the actual coordinate in the complex plane.
			double mouseReal = xRange.min + ((double)mouse.x / GetScreenWidth()) * xSpan;
			// same thought process as mouseReal
			double mouseImag = yRange.min + ((double)mouse.y / GetScreenHeight()) * ySpan;

			// Positive wheel zooms in; negative wheel zooms out.
			double factor = pow(0.8, (double)wheel.y);
			currentZoom *= factor;
			// Scale both ranges around the mouse coordinate.
			xRange.min = mouseReal + (xRange.min - mouseReal) * factor;
			xRange.max = mouseReal + (xRange.max - mouseReal) * factor;

			yRange.min = mouseImag + (yRange.min - mouseImag) * factor;
			yRange.max = mouseImag + (yRange.max - mouseImag) * factor;

			xRange.step = (xRange.max - xRange.min) / GetScreenWidth();
			yRange.step = (yRange.max - yRange.min) / GetScreenHeight();

			calculateMandelbrot(
				screen,
				GetScreenHeight(),
				GetScreenWidth(),
				&xRange,
				&yRange
			);
		}
		else {
			if (IsKeyDown(KEY_LEFT)) {
				xRange.min = xRange.min + 0.05 * currentZoom;
				xRange.max = xRange.max + 0.05 * currentZoom;
				calculateMandelbrot(screen, GetScreenHeight(), GetScreenWidth(), &xRange, &yRange);
			}
			else if (IsKeyDown(KEY_RIGHT)) {
				xRange.min = xRange.min - 0.05 * currentZoom;
				xRange.max = xRange.max - 0.05 * currentZoom;
				calculateMandelbrot(screen, GetScreenHeight(), GetScreenWidth(), &xRange, &yRange);
			}
			else if (IsKeyDown(KEY_UP)) {
				yRange.min = yRange.min + 0.05 * currentZoom;
				yRange.max = yRange.max + 0.05 * currentZoom;
				calculateMandelbrot(screen, GetScreenHeight(), GetScreenWidth(), &xRange, &yRange);
			}
			else if (IsKeyDown(KEY_DOWN)) {
				yRange.min = yRange.min - 0.05 * currentZoom;
				yRange.max = yRange.max - 0.05 * currentZoom;
				calculateMandelbrot(screen, GetScreenHeight(), GetScreenWidth(), &xRange, &yRange);
			}
		}
		drawArray(screen, GetScreenHeight(), GetScreenWidth());
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

int main() {
	std::cout << "Show Mandlebrot set or Julia set? (m/j): ";
	char choice;
	std::cin >> choice;
	std::cout << std::endl;
	switch (choice) {
	case 'm':
	case 'M':
		return showMandelbrot();
	case 'j':
	case 'J':
		// TODO get the complex constant from the user
		double imag, real;

		std::cout << "Enter real part: ";
		std::cin >> real;
		std::cout << "Enter imaginary part: ";
		std::cin >> imag;


		if (!std::cin) {
			std::cerr << "Invalid complex number\n";
			return 1;
		}
		struct Complex constant = { .real = real, .imag = imag };
		std::cout << "Real: " << constant.real << ", Imaginary: " << constant.imag << std::endl;
		return showJulia(constant);
	}
}

