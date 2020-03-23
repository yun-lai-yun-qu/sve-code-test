#ifdef __ARM_FEATURE_SVE
#include <arm_sve.h>
#else
#error "No Arm SVE intrinsics support in compiler"
#endif /* __ARM_FEATURE_SVE */

#include <stdio.h>

#define __START_TRACE() {asm volatile (".inst 0x2520e020");}
#define __STOP_TRACE() {asm volatile (".inst 0x2520e040");}

void testSVE()
{
	int n = 10;
	svfloat32_t C3;
	svbool_t pred = svwhilelt_b32_u32(0, n);
	C3 = svdup_n_f32(0.2);
	return;
}

void example_sum_squares( int N, int * sum)
{
	svbool_t pred_N;
	svint32_t svN_tmp;
	svbool_t p_all = svptrue_b32();
	svint32_t acc = svdup_s32(0);

	if (N > 0) {
		svN_tmp = svindex_s32(N, -1);
		pred_N = svcmpgt( p_all, svN_tmp, 0);

		do {
			acc = svmla_m( pred_N, acc, svN_tmp, svN_tmp);
			svN_tmp = svsub_x( p_all, svN_tmp, svcntw());
			pred_N = svcmpgt( p_all, svN_tmp, 0);
		} while (svptest_first( p_all, pred_N));
	}
	*sum = (int) svaddv( p_all, acc);
}

void main()
{
	int sum;

	testSVE();
	printf("hello world \n");
	printf("vector length (bytes): %d\n", svcntb());
	__START_TRACE();
	example_sum_squares(1000, &sum);
	__STOP_TRACE();
	printf("sum %d\n", sum);
}
