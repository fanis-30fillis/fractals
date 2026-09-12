#pragma once
#include <raylib.h>
#include "utils.h"

Color getJuliaColorBasedOnInvocations(int numOfInvocations);
uint16_t* calculateJulia(struct Complex c, struct Range* xRange, struct Range* yRange, Dimensions* dims, int maxIterations);
void drawJuliaSet(uint16_t* res, int rows, int cols, int maxIterations);