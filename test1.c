#ifdef __ARM_FEATURE_SVE
#include <arm_sve.h>
#else
#error "No Arm SVE support"
#endif /* __ARM_FEATURE_SVE */

#include <stdio.h>

void testSVE()
{
	int n = 10;
	svfloat32_t C3;
	svbool_t pred = svwhilelt_b32_u32(0, n);
	C3 = svdup_n_f32(0.2);
	return;
}

void main()
{
	testSVE();
	printf("hello world \n");
}
