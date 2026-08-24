#include "utils.h"
#ifdef __cplusplus
extern "C" {
#endif
struct Result checkValue(struct Complex c, int maxIterations);
void calculateMandelbrot(struct Result* res, int rows, int cols, struct Range* xRange, struct Range* yRange);
#ifdef __cplusplus
}
#endif