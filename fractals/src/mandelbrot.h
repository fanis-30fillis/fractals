#include "utils.h"
#ifdef __cplusplus
extern "C" {
#endif
void checkValue(struct Complex c, struct Result* res, int maxIterations);
void calculateMandelbrot(struct Result* res, int rows, int cols, struct Range* xRange, struct Range* yRange);
#ifdef __cplusplus
}
#endif