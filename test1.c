#ifdef __ARM_FEATURE_SVE
#include <arm_sve.h>
#else
#error "No Arm SVE intrinsics support in compiler"
#endif /* __ARM_FEATURE_SVE */

#include <stdio.h>

#define __START_TRACE() {asm volatile (".inst 0x2520e020");}
#define __STOP_TRACE() {asm volatile (".inst 0x2520e040");}

extern void bl_dtrmm_asm_sve_8x8 ( int k,
        double *a,
        double *b,
        double *c,
        unsigned long long ldc,
        void   *data,
        int    offset );

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


void test_svld1_f64( void )
{
	int          inca = 1;
	svfloat64_t  z_a0;
        svfloat64_t  z_a4;
	const  svbool_t all_active = svptrue_b64();
	double pi1[16];
	double alpha1[16] = { 1.0, 1.1, 1.2, 1.3,
                             2.01, 2.12, 2.23, 2.34 };

	// load
#if 1
	z_a0 = svld1_f64( all_active, alpha1 );
	z_a4 = svld1_vnum_f64( all_active, alpha1, 1);
#else
        double* restrict alpha1_4   = alpha1 + 4 * inca;
	svuint64_t   z_index = svindex_u64( 0, inca * sizeof( double) );
	z_a0 = svld1_gather_u64offset_f64( all_active, alpha1, z_index );
        z_a4 = svld1_gather_u64offset_f64( all_active, alpha1_4, z_index );
#endif

        // store them into *p
        svst1_f64( all_active, pi1, z_a0 );
        svst1_vnum_f64( all_active, pi1, 1, z_a4 );

	printf("Loaded by svld1_f64:\n");
        printf("%f, %f, %f, %f\n", pi1[0], pi1[1], pi1[2], pi1[3]);
	printf("Loaded by svld1_vnum_f64:\n");
        printf("%f, %f, %f, %f\n", pi1[4], pi1[5], pi1[6], pi1[7]);
	return;
}

void test_divide_12 ( int a )
{
	long register b;
	long int c;
	scanf("Input: %l", &c);
	b = c / 12;
	printf("Resutl: %l", b);
	return;
}


void main()
{
	int sum;

	testSVE();
	printf("hello world \n");
	printf("vector length (bytes): %d\n", svcntb());
//	__START_TRACE();
	example_sum_squares(1000, &sum);
//	__STOP_TRACE();
	printf("sum %d\n", sum);

        printf("call assembly routines\n");

	double packA[16] = { 1.0, 1.1, 1.2, 1.3,
                             2.0, 2.1, 2.2, 2.3 };
        double packB[16];
        double C[64];
        int    k = 8;

	printf("Original A is:\n");
        printf("%f, %f, %f, %f\n", packA[0], packA[1], packA[2], packA[3]);
        printf("%f, %f, %f, %f\n", packA[4], packA[5], packA[6], packA[7]);

	// input: packA
	// output: packB, C
        bl_dtrmm_asm_sve_8x8( k, packA, packB, C, 0, NULL, 0 );

	printf("Copied by ldr/str:\n");
        printf("%f, %f, %f, %f\n", C[0], C[1], C[2], C[3]);
        printf("%f, %f, %f, %f\n", C[4], C[5], C[6], C[7]);

	printf("Copied by ld1d/str:\n");
        printf("%f, %f, %f, %f\n", packB[0], packB[1], packB[2], packB[3]);
        printf("%f, %f, %f, %f\n", packB[4], packB[5], packB[6], packB[7]);

	// test svld1_f64();
	test_svld1_f64();
	// how assembly divide
	test_divide_12( sum );
}
