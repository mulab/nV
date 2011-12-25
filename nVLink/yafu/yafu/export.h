#pragma once
typedef struct {
	uint32 num_factors;
	factor_t *factors;
} factors_t;
#if __cplusplus
extern "C" {
#endif
void begin_factors(void);
factors_t get_factors(mpz_t N);
void end_factors(void);
#if __cplusplus
}
#endif
