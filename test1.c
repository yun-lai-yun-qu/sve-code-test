#ifdef __ARM_FEATURE_SVE
#include <arm_sve.h>
#else
#error "No Arm SVE intrinsics support in compiler"
#endif /* __ARM_FEATURE_SVE */

#include <stdio.h>

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

void main()
{
	printf("hello world \n");
	printf("vector length (bytes): %d\n", svcntb());

	// test svld1_f64();
	test_svld1_f64();
}
