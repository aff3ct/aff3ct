#include "mipp.h"

// -------------------------------------------------------------------------------------------------------- X86 AVX-512
// --------------------------------------------------------------------------------------------------------------------
#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)

	// ---------------------------------------------------------------------------------------------------------- loadu
#if defined(__AVX512F__)
	template <>
	inline reg loadu<float>(const float *mem_addr) {
		return _mm512_loadu_ps(mem_addr);
	}

	template <>
	inline reg loadu<double>(const double *mem_addr) {
		return (__m512) _mm512_loadu_pd(mem_addr);
	}

	template <>
	inline reg loadu<int>(const int *mem_addr) {
		return _mm512_loadu_ps((const float*) mem_addr);
	}

	template <>
	inline reg loadu<short>(const short *mem_addr) {
		return _mm512_loadu_ps((const float*) mem_addr);
	}

	template <>
	inline reg loadu<signed char>(const signed char *mem_addr) {
		return (__m512) _mm512_loadu_ps((const float*) mem_addr);
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- load
#ifdef MIPP_ALIGNED_LOADS
	template <>
	inline reg load<float>(const float *mem_addr) {
		return _mm512_load_ps(mem_addr);
	}

	template <>
	inline reg load<double>(const double *mem_addr) {
		return (__m512) _mm512_load_pd(mem_addr);
	}

	template <>
	inline reg load<int>(const int *mem_addr) {
		return _mm512_load_ps((const float*) mem_addr);
	}

	template <>
	inline reg load<short>(const short *mem_addr) {
		return _mm512_load_ps((const float*) mem_addr);
	}

	template <>
	inline reg load<signed char>(const signed char *mem_addr) {
		return (__m512) _mm512_load_ps((const float*) mem_addr);
	}
#else
	template <>
	inline reg load<float>(const float *mem_addr) {
		return mipp::loadu<float>(mem_addr);
	}

	template <>
	inline reg load<double>(const double *mem_addr) {
		return mipp::loadu<double>(mem_addr);
	}

	template <>
	inline reg load<long long>(const long long *mem_addr) {
		return mipp::loadu<long long>(mem_addr);
	}

	template <>
	inline reg load<int>(const int *mem_addr) {
		return mipp::loadu<int>(mem_addr);
	}

	template <>
	inline reg load<short>(const short *mem_addr) {
		return mipp::loadu<short>(mem_addr);
	}

	template <>
	inline reg load<signed char>(const signed char *mem_addr) {
		return mipp::loadu<signed char>(mem_addr);
	}
#endif

	// --------------------------------------------------------------------------------------------------------- storeu
#if defined(__AVX512F__)
	template <>
	inline void storeu<float>(float *mem_addr, const reg v) {
		_mm512_storeu_ps(mem_addr, v);
	}

	template <>
	inline void storeu<double>(double *mem_addr, const reg v) {
		_mm512_storeu_pd(mem_addr, (__m512d) v);
	}

	template <>
	inline void storeu<int>(int *mem_addr, const reg v) {
		_mm512_storeu_ps((float *)mem_addr, v);
	}

	template <>
	inline void storeu<short>(short *mem_addr, const reg v) {
		_mm512_storeu_ps((float *)mem_addr, v);
	}

	template <>
	inline void storeu<signed char>(signed char *mem_addr, const reg v) {
		_mm512_storeu_ps((float *)mem_addr, v);
	}
#endif

	// ---------------------------------------------------------------------------------------------------------- store
#ifdef MIPP_ALIGNED_LOADS
	template <>
	inline void store<float>(float *mem_addr, const reg v) {
		_mm512_store_ps(mem_addr, v);
	}

	template <>
	inline void store<double>(double *mem_addr, const reg v) {
		_mm512_store_pd(mem_addr, (__m512d) v);
	}

	template <>
	inline void store<int>(int *mem_addr, const reg v) {
		_mm512_store_ps((float *)mem_addr, v);
	}

	template <>
	inline void store<short>(short *mem_addr, const reg v) {
		_mm512_store_ps((float *)mem_addr, v);
	}

	template <>
	inline void store<signed char>(signed char *mem_addr, const reg v) {
		_mm512_store_ps((float *)mem_addr, v);
	}
#else
	template <>
	inline void store<float>(float *mem_addr, const reg v) {
		mipp::storeu<float>(mem_addr, v);
	}

	template <>
	inline void store<double>(double *mem_addr, const reg v) {
		mipp::storeu<double>(mem_addr, v);
	}

	template <>
	inline void store<long long>(long long *mem_addr, const reg v) {
		mipp::storeu<long long>(mem_addr, v);
	}

	template <>
	inline void store<int>(int *mem_addr, const reg v) {
		mipp::storeu<int>(mem_addr, v);
	}

	template <>
	inline void store<short>(short *mem_addr, const reg v) {
		mipp::storeu<short>(mem_addr, v);
	}

	template <>
	inline void store<signed char>(signed char *mem_addr, const reg v) {
		mipp::storeu<signed char>(mem_addr, v);
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- set1
#if defined(__AVX512F__)
	template <>
	inline reg set1<float>(const float val) {
		return _mm512_set1_ps(val);
	}

	template <>
	inline reg set1<double>(const double val) {
		return (__m512) _mm512_set1_pd(val);
	}

	template <>
	inline reg set1<int>(const int val) {
		return (reg) _mm512_set1_epi32(val);
	}

	template <>
	inline reg set1<long long>(const long long val) {
		return (reg) _mm512_set1_epi64(val);
	}

	template <>
	inline reg set1<short>(const short val) {
		return (reg) _mm512_set1_epi16(val);
	}

	template <>
	inline reg set1<signed char>(const signed char val) {
		return (reg) _mm512_set1_epi8(val);
	}

#elif defined(__MIC__) || defined(__KNCNI__)
	template <>
	inline reg set1<float>(const float val) {
		float init[16] __attribute__((aligned(64))) = {val, val, val, val, val, val, val, val, 
		                                               val, val, val, val, val, val, val, val};
		return load<float>(init);
	}

	template <>
	inline reg set1<double>(const double val) {
		double init[8] __attribute__((aligned(64))) = {val, val, val, val, val, val, val, val};
		return load<double>(init);
	}

	template <>
	inline reg set1<int>(const int val) {
		int init[16] __attribute__((aligned(64))) = {val, val, val, val, val, val, val, val, 
		                                             val, val, val, val, val, val, val, val};
		return load<int>(init);
	}

	template <>
	inline reg set1<long long>(const long long val) {
		long long init[8] __attribute__((aligned(64))) = {val, val, val, val, val, val, val, val};
		return load<long long>(init);
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- set0
#if defined(__AVX512F__)
	template <>
	inline reg set0<float>() {
		return _mm512_setzero_ps();
	}

	template <>
	inline reg set0<double>() {
		return (__m512) _mm512_setzero_pd();
	}

	template <>
	inline reg set0<int>() {
		return (__m512) _mm512_setzero_si512();
	}

	template <>
	inline reg set0<short>() {
		return (__m512) _mm512_setzero_si512();
	}

	template <>
	inline reg set0<signed char>() {
		return (__m512) _mm512_setzero_si512();
	}

#elif defined(__MIC__) || defined(__KNCNI__)
	template <>
	inline reg set0<float>() {
		return set1<float>(0.f);
	}

	template <>
	inline reg set0<double>() {
		return set1<double>(0.0);
	}

	template <>
	inline reg set0<int>() {
		return set1<int>(0);
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- andb
	template <>
	inline reg andb<float>(const reg v1, const reg v2) {
		return (__m512) _mm512_and_si512((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg andb<double>(const reg v1, const reg v2) {
		return (__m512) _mm512_and_si512((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg andb<short>(const reg v1, const reg v2) {
		return (__m512) _mm512_and_si512((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg andb<signed char>(const reg v1, const reg v2) {
		return (__m512) _mm512_and_si512((__m512i) v1, (__m512i) v2);
	}

	// ---------------------------------------------------------------------------------------------------------- andnb
	template <>
	inline reg andnb<float>(const reg v1, const reg v2) {
		return (__m512) _mm512_andnot_si512((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg andnb<double>(const reg v1, const reg v2) {
		return (__m512) _mm512_andnot_si512((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg andnb<short>(const reg v1, const reg v2) {
		return (__m512) _mm512_andnot_si512((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg andnb<signed char>(const reg v1, const reg v2) {
		return (__m512) _mm512_andnot_si512((__m512i) v1, (__m512i) v2);
	}

	// ------------------------------------------------------------------------------------------------------------ orb
	template <>
	inline reg orb<float>(const reg v1, const reg v2) {
		return (reg) _mm512_or_si512((__m512i)v1, (__m512i)v2);
	}

	template <>
	inline reg orb<double>(const reg v1, const reg v2) {
		return (reg) _mm512_or_si512((__m512i)v1, (__m512i)v2);
	}

	template <>
	inline reg orb<int>(const reg v1, const reg v2) {
		return (reg) _mm512_or_si512((__m512i)v1, (__m512i)v2);
	}

	template <>
	inline reg orb<short>(const reg v1, const reg v2) {
		return (reg) _mm512_or_si512((__m512i)v1, (__m512i)v2);
	}

	template <>
	inline reg orb<signed char>(const reg v1, const reg v2) {
		return (reg) _mm512_or_si512((__m512i)v1, (__m512i)v2);
	}

	// ----------------------------------------------------------------------------------------------------------- xorb
	template <>
	inline reg xorb<float>(const reg v1, const reg v2) {
		return (reg) _mm512_xor_si512((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg xorb<double>(const reg v1, const reg v2) {
		return (reg) _mm512_xor_si512((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg xorb<int>(const reg v1, const reg v2) {
		return (reg) _mm512_xor_si512((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg xorb<short>(const reg v1, const reg v2) {
		return (reg) _mm512_xor_si512((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg xorb<signed char>(const reg v1, const reg v2) {
		return (reg) _mm512_xor_si512((__m512i) v1, (__m512i) v2);
	}

	// --------------------------------------------------------------------------------------------------------- lshift
	template <>
	inline reg lshift<int>(const reg v1, const int n) {
		return (reg) _mm512_slli_epi32 ((__m512i) v1, n);
	}

#if defined(__AVX512BW__)
	template <>
	inline reg lshift<short>(const reg v1, const int n) {
		return (reg) _mm512_slli_epi16 ((__m512i) v1, n);
	}

	template <>
	inline reg lshift<signed char>(const reg v1, const int n) {
		return (reg) _mm512_slli_epi16 ((__m512i) v1, n); // TODO: be carefull here, this is a 16bit shift
	}
#endif

	// --------------------------------------------------------------------------------------------------------- rshift
	template <>
	inline reg rshift<int>(const reg v1, const int n) {
		return (reg) _mm512_srli_epi32 ((__m512i) v1, n);
	}

#if defined(__AVX512BW__)
	template <>
	inline reg rshift<short>(const reg v1, const int n) {
		return (reg) _mm512_srli_epi16 ((__m512i) v1, n);
	}

	template <>
	inline reg rshift<signed char>(const reg v1, const int n) {
		return (reg) _mm512_srli_epi16 ((__m512i) v1, n); // TODO: be carefull here, this is a 16bit shift
	}
#endif

	// ---------------------------------------------------------------------------------------------------------- cmpeq
	/*
	template <>
	inline reg cmpeq<float>(const reg v1, const reg v2) {
		return (reg) _mm512_cmp_ps_mask(v1, v2, _CMP_EQ_OQ); // TODO: this intrinsic return a mask...
	}

	template <>
	inline reg cmpeq<double>(const reg v1, const reg v2) {
		return (reg) _mm512_cmp_pd_mask((__m512d) v1, (__m512d) v2, _CMP_EQ_OQ); // TODO: this intrinsic return a mask...
	}

	template <>
	inline reg cmpeq<int>(const reg v1, const reg v2) {
		return (reg) _mm512_cmpeq_epi32_mask((__m512i) v1, (__m512i) v2); // TODO: this intrinsic return a mask...
	}

#if defined(__AVX512BW__)
	template <>
	inline reg cmpeq<short>(const reg v1, const reg v2) {
		return (reg) _mm512_cmpeq_epi16_mask((__m512i) v1, (__m512i) v2); // TODO: this intrinsic return a mask...
	}

	template <>
	inline reg cmpeq<signed char>(const reg v1, const reg v2) {
		return (reg) _mm512_cmpeq_epi8_mask((__m512i) v1, (__m512i) v2); // TODO: this intrinsic return a mask...
	}
#endif
	*/
	// --------------------------------------------------------------------------------------------------------- cmpneq
	/*
	template <>
	inline reg cmpneq<float>(const reg v1, const reg v2) {
		return (reg) _mm512_cmp_ps_mask(v1, v2, _CMP_NEQ_OQ); // TODO: this intrinsic return a mask...
	}

	template <>
	inline reg cmpneq<double>(const reg v1, const reg v2) {
		return (reg) _mm512_cmp_pd_mask((__m512d) v1, (__m512d) v2, _CMP_NEQ_OQ); // TODO: this intrinsic return a mask...
	}

	template <>
	inline reg cmpneq<int>(const reg v1, const reg v2) {
		return (reg) _mm512_cmpneq_epi32_mask((__m512i) v1, (__m512i) v2); // TODO: this intrinsic return a mask...
	}

#if defined(__AVX512BW__)
	template <>
	inline reg cmpneq<short>(const reg v1, const reg v2) {
		return (reg) _mm512_cmpneq_epi16_mask((__m512i) v1, (__m512i) v2); // TODO: this intrinsic return a mask...
	}

	template <>
	inline reg cmpneq<signed char>(const reg v1, const reg v2) {
		return (reg) _mm512_cmpneq_epi8_mask((__m512i) v1, (__m512i) v2); // TODO: this intrinsic return a mask...
	}
#endif
	*/
	// ---------------------------------------------------------------------------------------------------------- cmple
	
	template <>
	inline reg cmple<float>(const reg v1, const reg v2) {
		return (reg) _mm512_cmp_ps_mask(v1, v2, _CMP_LE_OS); // TODO: this intrinsic return a mask...
	}

	template <>
	inline reg cmple<double>(const reg v1, const reg v2) {
		return (reg) _mm512_cmp_pd_mask((__m512d) v1, (__m512d) v2, _CMP_LE_OS); // TODO: this intrinsic return a mask...
	}

	template <>
	inline reg cmple<int>(const reg v1, const reg v2) {
		return (reg) _mm512_cmp_epi32_mask((__m512d) v1, (__m512d) v2, _CMP_LE_OS); // TODO: this intrinsic return a mask...
	}

#if defined(__AVX512BW__)
	template <>
	inline reg cmple<short>(const reg v1, const reg v2) {
		return (reg) _mm512_cmp_epi16_mask((__m512d) v1, (__m512d) v2, _CMP_LE_OS); // TODO: this intrinsic return a mask...
	}

	template <>
	inline reg cmple<signed char>(const reg v1, const reg v2) {
		return (reg) _mm512_cmp_epi8_mask((__m512d) v1, (__m512d) v2, _CMP_LE_OS); // TODO: this intrinsic return a mask...
	}
#endif
	
	// ------------------------------------------------------------------------------------------------------------ add
	template <>
	inline reg add<float>(const reg v1, const reg v2) {
		return _mm512_add_ps(v1, v2);
	}

	template <>
	inline reg add<double>(const reg v1, const reg v2) {
		return (__m512) _mm512_add_pd((__m512d) v1, (__m512d) v2);
	}

	template <>
	inline reg add<int>(const reg v1, const reg v2) {
		return (__m512) _mm512_add_epi32((__m512i) v1, (__m512i) v2);
	}

#if defined(__AVX512BW__)
	template <>
	inline reg add<short>(const reg v1, const reg v2) {
		return (__m512) _mm512_adds_epi16((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg add<signed char>(const reg v1, const reg v2) {
		return (__m512) _mm512_adds_epi16((__m512i) v1, (__m512i) v2);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ sub
	template <>
	inline reg sub<float>(const reg v1, const reg v2) {
		return _mm512_sub_ps(v1, v2);
	}

	template <>
	inline reg sub<double>(const reg v1, const reg v2) {
		return (__m512) _mm512_sub_pd((__m512d) v1, (__m512d) v2);
	}

	template <>
	inline reg sub<int>(const reg v1, const reg v2) {
		return (__m512) _mm512_sub_epi32((__m512i) v1, (__m512i) v2);
	}

#if defined(__AVX512BW__)
	template <>
	inline reg sub<short>(const reg v1, const reg v2) {
		return (__m512) _mm512_subs_epi16((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg sub<signed char>(const reg v1, const reg v2) {
		return (__m512) _mm512_subs_epi8((__m512i) v1, (__m512i) v2);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ mul
	template <>
	inline reg mul<float>(const reg v1, const reg v2) {
		return _mm512_mul_ps(v1, v2);
	}

	template <>
	inline reg mul<double>(const reg v1, const reg v2) {
		return (__m512) _mm512_mul_pd((__m512d) v1, (__m512d) v2);
	}

#if defined(__AVX512F__)
	template <>
	inline reg mul<int>(const reg v1, const reg v2) {
		return (__m512) _mm512_mul_epi32((__m512i) v1, (__m512i) v2);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ div
#if defined(__AVX512F__)
	template <>
	inline reg div<float>(const reg v1, const reg v2) {
		return _mm512_div_ps(v1, v2);
	}

	template <>
	inline reg div<double>(const reg v1, const reg v2) {
		return (__m512) _mm512_div_pd((__m512d) v1, (__m512d) v2);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ min
#if defined(__AVX512F__)
	template <>
	inline reg min<float>(const reg v1, const reg v2) {
		return _mm512_min_ps(v1, v2);
	}

	template <>
	inline reg min<double>(const reg v1, const reg v2) {
		return (__m512) _mm512_min_pd((__m512d) v1, (__m512d) v2);
	}

	template <>
	inline reg min<int>(const reg v1, const reg v2) {
		return (__m512) _mm512_min_epi32((__m512i) v1, (__m512i) v2);
	}

#if defined(__AVX512BW__)
	template <>
	inline reg min<short>(const reg v1, const reg v2) {
		return (__m512) _mm512_min_epi16((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg min<signed char>(const reg v1, const reg v2) {
		return (__m512) _mm512_min_epi8((__m512i) v1, (__m512i) v2);
	}
#endif

#elif defined(__MIC__) || defined(__KNCNI__)
	template <>
	inline reg min<float>(const reg v1, const reg v2) {
		return _mm512_gmin_ps(v1, v2);
	}

	template <>
	inline reg min<double>(const reg v1, const reg v2) {
		return (__m512) _mm512_gmax_pd((__m512d) v1, (__m512d) v2);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ max
#if defined(__AVX512F__)
	template <>
	inline reg max<float>(const reg v1, const reg v2) {
		return _mm512_max_ps(v1, v2);
	}

	template <>
	inline reg max<double>(const reg v1, const reg v2) {
		return (__m512) _mm512_max_pd((__m512d) v1, (__m512d) v2);
	}

	template <>
	inline reg max<int>(const reg v1, const reg v2) {
		return (__m512) _mm512_max_epi32((__m512i) v1, (__m512i) v2);
	}

#if defined(__AVX512BW__)
	template <>
	inline reg max<short>(const reg v1, const reg v2) {
		return (__m512) _mm512_max_epi16((__m512i) v1, (__m512i) v2);
	}

	template <>
	inline reg max<signed char>(const reg v1, const reg v2) {
		return (__m512) _mm512_max_epi8((__m512i) v1, (__m512i) v2);
	}
#endif

#elif defined(__MIC__) || defined(__KNCNI__)
	template <>
	inline reg max<float>(const reg v1, const reg v2) {
		return _mm512_gmax_ps(v1, v2);
	}

	template <>
	inline reg max<double>(const reg v1, const reg v2) {
		return (__m512) _mm512_gmax_pd((__m512d) v1, (__m512d) v2);
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- sign
	template <>
	inline reg sign<float>(const reg v1) {
		// sign_mask = 10000000000000000000000000000000 // 32 bits
		const reg sign_mask = set1<int>(0x80000000);

		// indices = 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
		// mask    =  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
		// v1      =  ù  €  è  é  à  &  z  y  x  w  v  u  t  s  r  q  p  o  n  m  l  k  j  i  h  g  f  e  d  c  b  a
		// res     =  ù  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
		// res is the sign because the first bit is the sign bit (0 = positive, 1 = negative)
		return andb<float>(v1, sign_mask);
	}

	template <>
	inline reg sign<float>(const reg v1, const reg v2) {
		reg sign_v1_v2 = xorb<float>(v1, v2);
		    sign_v1_v2 = sign<float>(sign_v1_v2);
		return sign_v1_v2;
	}

	template <>
	inline reg sign<double>(const reg v1) {
		// sign_mask = 1000000000000000000000000000000000000000000000000000000000000000 // 64 bits
		const reg sign_mask = set1<long long>(0x8000000000000000);

		// indices = 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32...
		// mask    =  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0...
		// v1      =  ù  €  è  é  à  &  z  y  x  w  v  u  t  s  r  q  p  o  n  m  l  k  j  i  h  g  f  e  d  c  b  a...
		// res     =  ù  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0...
		// res is the sign because the first bit is the sign bit (0 = positive, 1 = negative)
		return andb<double>(v1, sign_mask);
	}

	template <>
	inline reg sign<double>(const reg v1, const reg v2) {
		reg sign_v1_v2 = xorb<double>(v1, v2);
		    sign_v1_v2 = sign<double>(sign_v1_v2);
		return sign_v1_v2;
	}

	template <>
	inline reg sign<short>(const reg v1) {
		const reg sign_mask = set1<short>(0x8000);
		return andb<short>(v1, sign_mask);
	}

	template <>
	inline reg sign<signed char>(const reg v1) {
		// sign_mask = 10000000 // 8 bits
		const reg sign_mask = set1<signed char>(0x80);

		// indices = 7  6  5  4  3  2  1  0
		// mask    = 1  0  0  0  0  0  0  0
		// v1      = h  g  f  e  d  c  b  a
		// res     = h  0  0  0  0  0  0  0
		// res is the sign because the first bit is the sign bit (0 = positive, 1 = negative)
		return andb<signed char>(v1, sign_mask);
	}

	template <>
	inline reg sign<short>(const reg v1, const reg v2) {
		reg sign_v1_v2 = xorb<short>(v1, v2);
		    sign_v1_v2 = sign<short>(sign_v1_v2);
		return sign_v1_v2;
	}

	template <>
	inline reg sign<signed char>(const reg v1, const reg v2) {
		reg sign_v1_v2 = xorb<signed char>(v1, v2);
		    sign_v1_v2 = sign<signed char>(sign_v1_v2);
		return sign_v1_v2;
	}

	// ------------------------------------------------------------------------------------------------------------ neg
	template <>
	inline reg neg<float>(const reg v1, const reg v2) {
		return xorb<float>(v1, v2);
	}

	template <>
	inline reg neg<double>(const reg v1, const reg v2) {
		return xorb<double>(v1, v2);
	}

	// ------------------------------------------------------------------------------------------------------------ neg
	template <>
	inline reg neg<float>(const reg v) {
		return xorb<int>(v, mipp::set1<int>(0x80000000));
	}

	template <>
	inline reg neg<double>(const reg v) {
		return xorb<long long>(v, mipp::set1<long long>(0x8000000000000000));
	}

	// ------------------------------------------------------------------------------------------------------------ abs
	template <>
	inline reg abs<float>(const reg v1) {
		return _mm512_abs_ps(v1);
	}

	template <>
	inline reg abs<double>(const reg v1) {
		return (reg) _mm512_abs_pd((__m512d) v1);
	}

	// ----------------------------------------------------------------------------------------------------------- sqrt
#if defined(__AVX512F__)
	template <>
	inline reg sqrt<float>(const reg v1) {
		return _mm512_sqrt_ps(v1);
	}

	template <>
	inline reg sqrt<double>(const reg v1) {
		return (__m512) _mm512_sqrt_pd((__m512d) v1);
	}
#endif

	// ---------------------------------------------------------------------------------------------------------- rsqrt
#if defined(__AVX512F__)
	template <>
	inline reg rsqrt<float>(const reg v1) {
		return _mm512_rsqrt14_ps(v1);
	}

	template <>
	inline reg rsqrt<double>(const reg v1) {
		return (__m512) _mm512_rsqrt14_pd((__m512d) v1)
	}

#elif defined(__MIC__) || defined(__KNCNI__)
	template <>
	inline reg rsqrt<float>(const reg v1) {
		return _mm512_rsqrt23_ps(v1);
	}
#endif

	// ---------------------------------------------------------------------------------------------------------- fmadd
	template <>
	inline reg fmadd<float>(const reg v1, const reg v2, const reg v3) {
		return _mm512_fmadd_ps(v1, v2, v3);
	}

	template <>
	inline reg fmadd<double>(const reg v1, const reg v2, const reg v3) {
		return (__m512) _mm512_fmadd_pd((__m512d) v1, (__m512d) v2, (__m512d) v3);
	}

	// --------------------------------------------------------------------------------------------------------- fnmadd
	template <>
	inline reg fnmadd<float>(const reg v1, const reg v2, const reg v3) {
		return _mm512_fnmadd_ps(v1, v2, v3);
	}

	template <>
	inline reg fnmadd<double>(const reg v1, const reg v2, const reg v3) {
		return (__m512) _mm512_fnmadd_pd((__m512d) v1, (__m512d) v2, (__m512d) v3);
	}

	// ---------------------------------------------------------------------------------------------------------- fmsub
	template <>
	inline reg fmsub<float>(const reg v1, const reg v2, const reg v3) {
		return _mm512_fmsub_ps(v1, v2, v3);
	}

	template <>
	inline reg fmsub<double>(const reg v1, const reg v2, const reg v3) {
		return (__m512) _mm512_fmsub_pd((__m512d) v1, (__m512d) v2, (__m512d) v3);
	}

	// ----------------------------------------------------------------------------------------------------------- div2
	template <>
	inline reg div2<float>(const reg v1) {
		return mul<float>(v1, set1<float>(0.5f));
	}

	template <>
	inline reg div2<double>(const reg v1) {
		return mul<double>(v1, set1<double>(0.5));
	}

	template <>
	inline reg div2<int>(const reg v1) {
		return (reg) _mm512_srai_epi32 ((__m512i) v1, 1);
	}

#if defined(__AVX512BW__)
	template <>
	inline reg div2<short>(const reg v1) {
		return (reg) _mm512_srai_epi16 ((__m512i) v1, 1);
	}

	template <>
	inline reg div2<signed char>(const reg v1) {
		reg abs_v1 = abs<signed char>(v1);
		reg sh16 = rshift<short>(abs_v1, 1);
		sh16 = andnb<signed char>(set1<signed char>(0x80), sh16);
		reg sh8 = neg<signed char>(sh16, v1);
		return sh8;
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- div4
	template <>
	inline reg div4<float>(const reg v1) {
		return mul<float>(v1, set1<float>(0.25f));
	}

	template <>
	inline reg div4<double>(const reg v1) {
		return mul<double>(v1, set1<double>(0.25));
	}

	template <>
	inline reg div4<int>(const reg v1) {
		return (reg) _mm512_srai_epi32 ((__m512i) v1, 2);
	}

#if defined(__AVX512BW__)
	template <>
	inline reg div4<short>(const reg v1) {
		return (reg) _mm512_srai_epi16 ((__m512i) v1, 2);
	}

	template <>
	inline reg div4<signed char>(const reg v1) {
		reg abs_v1 = abs<signed char>(v1);
		reg sh16 = rshift<short>(abs_v1, 2);
		sh16 = andnb<signed char>(set1<signed char>(0xc0), sh16);
		reg sh8 = neg<signed char>(sh16, v1);
		return sh8;
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ sat
	template <>
	inline reg sat<float>(const reg v1, float min, float max) {
		return mipp::min<float>(mipp::max<float>(v1, set1<float>(min)), set1<float>(max));
	}

	template <>
	inline reg sat<double>(const reg v1, double min, double max) {
		return mipp::min<double>(mipp::max<double>(v1, set1<double>(min)), set1<double>(max));
	}

	template <>
	inline reg sat<int>(const reg v1, int min, int max) {
		return mipp::min<int>(mipp::max<int>(v1, set1<int>(min)), set1<int>(max));
	}

	template <>
	inline reg sat<short>(const reg v1, short min, short max) {
		return mipp::min<short>(mipp::max<short>(v1, set1<short>(min)), set1<short>(max));
	}

	template <>
	inline reg sat<signed char>(const reg v1, signed char min, signed char max) {
		return mipp::min<signed char>(mipp::max<signed char>(v1, set1<signed char>(min)), set1<signed char>(max));
	}
#endif
