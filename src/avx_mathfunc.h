#include <immintrin.h>

#define __MIN_NORM_POS__  (0x00800000)
#define __MANT_MASK__     (0x7f800000)
#define __INV_MANT_MASK__ (~0x7f800000)

#define __SIGN_MASK__      (0x80000000)
#define __INV_SIGN_MASK__  (~0x80000000)

#define __CEPHES_SQRTHF__  (0.707106781186547524)
#define __CEPHES_LOG_P0__  (7.0376836292E-2)
#define __CEPHES_LOG_P1__  (-1.1514610310E-1)
#define __CEPHES_LOG_P2__  (1.1676998740E-1)
#define __CEPHES_LOG_P3__  (-1.2420140846E-1)
#define __CEPHES_LOG_P4__  (1.4249322787E-1)
#define __CEPHES_LOG_P5__  (-1.6668057665E-1)
#define __CEPHES_LOG_P6__  (+2.0000714765E-1)
#define __CEPHES_LOG_P7__  (-2.4999993993E-1)
#define __CEPHES_LOG_P8__  (+3.3333331174E-1)
#define __CEPHES_LOG_Q1__  (-2.12194440E-4)
#define __CEPHES_LOG_Q2__  (0.693359375)

__m256 log256_ps(__m256 _x)
{
  __m256i _i0;
  __m256  _one = _mm256_set1_ps(1.0f);
  __m256  _half = _mm256_set1_ps(0.5f);

  //  the smallest non-denormalized positive float number 
  __m256i _min_norm_pos = _mm256_set1_epi32(__MIN_NORM_POS__);
  // mantissa mask 
  __m256i _mant_mask = _mm256_set1_epi32(__MANT_MASK__);
  // and its complement
  __m256i _inv_mant_mask = _mm256_set1_epi32(__INV_MANT_MASK__);

  __m256 _invalid_mask = _mm256_cmp_ps(_x, _mm256_setzero_ps(), _CMP_LE_OS);

  _x = _mm256_max_ps(_x, (__m256)_min_norm_pos);  // cut off denormalized stuff 

  _i0 = _mm256_srli_epi32(_mm256_castps_si256(_x), 23);
  // keep only the fractional part
  _x = _mm256_and_ps(_x, (__m256)_inv_mant_mask);
  _x = _mm256_or_ps(_x, _half);

  _i0 = _mm256_sub_epi32(_i0, _mm256_set1_epi32(0x7f));
  __m256 _e = _mm256_cvtepi32_ps(_i0);

  _e = _mm256_add_ps(_e, _one);

  /*
    if (x < SQRTHF ) {
       e -= 1;
       x = x + x -1.0;
    }else{
       x = x-1.0;
    }
   */

  __m256 _mask = _mm256_cmp_ps(_x, _mm256_set1_ps(__CEPHES_SQRTHF__), _CMP_LT_OS);
  __m256 _tmp  = _mm256_and_ps(_x, _mask);
  _x = _mm256_sub_ps(_x, _one);
  _e = _mm256_sub_ps(_e, _mm256_and_ps(_one, _mask));
  _x = _mm256_add_ps(_x, _tmp);

  __m256 _z = _mm256_mul_ps(_x, _x);

  __m256 _y = _mm256_set1_ps(__CEPHES_LOG_P0__);
#if 1
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_LOG_P1__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_LOG_P2__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_LOG_P3__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_LOG_P4__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_LOG_P5__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_LOG_P6__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_LOG_P7__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_LOG_P8__));
  _y = _mm256_mul_ps(_y, _x);
#else
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_LOG_P1__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_LOG_P2__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_LOG_P3__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_LOG_P4__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_LOG_P5__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_LOG_P6__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_LOG_P7__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_LOG_P8__));
  _y = _mm256_mul_ps(_y, _x);
#endif

  _y = _mm256_mul_ps(_y, _z);
  
#if 1
  _y = _mm256_fmadd_ps(_e, _mm256_set1_ps(__CEPHES_LOG_Q1__), _y);
#else
  _tmp = _mm256_mul_ps(_e, _mm256_set1_ps(__CEPHES_LOG_Q1__));
  _y = _mm256_add_ps(_y, _tmp);
#endif

#if 1
  _y = _mm256_fmsub_ps(_z, _half, _y);
#else
  _tmp = _mm256_mul_ps(_z, _half);
  _y = _mm256_sub_ps(_y, _tmp);
#endif

#if 1
  _x = _mm256_sub_ps(_x, _y);
#else
  _x = _mm256_add_ps(_x, _y);
#endif

#if 1
  _x = _mm256_fmadd_ps(_e, _mm256_set1_ps(__CEPHES_LOG_Q2__), _x);
#else
  _tmp = _mm256_mul_ps(_e, _mm256_set1_ps(__CEPHES_LOG_Q2__));
  _x = _mm256_add_ps(_x, _tmp);
#endif
  _x = _mm256_or_ps(_x, _invalid_mask); // negative arg will be NAN
  return _x;

}

#define __EXP_HI__ (88.3762626647949f)
#define __EXP_LO__ (-88.3762626647949f)

#define __CEPHES_LOG2EF__ (1.44269504088896341)
#define __CEPHES_EXP_C1__ (0.693359375)
#define __CEPHES_EXP_C2__ (-2.12194440e-4)

#define __CEPHES_EXP_P0__ (1.9875691500E-4)
#define __CEPHES_EXP_P1__ (1.3981999507E-3)
#define __CEPHES_EXP_P2__ (8.3334519073E-3)
#define __CEPHES_EXP_P3__ (4.1665795894E-2)
#define __CEPHES_EXP_P4__ (1.6666665459E-1)
#define __CEPHES_EXP_P5__ (5.0000001201E-1)

__m256 exp256_ps(__m256 _x)
{
  __m256 _tmp = _mm256_setzero_ps();
  __m256 _one = _mm256_set1_ps(1.0f);
  __m256 _half = _mm256_set1_ps(0.5f);
  __m256i _i0;
  
  __m256 _fx;

  _x = _mm256_min_ps(_x, _mm256_set1_ps(__EXP_HI__));
  _x = _mm256_max_ps(_x, _mm256_set1_ps(__EXP_LO__));

  /* express exp(x) as exp(g + n*log(2)) */
  _fx = _mm256_mul_ps(_x, _mm256_set1_ps(__CEPHES_LOG2EF__));
  _fx = _mm256_add_ps(_fx, _half);

  _tmp = _mm256_floor_ps(_fx);
  
  /* if greater, subtract 1 */
  __m256 _mask = _mm256_cmp_ps(_tmp, _fx, _CMP_GT_OS);
  _mask = _mm256_and_ps(_mask, _one);
  _fx = _mm256_sub_ps(_tmp, _mask);

  _tmp = _mm256_mul_ps(_fx, _mm256_set1_ps(__CEPHES_EXP_C1__));
  __m256 _z = _mm256_mul_ps(_fx, _mm256_set1_ps(__CEPHES_EXP_C2__));
  _x = _mm256_sub_ps(_x, _tmp);
  _x = _mm256_sub_ps(_x, _z);

  _z = _mm256_mul_ps(_x, _x);
  
  __m256 _y = _mm256_set1_ps(__CEPHES_EXP_P0__);
#if 1
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_EXP_P1__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_EXP_P2__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_EXP_P3__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_EXP_P4__));
  _y = _mm256_fmadd_ps(_y, _x, _mm256_set1_ps(__CEPHES_EXP_P5__));
  _y = _mm256_fmadd_ps(_y, _z, _x);
  _y = _mm256_add_ps(_y, _one);
#else
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_EXP_P1__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_EXP_P2__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_EXP_P3__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_EXP_P4__));
  _y = _mm256_mul_ps(_y, _x);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__CEPHES_EXP_P5__));
  _y = _mm256_mul_ps(_y, _z);
  _y = _mm256_add_ps(_y, _x);
  _y = _mm256_add_ps(_y, _one);
#endif

  _i0 = _mm256_cvttps_epi32(_fx);
  _i0 = _mm256_add_epi32(_i0, _mm256_set1_epi32(0x7f));
  _i0 = _mm256_slli_epi32(_i0, 23);
  __m256 _pow2n = _mm256_castsi256_ps(_i0);
  _y = _mm256_mul_ps(_y, _pow2n);

  return _y;
}

__m256 cbrt256_ps(__m256 _x) 
{
  __m256 _one = _mm256_set1_ps(1.0);
  __m256 _one_third = _mm256_set1_ps(0.3333334);

  __m256i _bias = _mm256_set1_epi32(127);
  __m256i _exp = _mm256_srli_epi32(_mm256_castps_si256(_x), 23);
  __m256i _sgn = _mm256_srli_epi32(_exp, 8);
  __m256  _mant = _mm256_and_ps(_x, (__m256)_mm256_set1_epi32(0x7ffffff));
  _mant = _mm256_or_ps(_mant, (__m256)_mm256_slli_epi32(_bias, 23));
  __m256i _sgnmask = _mm256_set1_epi32(0xff);
  __m256 _ln2 = _mm256_set1_ps(0.69314718056);

  _mant = _mm256_sub_ps(_mant, _one);
  _mant = _mm256_mul_ps(_mant, _one_third);
  _mant = _mm256_add_ps(_mant, _one);

  _exp = _mm256_and_si256(_exp, _sgnmask);
  _exp = _mm256_sub_epi32(_exp, _bias);

  __m256 _fexp = _mm256_cvtepi32_ps(_exp);
  _fexp = _mm256_mul_ps(_fexp, _one_third);
  _exp = _mm256_cvtps_epi32(_mm256_floor_ps(_fexp));
  __m256 _diff_exp = _mm256_sub_ps(_fexp, _mm256_cvtepi32_ps(_exp));
  _exp = _mm256_add_epi32(_exp, _bias);
  _exp = _mm256_add_epi32(_exp, _mm256_slli_epi32(_sgn, 8));
  __m256 _init = _mm256_castsi256_ps(_mm256_slli_epi32(_exp, 23));

  __m256 _diff = _mm256_mul_ps(_mm256_mul_ps(_init, _ln2), _diff_exp);
  _init = _mm256_add_ps(_init, _diff);
  _init = _mm256_mul_ps(_init, _mant);

  //_init : initial value for the Newton-Raphson iteration

  __m256 _u2 = _mm256_mul_ps(_init, _init);
  __m256 _u3 = _mm256_fmsub_ps(_u2, _init, _x);
  _u3 = _mm256_mul_ps(_u3, _one_third);
#if 0
  _u3 = _mm256_div_ps(_u3, _u2);
#else
  _u3 = _mm256_mul_ps(_u3, _mm256_rcp_ps(_u2));
#endif
  _init = _mm256_sub_ps(_init, _u3);

  _u2 = _mm256_mul_ps(_init, _init);
  _u3 = _mm256_fmsub_ps(_u2, _init, _x);
  _u3 = _mm256_mul_ps(_u3, _one_third);
#if 0
  _u3 = _mm256_div_ps(_u3, _u2);
#else
  _u3 = _mm256_mul_ps(_u3, _mm256_rcp_ps(_u2));
#endif
  _init = _mm256_sub_ps(_init, _u3);

  _u2 = _mm256_mul_ps(_init, _init);
  _u3 = _mm256_fmsub_ps(_u2, _init, _x);
  _u3 = _mm256_mul_ps(_u3, _one_third);
#if 0
  _u3 = _mm256_div_ps(_u3, _u2);
#else
  _u3 = _mm256_mul_ps(_u3, _mm256_rcp_ps(_u2));
#endif
  _x = _mm256_sub_ps(_init, _u3);

  return _x;
}

#define __MINUS_CEPHES_DP1__ (-0.78515625)
#define __MINUS_CEPHES_DP2__ (-2.4187564849853515625e-4)
#define __MINUS_CEPHES_DP3__ (-3.77489497744594108e-8)
#define __SINCOF_P0__        (-1.9515295891E-4)
#define __SINCOF_P1__        (8.3321608736E-3)
#define __SINCOF_P2__        (-1.6666654611E-1)
#define __COSCOF_P0__        (2.443315711809948E-005)
#define __COSCOF_P1__        (-1.388731625493765E-003)
#define __COSCOF_P2__        (4.166664568298827E-002)
#define __CEPHES_FOPI__      (1.27323954473516) // 4 / M_PI


__m256 sin256_ps(__m256 _x) {

  __m256 _sign_bit = _x;

  // take an absolute value
  _x = _mm256_and_ps(_x, (__m256)_mm256_set1_epi32(__INV_SIGN_MASK__));
  // extract the sign bit (upper one) 
  _sign_bit = _mm256_and_ps(_sign_bit, (__m256)_mm256_set1_epi32(__SIGN_MASK__));

  // scale by 4/PI
  __m256 _y = _mm256_mul_ps(_x, _mm256_set1_ps(__CEPHES_FOPI__));

  // store the integer part of y
  __m256i _imm2 = _mm256_cvttps_epi32(_y);
  _imm2 = _mm256_add_epi32(_imm2, _mm256_set1_epi32(1));
  _imm2 = _mm256_and_si256(_imm2, _mm256_set1_epi32(~1));
  _y = _mm256_cvtepi32_ps(_imm2);

  // get the swap sign flag
  __m256i _imm0 = _mm256_and_si256(_imm2, _mm256_set1_epi32(4));
  _imm0 = _mm256_slli_epi32(_imm0, 29);

  /* get the polynom selection mask 
     there is one polynom for 0 <= x <= Pi/4
     and another one for Pi/4<x<=Pi/2

     Both branches will be computed.
  */
  _imm2 = _mm256_and_si256(_imm2, _mm256_set1_epi32(2));//*(v8si*)_pi32_256_2);
  _imm2 = _mm256_cmpeq_epi32(_imm2,_mm256_setzero_si256());//*(v8si*)_pi32_256_0);

  __m256 _swap_sign_bit = _mm256_castsi256_ps(_imm0);
  __m256 _poly_mask = _mm256_castsi256_ps(_imm2);
  _sign_bit = _mm256_xor_ps(_sign_bit, _swap_sign_bit);

  // The magic pass: "Extended precision modular arithmetic" 
  // x = ((x - y * DP1) - y * DP2) - y * DP3;
  __m256 _ymm1 = _mm256_set1_ps(__MINUS_CEPHES_DP1__);
  __m256 _ymm2 = _mm256_set1_ps(__MINUS_CEPHES_DP2__);
  __m256 _ymm3 = _mm256_set1_ps(__MINUS_CEPHES_DP3__);
#if 1
  _x = _mm256_fmadd_ps(_y, _ymm1, _x);
  _x = _mm256_fmadd_ps(_y, _ymm2, _x);
  _x = _mm256_fmadd_ps(_y, _ymm3, _x);
#else
  _ymm1 = _mm256_mul_ps(_y, _ymm1);
  _ymm2 = _mm256_mul_ps(_y, _ymm2);
  _ymm3 = _mm256_mul_ps(_y, _ymm3);
  _x = _mm256_add_ps(_x, _ymm1);
  _x = _mm256_add_ps(_x, _ymm2);
  _x = _mm256_add_ps(_x, _ymm3);
#endif

  // evaluate the first polynom ( 0 <= x <= PI/4 )
  _y = _mm256_set1_ps(__COSCOF_P0__);
  __m256 _z = _mm256_mul_ps(_x, _x);

#if 1
  _y = _mm256_fmadd_ps(_y, _z, _mm256_set1_ps(__COSCOF_P1__));
  _y = _mm256_fmadd_ps(_y, _z, _mm256_set1_ps(__COSCOF_P2__));
  _y = _mm256_mul_ps(_y, _z);
  _y = _mm256_mul_ps(_y, _z);
  _y = _mm256_fmsub_ps(_z, _mm256_set1_ps(0.5f), _y);
  _y = _mm256_sub_ps(_mm256_set1_ps(1.0f), _y);
#else
  _y = _mm256_mul_ps(_y, _z);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__COSCOF_P1__));
  _y = _mm256_mul_ps(_y, _z);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(__COSCOF_P2__));
  _y = _mm256_mul_ps(_y, _z);
  _y = _mm256_mul_ps(_y, _z);
  __m256 _tmp = _mm256_mul_ps(_z, _mm256_set1_ps(0.5f));
  _y = _mm256_sub_ps(_y, _tmp);
  _y = _mm256_add_ps(_y, _mm256_set1_ps(1.0f));
#endif

  // evaluate the second polynom ( PI/4 <= x )
  __m256 _y2 = _mm256_set1_ps(__SINCOF_P0__);
  _y2 = _mm256_fmadd_ps(_y2, _z, _mm256_set1_ps(__SINCOF_P1__));
  _y2 = _mm256_fmadd_ps(_y2, _z, _mm256_set1_ps(__SINCOF_P2__));
  _y2 = _mm256_mul_ps(_y2, _z);
  _y2 = _mm256_fmadd_ps(_y2, _x, _x);

  // select the correct result from the two polynoms
  _ymm3 = _poly_mask;
  _y2 = _mm256_and_ps(_ymm3, _y2);
  _y  = _mm256_andnot_ps(_ymm3, _y);
  _y  = _mm256_add_ps(_y, _y2);
  // update the sign
  _y = _mm256_xor_ps(_y, _sign_bit);

  return _y;
  
}

__m256 cos256_ps(__m256 _x)
{
  // take an absolute value 
  _x = _mm256_and_ps(_x, (__m256)_mm256_set1_epi32(__INV_SIGN_MASK__));

  // scale by 4/PI
  __m256 _y = _mm256_mul_ps(_x, _mm256_set1_ps(__CEPHES_FOPI__));

  // store the integer part of _y 
  __m256i _imm2 = _mm256_cvttps_epi32(_y);
  _imm2 = _mm256_add_epi32(_imm2, _mm256_set1_epi32(1));
  _imm2 = _mm256_and_si256(_imm2, _mm256_set1_epi32(~1));
  _y = _mm256_cvtepi32_ps(_imm2);
  _imm2 = _mm256_sub_epi32(_imm2, _mm256_set1_epi32(2));

  // get the swap sign flag
  __m256i _imm0 = _mm256_andnot_si256(_imm2, _mm256_set1_epi32(4));
  _imm0 = _mm256_slli_epi32(_imm0, 29);

  // get the polynom selection mask
  _imm2 = _mm256_and_si256(_imm2, _mm256_set1_epi32(2));
  _imm2 = _mm256_cmpeq_epi32(_imm2, _mm256_setzero_si256());

  __m256 _sign_bit = _mm256_castsi256_ps(_imm0);
  __m256 _poly_mask = _mm256_castsi256_ps(_imm2);

  // The magic pass: "Extended precision modular arithmetic"
  // x = ((x - y * DP1) - y * DP2) - y * DP3;
  __m256 _ymm1 = _mm256_set1_ps(__MINUS_CEPHES_DP1__);
  __m256 _ymm2 = _mm256_set1_ps(__MINUS_CEPHES_DP2__);
  __m256 _ymm3 = _mm256_set1_ps(__MINUS_CEPHES_DP3__);

  _x = _mm256_fmadd_ps(_y, _ymm1, _x);
  _x = _mm256_fmadd_ps(_y, _ymm2, _x);
  _x = _mm256_fmadd_ps(_y, _ymm3, _x);

  // evaluate the first polynom ( 0 <= x <= PI/4 )
  _y = _mm256_set1_ps(__COSCOF_P0__);
  __m256 _z = _mm256_mul_ps(_x, _x);
  
  _y = _mm256_fmadd_ps(_y, _z, _mm256_set1_ps(__COSCOF_P1__));
  _y = _mm256_fmadd_ps(_y, _z, _mm256_set1_ps(__COSCOF_P2__));
  _y = _mm256_mul_ps(_y, _z);
  _y = _mm256_mul_ps(_y, _z);
  _y = _mm256_fmsub_ps(_z, _mm256_set1_ps(0.5f), _y);
  _y = _mm256_sub_ps(_mm256_set1_ps(1.0f), _y);

  // evaluate the second polynom ( PI/4 <= x )
  __m256 _y2 = _mm256_set1_ps(__SINCOF_P0__);
  _y2 = _mm256_fmadd_ps(_y2, _z, _mm256_set1_ps(__SINCOF_P1__));
  _y2 = _mm256_fmadd_ps(_y2, _z, _mm256_set1_ps(__SINCOF_P2__));
  _y2 = _mm256_mul_ps(_y2, _z);
  _y2 = _mm256_fmadd_ps(_y2, _x, _x);

  // select the correct result from two polynoms
  _ymm3 = _poly_mask;
  _y2 = _mm256_and_ps(_ymm3, _y2);
  _y = _mm256_andnot_ps(_ymm3, _y);
  _y = _mm256_add_ps(_y, _y2);
  // update the sign
  _y = _mm256_xor_ps(_y, _sign_bit);

  return _y;
}

void sincos256_ps(__m256 _x, __m256 *_s, __m256 *_c)
{
  __m256 _sign_bit_sin = _x;
  // take an absolute value 
  _x = _mm256_and_ps(_x, (__m256)_mm256_set1_epi32(__INV_SIGN_MASK__));
  // extract the sign bit (upper one)
  _sign_bit_sin = _mm256_and_ps(_sign_bit_sin, (__m256)_mm256_set1_epi32(__SIGN_MASK__));

  // scale by 4/PI
  __m256 _y  = _mm256_mul_ps(_x, _mm256_set1_ps(__CEPHES_FOPI__));

  // store the integer part of _y in 
  __m256i _imm2 = _mm256_cvttps_epi32(_y);
  _imm2 = _mm256_add_epi32(_imm2, _mm256_set1_epi32(1));
  _imm2 = _mm256_and_si256(_imm2, _mm256_set1_epi32(~1));
  _y = _mm256_cvtepi32_ps(_imm2);
  __m256i _imm4 = _imm2;

  // get the swap sign flag
  __m256i _imm0 = _mm256_and_si256(_imm2, _mm256_set1_epi32(4));
  _imm0 = _mm256_slli_epi32(_imm0, 29);

  // get the polynom selection mask for the sine 
  _imm2 = _mm256_and_si256(_imm2, _mm256_set1_epi32(2));
  _imm2 = _mm256_cmpeq_epi32(_imm2, _mm256_setzero_si256());

  __m256 _swap_sign_bit_sin = _mm256_castsi256_ps(_imm0);
  __m256 _poly_mask = _mm256_castsi256_ps(_imm2);

  // The magic pass: "Extended precision modular arithmetic" 
  // x = ((x - y * DP1) - y * DP2) - y * DP3;
  __m256 _ymm1 = _mm256_set1_ps(__MINUS_CEPHES_DP1__);
  __m256 _ymm2 = _mm256_set1_ps(__MINUS_CEPHES_DP2__);
  __m256 _ymm3 = _mm256_set1_ps(__MINUS_CEPHES_DP3__);
  _x = _mm256_fmadd_ps(_y, _ymm1, _x);
  _x = _mm256_fmadd_ps(_y, _ymm2, _x);
  _x = _mm256_fmadd_ps(_y, _ymm3, _x);

  _imm4 = _mm256_sub_epi32(_imm4, _mm256_set1_epi32(2));
  _imm4 = _mm256_andnot_si256(_imm4, _mm256_set1_epi32(4));
  _imm4 = _mm256_slli_epi32(_imm4, 29);

  __m256 _sign_bit_cos = _mm256_castsi256_ps(_imm4);
  _sign_bit_sin = _mm256_xor_ps(_sign_bit_sin, _swap_sign_bit_sin);
  
  // evaluate the first polynom (0 <= x < PI/4)
  __m256 _z = _mm256_mul_ps(_x, _x);
  _y = _mm256_set1_ps(__COSCOF_P0__);

  _y = _mm256_fmadd_ps(_y, _z, _mm256_set1_ps(__COSCOF_P1__));
  _y = _mm256_fmadd_ps(_y, _z, _mm256_set1_ps(__COSCOF_P2__));
  _y = _mm256_mul_ps(_y, _z);
  _y = _mm256_mul_ps(_y, _z);
  _y = _mm256_fmsub_ps(_z, _mm256_set1_ps(0.5f), _y);
  _y = _mm256_sub_ps(_mm256_set1_ps(1.0f), _y);

  // evaluate the second polynom ( PI/4 <= x )
  __m256 _y2 = _mm256_set1_ps(__SINCOF_P0__);
  _y2 = _mm256_fmadd_ps(_y2, _z, _mm256_set1_ps(__SINCOF_P1__));
  _y2 = _mm256_fmadd_ps(_y2, _z, _mm256_set1_ps(__SINCOF_P2__));
  _y2 = _mm256_mul_ps(_y2, _z);
  _y2 = _mm256_fmadd_ps(_y2, _x, _x);

  // select the correct result from the two polynoms 
  _ymm3 = _poly_mask;
  __m256 _ysin2 = _mm256_and_ps(_ymm3, _y2);
  __m256 _ysin1 = _mm256_andnot_ps(_ymm3, _y);
  _y2 = _mm256_sub_ps(_y2, _ysin2);
  _y  = _mm256_sub_ps(_y, _ysin1);

  _ymm1 = _mm256_add_ps(_ysin1, _ysin2);
  _ymm2 = _mm256_add_ps(_y, _y2);

  *_s = _mm256_xor_ps(_ymm1, _sign_bit_sin);
  *_c = _mm256_xor_ps(_ymm2, _sign_bit_cos);
  
}
