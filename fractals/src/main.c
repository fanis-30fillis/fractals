#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "mandelbrot.h"

Color getColorBasedOnInvocations(int numOfInvocations) {
	if (numOfInvocations < 20) {
		return RED;
	}
	else if (numOfInvocations < 40) {
		return ORANGE;
	}
	else if (numOfInvocations < 60) {
		return YELLOW;
	}
	else if (numOfInvocations < 80) {
		return GREEN;
	}
	else if (numOfInvocations < 100) {
		return BLUE;
	}
	else if (numOfInvocations < 120) {
		return PURPLE;
	}
	return WHITE; // Placeholder, replace with actual color logic
}

void drawArray(struct Result* res, int rows, int cols) {
	for (int row = 0; row < GetScreenHeight(); row++) {
		for (int col = 0; col < GetScreenWidth(); col++) {
			if (res[row * GetScreenWidth() + col].bounded) {
				DrawPixel(col, row, BLACK);
			} else {
				DrawPixel(col, row, getColorBasedOnInvocations(res[row * GetScreenWidth() + col].tillInfty));
			}
		}
	}
}

void calculateMandelbrot(struct Result* res, int rows, int cols, struct Range *xRange, struct Range *yRange) {
	int rowNum = 0, colNum = 0;
	for (double row = yRange->min; row < yRange->max && rowNum < rows; row+= yRange->step, rowNum++) {
		for (double col = xRange->min; col < xRange->max && colNum < cols; col+= xRange->step, colNum++) {
			struct Complex c = { .real = col, .imag = row };
			struct Result result = checkValue(c, 200);
			res[rowNum * cols + colNum] = result;
		}
		colNum = 0;
	}
}

int main ()
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
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();
		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLUE);
		Vector2 wheel = GetMouseWheelMoveV();
		if(wheel.y != 0) {

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
		} else {
			if (IsKeyDown(KEY_LEFT)) {
				xRange.min = xRange.min + 0.05;
				xRange.max = xRange.max + 0.05;
				calculateMandelbrot(screen, GetScreenHeight(), GetScreenWidth(), &xRange, &yRange);
			}
			else if (IsKeyDown(KEY_RIGHT)) {
				xRange.min = xRange.min - 0.05;
				xRange.max = xRange.max - 0.05;
				calculateMandelbrot(screen, GetScreenHeight(), GetScreenWidth(), &xRange, &yRange);
			}
			else if (IsKeyDown(KEY_UP)) {
				yRange.min = yRange.min + 0.05;
				yRange.max = yRange.max + 0.05;
				calculateMandelbrot(screen, GetScreenHeight(), GetScreenWidth(), &xRange, &yRange);
			}
			else if (IsKeyDown(KEY_DOWN)) {
				yRange.min = yRange.min - 0.05;
				yRange.max = yRange.max - 0.05;
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
