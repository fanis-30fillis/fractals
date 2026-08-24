#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
	struct Range {
		double min;
		double max;
		double step;
	};
	struct Complex {
		double real;
		double imag;
	};

	struct Result {
		bool bounded;
		int tillInfty;
	};

	struct Complex mulComplex(struct Complex a, struct Complex b);
	double complexAbs(struct Complex c);
#ifdef __cplusplus
}
#endif