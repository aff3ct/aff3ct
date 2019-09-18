#ifdef __SSE4_1__

#include <sstream>
#include <xmmintrin.h>
#include <immintrin.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Transpose/transpose_SSE.h"

using namespace aff3ct;
using namespace aff3ct::tools;

//
// TRANPOSITION DE MATRICE DE TAILLE Nx4
//
void aff3ct::tools::sse_trans_float(float *A, float *B, int n)
{
	int i = n/4;
	while( i-- ){
		__m128 row1 = _mm_load_ps(A        );
		__m128 row2 = _mm_load_ps(A +     n);
		__m128 row3 = _mm_load_ps(A + 2 * n);
		__m128 row4 = _mm_load_ps(A + 3 * n);
		_MM_TRANSPOSE4_PS(row1, row2, row3, row4);
		_mm_store_ps(B     , row1);
		_mm_store_ps(B +  4, row2);
		_mm_store_ps(B +  8, row3);
		_mm_store_ps(B + 12, row4);
		A += 4;
		B += 16;
	}
}

//
// TRANPOSITION DE MATRICE DE TAILLE 4xN
//
void aff3ct::tools::sse_itrans_float(float *A, float *B, int n)
{
	int i = n/4;
	while( i-- ){
		__m128 row1 = _mm_load_ps(A     );
		__m128 row2 = _mm_load_ps(A +  4);
		__m128 row3 = _mm_load_ps(A +  8);
		__m128 row4 = _mm_load_ps(A + 12);
		_MM_TRANSPOSE4_PS(row1, row2, row3, row4);
		_mm_store_ps(B        , row1);
		_mm_store_ps(B +     n, row2);
		_mm_store_ps(B + 2 * n, row3);
		_mm_store_ps(B + 3 * n, row4);
		A += 16;
		B += 4;
	}
}

#define LOAD_SIMD_FX    _mm_load_si128
#define STORE_SIMD_FX   _mm_store_si128

void aff3ct::tools::uchar_transpose_sse(const __m128i *src, __m128i *dst, int n)
{
	if (n % 16)
	{
		std::stringstream message;
		message << "'n' has to be divisible by 16 ('n' = " << n << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const int N = n /16; // NOMBRE DE PAQUET (128 bits) PAR TRAME
	__m128i *p_input = const_cast<__m128i*>(src);
	__m128i *p_output = dst;
	int loop = N;

	while( loop-- )
	{
		__m128i *copy_p_input  = p_input;

		// STAGE 2 DU BUTTERFLY
		__m128i a             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i b             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i a_0_7_b_0_7   = _mm_unpacklo_epi8(a, b);
		__m128i a_8_15_b_8_15 = _mm_unpackhi_epi8(a, b);

		__m128i c             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i d             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i c_0_7_d_0_7   = _mm_unpacklo_epi8(c, d);
		__m128i c_8_15_d_8_15 = _mm_unpackhi_epi8(c, d);

		__m128i a_0_3_b_0_3_c_0_3_d_0_3         = _mm_unpacklo_epi16(a_0_7_b_0_7, c_0_7_d_0_7);
		__m128i a_4_7_b_4_7_c_4_7_d_4_7         = _mm_unpackhi_epi16(a_0_7_b_0_7, c_0_7_d_0_7);
		__m128i a_8_11_b_8_11_c_8_11_d_8_11     = _mm_unpacklo_epi16(a_8_15_b_8_15, c_8_15_d_8_15);
		__m128i a_12_15_b_12_15_c_12_15_d_12_15 = _mm_unpackhi_epi16(a_8_15_b_8_15, c_8_15_d_8_15);

		__m128i e             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i f             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i e_0_7_f_0_7   = _mm_unpacklo_epi8(e, f);
		__m128i e_8_15_f_8_15 = _mm_unpackhi_epi8(e, f);

		__m128i g             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i h             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i g_0_7_h_0_7   = _mm_unpacklo_epi8(g, h);
		__m128i g_8_15_h_8_15 = _mm_unpackhi_epi8(g, h);

		__m128i e_0_3_f_0_3_g_0_3_h_0_3         = _mm_unpacklo_epi16(e_0_7_f_0_7, g_0_7_h_0_7);
		__m128i e_4_7_f_4_7_g_4_7_h_4_7         = _mm_unpackhi_epi16(e_0_7_f_0_7, g_0_7_h_0_7);
		__m128i e_8_11_f_8_11_g_8_11_h_8_11     = _mm_unpacklo_epi16(e_8_15_f_8_15, g_8_15_h_8_15);
		__m128i e_12_15_f_12_15_g_12_15_h_12_15 = _mm_unpackhi_epi16(e_8_15_f_8_15, g_8_15_h_8_15);

		__m128i i             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i j             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i i_0_7_j_0_7   = _mm_unpacklo_epi8(i, j);
		__m128i i_8_15_j_8_15 = _mm_unpackhi_epi8(i, j);

		__m128i k             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i l             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i k_0_7_l_0_7   = _mm_unpacklo_epi8(k, l);
		__m128i k_8_15_l_8_15 = _mm_unpackhi_epi8(k, l);

		__m128i i_0_3_j_0_3_k_0_3_l_0_3         = _mm_unpacklo_epi16(i_0_7_j_0_7, k_0_7_l_0_7);
		__m128i i_4_7_j_4_7_k_4_7_l_4_7         = _mm_unpackhi_epi16(i_0_7_j_0_7, k_0_7_l_0_7);
		__m128i i_8_11_j_8_11_k_8_11_l_8_11     = _mm_unpacklo_epi16(i_8_15_j_8_15, k_8_15_l_8_15);
		__m128i i_12_15_j_12_15_k_12_15_l_12_15 = _mm_unpackhi_epi16(i_8_15_j_8_15, k_8_15_l_8_15);

		__m128i m             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i n             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i m_0_7_n_0_7   = _mm_unpacklo_epi8(m, n);
		__m128i m_8_15_n_8_15 = _mm_unpackhi_epi8(m, n);

		__m128i o             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i p             = LOAD_SIMD_FX(copy_p_input); copy_p_input += N;
		__m128i o_0_7_p_0_7   = _mm_unpacklo_epi8(o, p);
		__m128i o_8_15_p_8_15 = _mm_unpackhi_epi8(o, p);

		__m128i m_0_3_n_0_3_o_0_3_p_0_3         = _mm_unpacklo_epi16(m_0_7_n_0_7, o_0_7_p_0_7);
		__m128i m_4_7_n_4_7_o_4_7_p_4_7         = _mm_unpackhi_epi16(m_0_7_n_0_7, o_0_7_p_0_7);
		__m128i m_8_11_n_8_11_o_8_11_p_8_11     = _mm_unpacklo_epi16(m_8_15_n_8_15, o_8_15_p_8_15);
		__m128i m_12_15_n_12_15_o_12_15_p_12_15 = _mm_unpackhi_epi16(m_8_15_n_8_15, o_8_15_p_8_15);


		// STAGE 4 DU BUTTERFLY
		__m128i a_0_1_b_0_1_c_0_1_d_0_1_e_0_1_f_0_1_g_0_1_h_0_1                 = _mm_unpacklo_epi32(a_0_3_b_0_3_c_0_3_d_0_3, e_0_3_f_0_3_g_0_3_h_0_3);
		__m128i i_0_1_j_0_1_k_0_1_l_0_1_m_0_1_n_0_1_o_0_1_p_0_1                 = _mm_unpacklo_epi32(i_0_3_j_0_3_k_0_3_l_0_3, m_0_3_n_0_3_o_0_3_p_0_3);

		STORE_SIMD_FX(p_output++, _mm_unpacklo_epi64(a_0_1_b_0_1_c_0_1_d_0_1_e_0_1_f_0_1_g_0_1_h_0_1, i_0_1_j_0_1_k_0_1_l_0_1_m_0_1_n_0_1_o_0_1_p_0_1));
		STORE_SIMD_FX(p_output++, _mm_unpackhi_epi64(a_0_1_b_0_1_c_0_1_d_0_1_e_0_1_f_0_1_g_0_1_h_0_1, i_0_1_j_0_1_k_0_1_l_0_1_m_0_1_n_0_1_o_0_1_p_0_1));

		__m128i a_2_3_b_2_3_c_2_3_d_2_3_e_2_3_f_2_3_g_2_3_h_2_3                 = _mm_unpackhi_epi32(a_0_3_b_0_3_c_0_3_d_0_3, e_0_3_f_0_3_g_0_3_h_0_3);
		__m128i i_2_3_j_2_3_k_2_3_l_2_3_m_2_3_n_2_3_o_2_3_p_2_3                 = _mm_unpackhi_epi32(i_0_3_j_0_3_k_0_3_l_0_3, m_0_3_n_0_3_o_0_3_p_0_3);

		STORE_SIMD_FX(p_output++, _mm_unpacklo_epi64(a_2_3_b_2_3_c_2_3_d_2_3_e_2_3_f_2_3_g_2_3_h_2_3, i_2_3_j_2_3_k_2_3_l_2_3_m_2_3_n_2_3_o_2_3_p_2_3));
		STORE_SIMD_FX(p_output++, _mm_unpackhi_epi64(a_2_3_b_2_3_c_2_3_d_2_3_e_2_3_f_2_3_g_2_3_h_2_3, i_2_3_j_2_3_k_2_3_l_2_3_m_2_3_n_2_3_o_2_3_p_2_3));

		__m128i a_4_5_b_4_5_c_4_5_d_4_5_e_4_5_f_4_5_g_4_5_h_4_5                 = _mm_unpacklo_epi32(a_4_7_b_4_7_c_4_7_d_4_7,         e_4_7_f_4_7_g_4_7_h_4_7);
		__m128i i_4_5_j_4_5_k_4_5_l_4_5_m_4_5_n_4_5_o_4_5_p_4_5                 = _mm_unpacklo_epi32(i_4_7_j_4_7_k_4_7_l_4_7, m_4_7_n_4_7_o_4_7_p_4_7);

		STORE_SIMD_FX(p_output++, _mm_unpacklo_epi64(a_4_5_b_4_5_c_4_5_d_4_5_e_4_5_f_4_5_g_4_5_h_4_5, i_4_5_j_4_5_k_4_5_l_4_5_m_4_5_n_4_5_o_4_5_p_4_5));
		STORE_SIMD_FX(p_output++, _mm_unpackhi_epi64(a_4_5_b_4_5_c_4_5_d_4_5_e_4_5_f_4_5_g_4_5_h_4_5, i_4_5_j_4_5_k_4_5_l_4_5_m_4_5_n_4_5_o_4_5_p_4_5));

		__m128i a_6_7_b_6_7_c_6_7_d_6_7_e_6_7_f_6_7_g_6_7_h_6_7                 = _mm_unpackhi_epi32(a_4_7_b_4_7_c_4_7_d_4_7,         e_4_7_f_4_7_g_4_7_h_4_7);
		__m128i i_6_7_j_6_7_k_6_7_l_6_7_m_6_7_n_6_7_o_6_7_p_6_7                 = _mm_unpackhi_epi32(i_4_7_j_4_7_k_4_7_l_4_7, m_4_7_n_4_7_o_4_7_p_4_7);

		STORE_SIMD_FX(p_output++, _mm_unpacklo_epi64(a_6_7_b_6_7_c_6_7_d_6_7_e_6_7_f_6_7_g_6_7_h_6_7, i_6_7_j_6_7_k_6_7_l_6_7_m_6_7_n_6_7_o_6_7_p_6_7));
		STORE_SIMD_FX(p_output++, _mm_unpackhi_epi64(a_6_7_b_6_7_c_6_7_d_6_7_e_6_7_f_6_7_g_6_7_h_6_7, i_6_7_j_6_7_k_6_7_l_6_7_m_6_7_n_6_7_o_6_7_p_6_7));

		__m128i a_8_10_b_8_10_c_8_10_d_8_10_e_8_10_f_8_10_g_8_10_h_8_10         = _mm_unpacklo_epi32(a_8_11_b_8_11_c_8_11_d_8_11,     e_8_11_f_8_11_g_8_11_h_8_11);
		__m128i i_8_10_j_8_10_k_8_10_l_8_10_m_8_10_n_8_10_o_8_10_p_8_10         = _mm_unpacklo_epi32(i_8_11_j_8_11_k_8_11_l_8_11,     m_8_11_n_8_11_o_8_11_p_8_11);

		STORE_SIMD_FX(p_output++, _mm_unpacklo_epi64(a_8_10_b_8_10_c_8_10_d_8_10_e_8_10_f_8_10_g_8_10_h_8_10,         i_8_10_j_8_10_k_8_10_l_8_10_m_8_10_n_8_10_o_8_10_p_8_10));
		STORE_SIMD_FX(p_output++, _mm_unpackhi_epi64(a_8_10_b_8_10_c_8_10_d_8_10_e_8_10_f_8_10_g_8_10_h_8_10,         i_8_10_j_8_10_k_8_10_l_8_10_m_8_10_n_8_10_o_8_10_p_8_10));

		__m128i i_10_11_j_10_11_k_10_11_l_10_11_m_10_11_n_10_11_o_10_11_p_10_11 = _mm_unpackhi_epi32(i_8_11_j_8_11_k_8_11_l_8_11,     m_8_11_n_8_11_o_8_11_p_8_11);
		__m128i a_10_11_b_10_11_c_10_11_d_10_11_e_10_11_f_10_11_g_10_11_h_10_11 = _mm_unpackhi_epi32(a_8_11_b_8_11_c_8_11_d_8_11,     e_8_11_f_8_11_g_8_11_h_8_11);

		STORE_SIMD_FX(p_output++, _mm_unpacklo_epi64(a_10_11_b_10_11_c_10_11_d_10_11_e_10_11_f_10_11_g_10_11_h_10_11, i_10_11_j_10_11_k_10_11_l_10_11_m_10_11_n_10_11_o_10_11_p_10_11));
		STORE_SIMD_FX(p_output++, _mm_unpackhi_epi64(a_10_11_b_10_11_c_10_11_d_10_11_e_10_11_f_10_11_g_10_11_h_10_11, i_10_11_j_10_11_k_10_11_l_10_11_m_10_11_n_10_11_o_10_11_p_10_11));

		__m128i i_12_13_j_12_13_k_12_13_l_12_13_m_12_13_n_12_13_o_12_13_p_12_13 = _mm_unpacklo_epi32(i_12_15_j_12_15_k_12_15_l_12_15, m_12_15_n_12_15_o_12_15_p_12_15);
		__m128i a_12_13_b_12_13_c_12_13_d_12_13_e_12_13_f_12_13_g_12_13_h_12_13 = _mm_unpacklo_epi32(a_12_15_b_12_15_c_12_15_d_12_15, e_12_15_f_12_15_g_12_15_h_12_15);

		STORE_SIMD_FX(p_output++, _mm_unpacklo_epi64(a_12_13_b_12_13_c_12_13_d_12_13_e_12_13_f_12_13_g_12_13_h_12_13, i_12_13_j_12_13_k_12_13_l_12_13_m_12_13_n_12_13_o_12_13_p_12_13));
		STORE_SIMD_FX(p_output++, _mm_unpackhi_epi64(a_12_13_b_12_13_c_12_13_d_12_13_e_12_13_f_12_13_g_12_13_h_12_13, i_12_13_j_12_13_k_12_13_l_12_13_m_12_13_n_12_13_o_12_13_p_12_13));

		__m128i i_14_15_j_14_15_k_14_15_l_14_15_m_14_15_n_14_15_o_14_15_p_14_15 = _mm_unpackhi_epi32(i_12_15_j_12_15_k_12_15_l_12_15, m_12_15_n_12_15_o_12_15_p_12_15);
		__m128i a_14_15_b_14_15_c_14_15_d_14_15_e_14_15_f_14_15_g_14_15_h_14_15 = _mm_unpackhi_epi32(a_12_15_b_12_15_c_12_15_d_12_15, e_12_15_f_12_15_g_12_15_h_12_15);

		STORE_SIMD_FX(p_output++, _mm_unpacklo_epi64(a_14_15_b_14_15_c_14_15_d_14_15_e_14_15_f_14_15_g_14_15_h_14_15, i_14_15_j_14_15_k_14_15_l_14_15_m_14_15_n_14_15_o_14_15_p_14_15));
		STORE_SIMD_FX(p_output++, _mm_unpackhi_epi64(a_14_15_b_14_15_c_14_15_d_14_15_e_14_15_f_14_15_g_14_15_h_14_15, i_14_15_j_14_15_k_14_15_l_14_15_m_14_15_n_14_15_o_14_15_p_14_15));
		p_input  += 1;
	}
}

#define LOAD_SIMD_FX    _mm_load_si128
#define STORE_SIMD_FX   _mm_store_si128

void aff3ct::tools::uchar_itranspose_sse(const __m128i *src, __m128i *dst, int n)
{
	if (n % 16)
	{
		std::stringstream message;
		message << "'n' has to be divisible by 16 ('n' = " << n << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const int N = n /16; // NOMBRE DE PAQUET (128 bits) PAR TRAME
	__m128i *p_input  = const_cast<__m128i*>(src);
	__m128i *p_output = dst;
	int loop = N;

	while( loop-- )
	{
		// STAGE 2 DU BUTTERFLY
		__m128i a             = LOAD_SIMD_FX(p_input);
		__m128i b             = LOAD_SIMD_FX(p_input + 1);
		__m128i a_0_7_b_0_7   = _mm_unpacklo_epi8(a, b);
		__m128i a_8_15_b_8_15 = _mm_unpackhi_epi8(a, b);

		__m128i c             = LOAD_SIMD_FX(p_input + 2);
		__m128i d             = LOAD_SIMD_FX(p_input + 3);
		__m128i c_0_7_d_0_7   = _mm_unpacklo_epi8(c, d);
		__m128i c_8_15_d_8_15 = _mm_unpackhi_epi8(c, d);

		__m128i a_0_3_b_0_3_c_0_3_d_0_3         = _mm_unpacklo_epi16(a_0_7_b_0_7, c_0_7_d_0_7);
		__m128i a_4_7_b_4_7_c_4_7_d_4_7         = _mm_unpackhi_epi16(a_0_7_b_0_7, c_0_7_d_0_7);
		__m128i a_8_11_b_8_11_c_8_11_d_8_11     = _mm_unpacklo_epi16(a_8_15_b_8_15, c_8_15_d_8_15);
		__m128i a_12_15_b_12_15_c_12_15_d_12_15 = _mm_unpackhi_epi16(a_8_15_b_8_15, c_8_15_d_8_15);

		__m128i e             = LOAD_SIMD_FX(p_input + 4);
		__m128i f             = LOAD_SIMD_FX(p_input + 5);
		__m128i e_0_7_f_0_7   = _mm_unpacklo_epi8(e, f);
		__m128i e_8_15_f_8_15 = _mm_unpackhi_epi8(e, f);

		__m128i g             = LOAD_SIMD_FX(p_input + 6);
		__m128i h             = LOAD_SIMD_FX(p_input + 7);
		__m128i g_0_7_h_0_7   = _mm_unpacklo_epi8(g, h);
		__m128i g_8_15_h_8_15 = _mm_unpackhi_epi8(g, h);

		__m128i e_0_3_f_0_3_g_0_3_h_0_3         = _mm_unpacklo_epi16(e_0_7_f_0_7, g_0_7_h_0_7);
		__m128i e_4_7_f_4_7_g_4_7_h_4_7         = _mm_unpackhi_epi16(e_0_7_f_0_7, g_0_7_h_0_7);
		__m128i e_8_11_f_8_11_g_8_11_h_8_11     = _mm_unpacklo_epi16(e_8_15_f_8_15, g_8_15_h_8_15);
		__m128i e_12_15_f_12_15_g_12_15_h_12_15 = _mm_unpackhi_epi16(e_8_15_f_8_15, g_8_15_h_8_15);

		__m128i i             = LOAD_SIMD_FX(p_input + 8);
		__m128i j             = LOAD_SIMD_FX(p_input + 9);
		__m128i i_0_7_j_0_7   = _mm_unpacklo_epi8(i, j);
		__m128i i_8_15_j_8_15 = _mm_unpackhi_epi8(i, j);

		__m128i k             = LOAD_SIMD_FX(p_input + 10);
		__m128i l             = LOAD_SIMD_FX(p_input + 11);
		__m128i k_0_7_l_0_7   = _mm_unpacklo_epi8(k, l);
		__m128i k_8_15_l_8_15 = _mm_unpackhi_epi8(k, l);

		__m128i i_0_3_j_0_3_k_0_3_l_0_3         = _mm_unpacklo_epi16(i_0_7_j_0_7, k_0_7_l_0_7);
		__m128i i_4_7_j_4_7_k_4_7_l_4_7         = _mm_unpackhi_epi16(i_0_7_j_0_7, k_0_7_l_0_7);
		__m128i i_8_11_j_8_11_k_8_11_l_8_11     = _mm_unpacklo_epi16(i_8_15_j_8_15, k_8_15_l_8_15);
		__m128i i_12_15_j_12_15_k_12_15_l_12_15 = _mm_unpackhi_epi16(i_8_15_j_8_15, k_8_15_l_8_15);

		__m128i m             = LOAD_SIMD_FX(p_input + 12);
		__m128i n             = LOAD_SIMD_FX(p_input + 13);
		__m128i m_0_7_n_0_7   = _mm_unpacklo_epi8(m, n);
		__m128i m_8_15_n_8_15 = _mm_unpackhi_epi8(m, n);

		__m128i o             = LOAD_SIMD_FX(p_input + 14);
		__m128i p             = LOAD_SIMD_FX(p_input + 15);
		__m128i o_0_7_p_0_7   = _mm_unpacklo_epi8(o, p);
		__m128i o_8_15_p_8_15 = _mm_unpackhi_epi8(o, p);

		__m128i m_0_3_n_0_3_o_0_3_p_0_3         = _mm_unpacklo_epi16(m_0_7_n_0_7, o_0_7_p_0_7);
		__m128i m_4_7_n_4_7_o_4_7_p_4_7         = _mm_unpackhi_epi16(m_0_7_n_0_7, o_0_7_p_0_7);
		__m128i m_8_11_n_8_11_o_8_11_p_8_11     = _mm_unpacklo_epi16(m_8_15_n_8_15, o_8_15_p_8_15);
		__m128i m_12_15_n_12_15_o_12_15_p_12_15 = _mm_unpackhi_epi16(m_8_15_n_8_15, o_8_15_p_8_15);


		// STAGE 4 DU BUTTERFLY
		__m128i a_0_1_b_0_1_c_0_1_d_0_1_e_0_1_f_0_1_g_0_1_h_0_1                 = _mm_unpacklo_epi32(a_0_3_b_0_3_c_0_3_d_0_3, e_0_3_f_0_3_g_0_3_h_0_3);
		__m128i i_0_1_j_0_1_k_0_1_l_0_1_m_0_1_n_0_1_o_0_1_p_0_1                 = _mm_unpacklo_epi32(i_0_3_j_0_3_k_0_3_l_0_3, m_0_3_n_0_3_o_0_3_p_0_3);

		__m128i* copy_p_output1  = p_output;
		__m128i* copy_p_output2  = p_output       + N;
		__m128i* copy_p_output3  = copy_p_output2 + N;
		__m128i* copy_p_output4  = copy_p_output3 + N;
		__m128i* copy_p_output5  = copy_p_output4 + N;
		__m128i* copy_p_output6  = copy_p_output5 + N;
		__m128i* copy_p_output7  = copy_p_output6 + N;
		__m128i* copy_p_output8  = copy_p_output7 + N;
		__m128i* copy_p_output9  = copy_p_output8 + N;
		__m128i* copy_p_output10 = copy_p_output9 + N;
		__m128i* copy_p_output11 = copy_p_output10 + N;
		__m128i* copy_p_output12 = copy_p_output11 + N;
		__m128i* copy_p_output13 = copy_p_output12 + N;
		__m128i* copy_p_output14 = copy_p_output13 + N;
		__m128i* copy_p_output15 = copy_p_output14 + N;
		__m128i* copy_p_output16 = copy_p_output15 + N;
		STORE_SIMD_FX(copy_p_output1, _mm_unpacklo_epi64(a_0_1_b_0_1_c_0_1_d_0_1_e_0_1_f_0_1_g_0_1_h_0_1, i_0_1_j_0_1_k_0_1_l_0_1_m_0_1_n_0_1_o_0_1_p_0_1));
		STORE_SIMD_FX(copy_p_output2, _mm_unpackhi_epi64(a_0_1_b_0_1_c_0_1_d_0_1_e_0_1_f_0_1_g_0_1_h_0_1, i_0_1_j_0_1_k_0_1_l_0_1_m_0_1_n_0_1_o_0_1_p_0_1));

		__m128i a_2_3_b_2_3_c_2_3_d_2_3_e_2_3_f_2_3_g_2_3_h_2_3                 = _mm_unpackhi_epi32(a_0_3_b_0_3_c_0_3_d_0_3, e_0_3_f_0_3_g_0_3_h_0_3);
		__m128i i_2_3_j_2_3_k_2_3_l_2_3_m_2_3_n_2_3_o_2_3_p_2_3                 = _mm_unpackhi_epi32(i_0_3_j_0_3_k_0_3_l_0_3, m_0_3_n_0_3_o_0_3_p_0_3);

		STORE_SIMD_FX(copy_p_output3, _mm_unpacklo_epi64(a_2_3_b_2_3_c_2_3_d_2_3_e_2_3_f_2_3_g_2_3_h_2_3, i_2_3_j_2_3_k_2_3_l_2_3_m_2_3_n_2_3_o_2_3_p_2_3));
		STORE_SIMD_FX(copy_p_output4, _mm_unpackhi_epi64(a_2_3_b_2_3_c_2_3_d_2_3_e_2_3_f_2_3_g_2_3_h_2_3, i_2_3_j_2_3_k_2_3_l_2_3_m_2_3_n_2_3_o_2_3_p_2_3));

		__m128i a_4_5_b_4_5_c_4_5_d_4_5_e_4_5_f_4_5_g_4_5_h_4_5                 = _mm_unpacklo_epi32(a_4_7_b_4_7_c_4_7_d_4_7,         e_4_7_f_4_7_g_4_7_h_4_7);
		__m128i i_4_5_j_4_5_k_4_5_l_4_5_m_4_5_n_4_5_o_4_5_p_4_5                 = _mm_unpacklo_epi32(i_4_7_j_4_7_k_4_7_l_4_7, m_4_7_n_4_7_o_4_7_p_4_7);

		STORE_SIMD_FX(copy_p_output5, _mm_unpacklo_epi64(a_4_5_b_4_5_c_4_5_d_4_5_e_4_5_f_4_5_g_4_5_h_4_5, i_4_5_j_4_5_k_4_5_l_4_5_m_4_5_n_4_5_o_4_5_p_4_5));
		STORE_SIMD_FX(copy_p_output6, _mm_unpackhi_epi64(a_4_5_b_4_5_c_4_5_d_4_5_e_4_5_f_4_5_g_4_5_h_4_5, i_4_5_j_4_5_k_4_5_l_4_5_m_4_5_n_4_5_o_4_5_p_4_5));

		__m128i a_6_7_b_6_7_c_6_7_d_6_7_e_6_7_f_6_7_g_6_7_h_6_7                 = _mm_unpackhi_epi32(a_4_7_b_4_7_c_4_7_d_4_7,         e_4_7_f_4_7_g_4_7_h_4_7);
		__m128i i_6_7_j_6_7_k_6_7_l_6_7_m_6_7_n_6_7_o_6_7_p_6_7                 = _mm_unpackhi_epi32(i_4_7_j_4_7_k_4_7_l_4_7, m_4_7_n_4_7_o_4_7_p_4_7);

		STORE_SIMD_FX(copy_p_output7, _mm_unpacklo_epi64(a_6_7_b_6_7_c_6_7_d_6_7_e_6_7_f_6_7_g_6_7_h_6_7, i_6_7_j_6_7_k_6_7_l_6_7_m_6_7_n_6_7_o_6_7_p_6_7));
		STORE_SIMD_FX(copy_p_output8, _mm_unpackhi_epi64(a_6_7_b_6_7_c_6_7_d_6_7_e_6_7_f_6_7_g_6_7_h_6_7, i_6_7_j_6_7_k_6_7_l_6_7_m_6_7_n_6_7_o_6_7_p_6_7));

		__m128i a_8_10_b_8_10_c_8_10_d_8_10_e_8_10_f_8_10_g_8_10_h_8_10         = _mm_unpacklo_epi32(a_8_11_b_8_11_c_8_11_d_8_11,     e_8_11_f_8_11_g_8_11_h_8_11);
		__m128i i_8_10_j_8_10_k_8_10_l_8_10_m_8_10_n_8_10_o_8_10_p_8_10         = _mm_unpacklo_epi32(i_8_11_j_8_11_k_8_11_l_8_11,     m_8_11_n_8_11_o_8_11_p_8_11);

		STORE_SIMD_FX(copy_p_output9, _mm_unpacklo_epi64(a_8_10_b_8_10_c_8_10_d_8_10_e_8_10_f_8_10_g_8_10_h_8_10,         i_8_10_j_8_10_k_8_10_l_8_10_m_8_10_n_8_10_o_8_10_p_8_10));
		STORE_SIMD_FX(copy_p_output10, _mm_unpackhi_epi64(a_8_10_b_8_10_c_8_10_d_8_10_e_8_10_f_8_10_g_8_10_h_8_10,         i_8_10_j_8_10_k_8_10_l_8_10_m_8_10_n_8_10_o_8_10_p_8_10));

		__m128i i_10_11_j_10_11_k_10_11_l_10_11_m_10_11_n_10_11_o_10_11_p_10_11 = _mm_unpackhi_epi32(i_8_11_j_8_11_k_8_11_l_8_11,     m_8_11_n_8_11_o_8_11_p_8_11);
		__m128i a_10_11_b_10_11_c_10_11_d_10_11_e_10_11_f_10_11_g_10_11_h_10_11 = _mm_unpackhi_epi32(a_8_11_b_8_11_c_8_11_d_8_11,     e_8_11_f_8_11_g_8_11_h_8_11);

		STORE_SIMD_FX(copy_p_output11, _mm_unpacklo_epi64(a_10_11_b_10_11_c_10_11_d_10_11_e_10_11_f_10_11_g_10_11_h_10_11, i_10_11_j_10_11_k_10_11_l_10_11_m_10_11_n_10_11_o_10_11_p_10_11));
		STORE_SIMD_FX(copy_p_output12, _mm_unpackhi_epi64(a_10_11_b_10_11_c_10_11_d_10_11_e_10_11_f_10_11_g_10_11_h_10_11, i_10_11_j_10_11_k_10_11_l_10_11_m_10_11_n_10_11_o_10_11_p_10_11));

		__m128i i_12_13_j_12_13_k_12_13_l_12_13_m_12_13_n_12_13_o_12_13_p_12_13 = _mm_unpacklo_epi32(i_12_15_j_12_15_k_12_15_l_12_15, m_12_15_n_12_15_o_12_15_p_12_15);
		__m128i a_12_13_b_12_13_c_12_13_d_12_13_e_12_13_f_12_13_g_12_13_h_12_13 = _mm_unpacklo_epi32(a_12_15_b_12_15_c_12_15_d_12_15, e_12_15_f_12_15_g_12_15_h_12_15);

		STORE_SIMD_FX(copy_p_output13, _mm_unpacklo_epi64(a_12_13_b_12_13_c_12_13_d_12_13_e_12_13_f_12_13_g_12_13_h_12_13, i_12_13_j_12_13_k_12_13_l_12_13_m_12_13_n_12_13_o_12_13_p_12_13));
		STORE_SIMD_FX(copy_p_output14, _mm_unpackhi_epi64(a_12_13_b_12_13_c_12_13_d_12_13_e_12_13_f_12_13_g_12_13_h_12_13, i_12_13_j_12_13_k_12_13_l_12_13_m_12_13_n_12_13_o_12_13_p_12_13));

		__m128i i_14_15_j_14_15_k_14_15_l_14_15_m_14_15_n_14_15_o_14_15_p_14_15 = _mm_unpackhi_epi32(i_12_15_j_12_15_k_12_15_l_12_15, m_12_15_n_12_15_o_12_15_p_12_15);
		__m128i a_14_15_b_14_15_c_14_15_d_14_15_e_14_15_f_14_15_g_14_15_h_14_15 = _mm_unpackhi_epi32(a_12_15_b_12_15_c_12_15_d_12_15, e_12_15_f_12_15_g_12_15_h_12_15);

		STORE_SIMD_FX(copy_p_output15, _mm_unpacklo_epi64(a_14_15_b_14_15_c_14_15_d_14_15_e_14_15_f_14_15_g_14_15_h_14_15, i_14_15_j_14_15_k_14_15_l_14_15_m_14_15_n_14_15_o_14_15_p_14_15));
		STORE_SIMD_FX(copy_p_output16, _mm_unpackhi_epi64(a_14_15_b_14_15_c_14_15_d_14_15_e_14_15_f_14_15_g_14_15_h_14_15, i_14_15_j_14_15_k_14_15_l_14_15_m_14_15_n_14_15_o_14_15_p_14_15));
		p_output += 1;
		p_input += 16;
	}
}

#endif
