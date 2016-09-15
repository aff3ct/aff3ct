#include "mipp.h"

// -------------------------------------------------------------------------------------------------------- X86 AVX-256
// --------------------------------------------------------------------------------------------------------------------
#if defined(__AVX__)

	// ----------------------------------------------------------------------------------------------------------- load
	template <>
	inline reg load<float>(const float *mem_addr) {
		return _mm256_load_ps(mem_addr);
	}

	template <>
	inline reg load<double>(const double *mem_addr) {
		return (__m256) _mm256_load_pd(mem_addr);
	}

	template <>
	inline reg load<long long>(const long long *mem_addr) {
		return (__m256) _mm256_load_pd((const double*)mem_addr);
	}

	template <>
	inline reg load<int>(const int *mem_addr) {
		return _mm256_load_ps((const float*) mem_addr);
	}

	template <>
	inline reg load<short>(const short *mem_addr) {
		return _mm256_load_ps((const float*) mem_addr);
	}

	template <>
	inline reg load<signed char>(const signed char *mem_addr) {
		return (__m256) _mm256_load_ps((const float*) mem_addr);
	}

	// ---------------------------------------------------------------------------------------------------------- loadu
	template <>
	inline reg loadu<float>(const float *mem_addr) {
		return _mm256_loadu_ps(mem_addr);
	}

	template <>
	inline reg loadu<double>(const double *mem_addr) {
		return (__m256) _mm256_loadu_pd(mem_addr);
	}

	template <>
	inline reg loadu<long long>(const long long *mem_addr) {
		return (__m256) _mm256_loadu_pd((const double*) mem_addr);
	}

	template <>
	inline reg loadu<int>(const int *mem_addr) {
		return _mm256_loadu_ps((const float*) mem_addr);
	}

	template <>
	inline reg loadu<short>(const short *mem_addr) {
		return _mm256_loadu_ps((const float*) mem_addr);
	}

	template <>
	inline reg loadu<signed char>(const signed char *mem_addr) {
		return (__m256) _mm256_loadu_ps((const float*) mem_addr);
	}

	// ---------------------------------------------------------------------------------------------------------- store
	template <>
	inline void store<float>(float *mem_addr, const reg v) {
		_mm256_store_ps(mem_addr, v);
	}

	template <>
	inline void store<double>(double *mem_addr, const reg v) {
		_mm256_store_pd(mem_addr, (__m256d) v);
	}

	template <>
	inline void store<long long>(long long *mem_addr, const reg v) {
		_mm256_store_ps((float *)mem_addr, v);
	}

	template <>
	inline void store<int>(int *mem_addr, const reg v) {
		_mm256_store_ps((float *)mem_addr, v);
	}

	template <>
	inline void store<short>(short *mem_addr, const reg v) {
		_mm256_store_ps((float *)mem_addr, v);
	}

	template <>
	inline void store<signed char>(signed char *mem_addr, const reg v) {
		_mm256_store_ps((float *)mem_addr, v);
	}

	// --------------------------------------------------------------------------------------------------------- storeu
	template <>
	inline void storeu<float>(float *mem_addr, const reg v) {
		_mm256_storeu_ps(mem_addr, v);
	}

	template <>
	inline void storeu<double>(double *mem_addr, const reg v) {
		_mm256_storeu_pd(mem_addr, (__m256d) v);
	}

	template <>
	inline void storeu<long long>(long long *mem_addr, const reg v) {
		_mm256_storeu_ps((float *)mem_addr, v);
	}

	template <>
	inline void storeu<int>(int *mem_addr, const reg v) {
		_mm256_storeu_ps((float *)mem_addr, v);
	}

	template <>
	inline void storeu<short>(short *mem_addr, const reg v) {
		_mm256_storeu_ps((float *)mem_addr, v);
	}

	template <>
	inline void storeu<signed char>(signed char *mem_addr, const reg v) {
		_mm256_storeu_ps((float *)mem_addr, v);
	}

	// ------------------------------------------------------------------------------------------------------------ set
	template <>
	inline reg set<double>(const double vals[nElReg<double>()]) {
		return (reg)_mm256_set_pd(vals[3], vals[2], vals[1], vals[0]);
	}

	template <>
	inline reg set<float>(const float vals[nElReg<float>()]) {
		return (reg)_mm256_set_ps(vals[7], vals[6], vals[5], vals[4], vals[3], vals[2], vals[1], vals[0]);
	}

	template <>
	inline reg set<int>(const int vals[nElReg<int>()]) {
		return (reg)_mm256_set_epi32(vals[7], vals[6], vals[5], vals[4], vals[3], vals[2], vals[1], vals[0]);
	}

	template <>
	inline reg set<short>(const short vals[nElReg<short>()]) {
		return (reg)_mm256_set_epi16(vals[15], vals[14], vals[13], vals[12], vals[11], vals[10], vals[ 9], vals[ 8],
		                             vals[ 7], vals[ 6], vals[ 5], vals[ 4], vals[ 3], vals[ 2], vals[ 1], vals[ 0]);
	}

	template <>
	inline reg set<signed char>(const signed char vals[nElReg<signed char>()]) {
		return (reg)_mm256_set_epi8((char)vals[31], (char)vals[30], (char)vals[29], (char)vals[28], 
		                            (char)vals[27], (char)vals[26], (char)vals[25], (char)vals[24], 
		                            (char)vals[23], (char)vals[22], (char)vals[21], (char)vals[20], 
		                            (char)vals[19], (char)vals[18], (char)vals[17], (char)vals[16],
		                            (char)vals[15], (char)vals[14], (char)vals[13], (char)vals[12], 
		                            (char)vals[11], (char)vals[10], (char)vals[ 9], (char)vals[ 8], 
		                            (char)vals[ 7], (char)vals[ 6], (char)vals[ 5], (char)vals[ 4], 
		                            (char)vals[ 3], (char)vals[ 2], (char)vals[ 1], (char)vals[ 0]);
	}

	// ----------------------------------------------------------------------------------------------------------- set1
	template <>
	inline reg set1<float>(const float val) {
		return _mm256_set1_ps(val);
	}

	template <>
	inline reg set1<double>(const double val) {
		return (__m256) _mm256_set1_pd(val);
	}

	template <>
	inline reg set1<int>(const int val) {
		return (reg) _mm256_set1_epi32(val);
	}

	template <>
	inline reg set1<long long>(const long long val) {
		return (reg) _mm256_set1_epi64x(val);
	}

#ifdef __AVX2__
	template <>
	inline reg set1<short>(const short val) {
		return (reg) _mm256_set1_epi16(val);
	}

	template <>
	inline reg set1<signed char>(const signed char val) {
		return (reg) _mm256_set1_epi8(val);
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- set0
	template <>
	inline reg set0<float>() {
		return _mm256_setzero_ps();
	}

	template <>
	inline reg set0<double>() {
		return (__m256) _mm256_setzero_pd();
	}

	template <>
	inline reg set0<int>() {
		return (__m256) _mm256_setzero_si256();
	}

	template <>
	inline reg set0<short>() {
		return (__m256) _mm256_setzero_si256();
	}

	template <>
	inline reg set0<signed char>() {
		return (__m256) _mm256_setzero_si256();
	}

	// ------------------------------------------------------------------------------------------------------------ low
	template <>
	inline reg_2 low<double>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 0);
	}

	template <>
	inline reg_2 low<float>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 0);
	}

	template <>
	inline reg_2 low<long long>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 0);
	}

	template <>
	inline reg_2 low<int>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 0);
	}

	template <>
	inline reg_2 low<short>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 0);
	}

	template <>
	inline reg_2 low<signed char>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 0);
	}

	// ----------------------------------------------------------------------------------------------------------- high
	template <>
	inline reg_2 high<double>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 1);
	}

	template <>
	inline reg_2 high<float>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 1);
	}

	template <>
	inline reg_2 high<long long>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 1);
	}

	template <>
	inline reg_2 high<int>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 1);
	}

	template <>
	inline reg_2 high<short>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 1);
	}

	template <>
	inline reg_2 high<signed char>(const reg v) {
		return (reg_2) _mm256_extractf128_si256((__m256i) v, 1);
	}

	// ---------------------------------------------------------------------------------------------------------- cmask
	template <>
	inline reg cmask<float>(const int val[nElReg<float>()]) {
		signed char val_bis[nElReg<signed char>()] = {(signed char)(val[0]*4 + 0), (signed char)(val[0]*4 + 1), 
		                                              (signed char)(val[0]*4 + 2), (signed char)(val[0]*4 + 3),
		                                              (signed char)(val[1]*4 + 0), (signed char)(val[1]*4 + 1), 
		                                              (signed char)(val[1]*4 + 2), (signed char)(val[1]*4 + 3),
		                                              (signed char)(val[2]*4 + 0), (signed char)(val[2]*4 + 1), 
		                                              (signed char)(val[2]*4 + 2), (signed char)(val[2]*4 + 3),
		                                              (signed char)(val[3]*4 + 0), (signed char)(val[3]*4 + 1), 
		                                              (signed char)(val[3]*4 + 2), (signed char)(val[3]*4 + 3),
		                                              (signed char)(val[4]*4 + 0), (signed char)(val[4]*4 + 1), 
		                                              (signed char)(val[4]*4 + 2), (signed char)(val[4]*4 + 3),
		                                              (signed char)(val[5]*4 + 0), (signed char)(val[5]*4 + 1), 
		                                              (signed char)(val[5]*4 + 2), (signed char)(val[5]*4 + 3),
		                                              (signed char)(val[6]*4 + 0), (signed char)(val[6]*4 + 1), 
		                                              (signed char)(val[6]*4 + 2), (signed char)(val[6]*4 + 3),
		                                              (signed char)(val[7]*4 + 0), (signed char)(val[7]*4 + 1), 
		                                              (signed char)(val[7]*4 + 2), (signed char)(val[7]*4 + 3)};
		return mipp::set<signed char>(val_bis);
	}

	template <>
	inline reg cmask<int>(const int val[nElReg<int>()]) {
		signed char val_bis[nElReg<signed char>()] = {(signed char)(val[0]*4 + 0), (signed char)(val[0]*4 + 1), 
		                                              (signed char)(val[0]*4 + 2), (signed char)(val[0]*4 + 3),
		                                              (signed char)(val[1]*4 + 0), (signed char)(val[1]*4 + 1), 
		                                              (signed char)(val[1]*4 + 2), (signed char)(val[1]*4 + 3),
		                                              (signed char)(val[2]*4 + 0), (signed char)(val[2]*4 + 1), 
		                                              (signed char)(val[2]*4 + 2), (signed char)(val[2]*4 + 3),
		                                              (signed char)(val[3]*4 + 0), (signed char)(val[3]*4 + 1), 
		                                              (signed char)(val[3]*4 + 2), (signed char)(val[3]*4 + 3),
		                                              (signed char)(val[4]*4 + 0), (signed char)(val[4]*4 + 1), 
		                                              (signed char)(val[4]*4 + 2), (signed char)(val[4]*4 + 3),
		                                              (signed char)(val[5]*4 + 0), (signed char)(val[5]*4 + 1), 
		                                              (signed char)(val[5]*4 + 2), (signed char)(val[5]*4 + 3),
		                                              (signed char)(val[6]*4 + 0), (signed char)(val[6]*4 + 1), 
		                                              (signed char)(val[6]*4 + 2), (signed char)(val[6]*4 + 3),
		                                              (signed char)(val[7]*4 + 0), (signed char)(val[7]*4 + 1), 
		                                              (signed char)(val[7]*4 + 2), (signed char)(val[7]*4 + 3)};
		return mipp::set<signed char>(val_bis);
	}

	template <>
	inline reg cmask<short>(const int val[nElReg<short>()]) {
		signed char val_bis[nElReg<signed char>()] = {(signed char)(val[ 0]*2 + 0), (signed char)(val[ 0]*2 + 1),
		                                              (signed char)(val[ 1]*2 + 0), (signed char)(val[ 1]*2 + 1),
		                                              (signed char)(val[ 2]*2 + 0), (signed char)(val[ 2]*2 + 1),
		                                              (signed char)(val[ 3]*2 + 0), (signed char)(val[ 3]*2 + 1),
		                                              (signed char)(val[ 4]*2 + 0), (signed char)(val[ 4]*2 + 1),
		                                              (signed char)(val[ 5]*2 + 0), (signed char)(val[ 5]*2 + 1),
		                                              (signed char)(val[ 6]*2 + 0), (signed char)(val[ 6]*2 + 1),
		                                              (signed char)(val[ 7]*2 + 0), (signed char)(val[ 7]*2 + 1),
		                                              (signed char)(val[ 8]*2 + 0), (signed char)(val[ 8]*2 + 1),
		                                              (signed char)(val[ 9]*2 + 0), (signed char)(val[ 9]*2 + 1),
		                                              (signed char)(val[10]*2 + 0), (signed char)(val[10]*2 + 1),
		                                              (signed char)(val[11]*2 + 0), (signed char)(val[11]*2 + 1),
		                                              (signed char)(val[12]*2 + 0), (signed char)(val[12]*2 + 1),
		                                              (signed char)(val[13]*2 + 0), (signed char)(val[13]*2 + 1),
		                                              (signed char)(val[14]*2 + 0), (signed char)(val[14]*2 + 1),
		                                              (signed char)(val[15]*2 + 0), (signed char)(val[15]*2 + 1)};
		return mipp::set<signed char>(val_bis);
	}

	template <>
	inline reg cmask<signed char>(const int val[nElReg<signed char>()]) {
		signed char val_bis[nElReg<signed char>()] = {(signed char)val[ 0], (signed char)val[ 1], 
		                                              (signed char)val[ 2], (signed char)val[ 3], 
		                                              (signed char)val[ 4], (signed char)val[ 5], 
		                                              (signed char)val[ 6], (signed char)val[ 7],
		                                              (signed char)val[ 8], (signed char)val[ 9], 
		                                              (signed char)val[10], (signed char)val[11],
		                                              (signed char)val[12], (signed char)val[13], 
		                                              (signed char)val[14], (signed char)val[15],
		                                              (signed char)val[16], (signed char)val[17], 
		                                              (signed char)val[18], (signed char)val[19], 
		                                              (signed char)val[20], (signed char)val[21], 
		                                              (signed char)val[22], (signed char)val[23],
		                                              (signed char)val[24], (signed char)val[25], 
		                                              (signed char)val[26], (signed char)val[27],
		                                              (signed char)val[28], (signed char)val[29], 
		                                              (signed char)val[30], (signed char)val[31]};
		return mipp::set<signed char>(val_bis);
	}

	// --------------------------------------------------------------------------------------------------------- cmask2
	template <>
	inline reg cmask2<float>(const int val[nElReg<float>()/2]) {
		signed char val_bis[nElReg<signed char>()] = {(signed char)(val[0]*4 + 0), (signed char)(val[0]*4 + 1), 
		                                              (signed char)(val[0]*4 + 2), (signed char)(val[0]*4 + 3),
		                                              (signed char)(val[1]*4 + 0), (signed char)(val[1]*4 + 1), 
		                                              (signed char)(val[1]*4 + 2), (signed char)(val[1]*4 + 3),
		                                              (signed char)(val[2]*4 + 0), (signed char)(val[2]*4 + 1), 
		                                              (signed char)(val[2]*4 + 2), (signed char)(val[2]*4 + 3),
		                                              (signed char)(val[3]*4 + 0), (signed char)(val[3]*4 + 1), 
		                                              (signed char)(val[3]*4 + 2), (signed char)(val[3]*4 + 3),
		                                              (signed char)(val[0]*4 + 0), (signed char)(val[0]*4 + 1), 
		                                              (signed char)(val[0]*4 + 2), (signed char)(val[0]*4 + 3),
		                                              (signed char)(val[1]*4 + 0), (signed char)(val[1]*4 + 1), 
		                                              (signed char)(val[1]*4 + 2), (signed char)(val[1]*4 + 3),
		                                              (signed char)(val[2]*4 + 0), (signed char)(val[2]*4 + 1), 
		                                              (signed char)(val[2]*4 + 2), (signed char)(val[2]*4 + 3),
		                                              (signed char)(val[3]*4 + 0), (signed char)(val[3]*4 + 1), 
		                                              (signed char)(val[3]*4 + 2), (signed char)(val[3]*4 + 3)};
		return mipp::set<signed char>(val_bis);
	}

	template <>
	inline reg cmask2<int>(const int val[nElReg<int>()/2]) {
		signed char val_bis[nElReg<signed char>()] = {(signed char)(val[0]*4 + 0), (signed char)(val[0]*4 + 1), 
		                                              (signed char)(val[0]*4 + 2), (signed char)(val[0]*4 + 3),
		                                              (signed char)(val[1]*4 + 0), (signed char)(val[1]*4 + 1), 
		                                              (signed char)(val[1]*4 + 2), (signed char)(val[1]*4 + 3),
		                                              (signed char)(val[2]*4 + 0), (signed char)(val[2]*4 + 1), 
		                                              (signed char)(val[2]*4 + 2), (signed char)(val[2]*4 + 3),
		                                              (signed char)(val[3]*4 + 0), (signed char)(val[3]*4 + 1), 
		                                              (signed char)(val[3]*4 + 2), (signed char)(val[3]*4 + 3),
		                                              (signed char)(val[0]*4 + 0), (signed char)(val[0]*4 + 1), 
		                                              (signed char)(val[0]*4 + 2), (signed char)(val[0]*4 + 3),
		                                              (signed char)(val[1]*4 + 0), (signed char)(val[1]*4 + 1), 
		                                              (signed char)(val[1]*4 + 2), (signed char)(val[1]*4 + 3),
		                                              (signed char)(val[2]*4 + 0), (signed char)(val[2]*4 + 1), 
		                                              (signed char)(val[2]*4 + 2), (signed char)(val[2]*4 + 3),
		                                              (signed char)(val[3]*4 + 0), (signed char)(val[3]*4 + 1), 
		                                              (signed char)(val[3]*4 + 2), (signed char)(val[3]*4 + 3)};
		return mipp::set<signed char>(val_bis);
	}

	template <>
	inline reg cmask2<short>(const int val[nElReg<short>()/2]) {
		signed char val_bis[nElReg<signed char>()] = {(signed char)(val[ 0]*2 + 0), (signed char)(val[ 0]*2 + 1),
		                                              (signed char)(val[ 1]*2 + 0), (signed char)(val[ 1]*2 + 1),
		                                              (signed char)(val[ 2]*2 + 0), (signed char)(val[ 2]*2 + 1),
		                                              (signed char)(val[ 3]*2 + 0), (signed char)(val[ 3]*2 + 1),
		                                              (signed char)(val[ 4]*2 + 0), (signed char)(val[ 4]*2 + 1),
		                                              (signed char)(val[ 5]*2 + 0), (signed char)(val[ 5]*2 + 1),
		                                              (signed char)(val[ 6]*2 + 0), (signed char)(val[ 6]*2 + 1),
		                                              (signed char)(val[ 7]*2 + 0), (signed char)(val[ 7]*2 + 1),
		                                              (signed char)(val[ 0]*2 + 0), (signed char)(val[ 0]*2 + 1),
		                                              (signed char)(val[ 1]*2 + 0), (signed char)(val[ 1]*2 + 1),
		                                              (signed char)(val[ 2]*2 + 0), (signed char)(val[ 2]*2 + 1),
		                                              (signed char)(val[ 3]*2 + 0), (signed char)(val[ 3]*2 + 1),
		                                              (signed char)(val[ 4]*2 + 0), (signed char)(val[ 4]*2 + 1),
		                                              (signed char)(val[ 5]*2 + 0), (signed char)(val[ 5]*2 + 1),
		                                              (signed char)(val[ 6]*2 + 0), (signed char)(val[ 6]*2 + 1),
		                                              (signed char)(val[ 7]*2 + 0), (signed char)(val[ 7]*2 + 1)};
		return mipp::set<signed char>(val_bis);
	}

	template <>
	inline reg cmask2<signed char>(const int val[nElReg<signed char>()/2]) {
		signed char val_bis[nElReg<signed char>()] = {(signed char)val[ 0], (signed char)val[ 1], 
		                                              (signed char)val[ 2], (signed char)val[ 3], 
		                                              (signed char)val[ 4], (signed char)val[ 5], 
		                                              (signed char)val[ 6], (signed char)val[ 7],
		                                              (signed char)val[ 8], (signed char)val[ 9], 
		                                              (signed char)val[10], (signed char)val[11],
		                                              (signed char)val[12], (signed char)val[13], 
		                                              (signed char)val[14], (signed char)val[15],
		                                              (signed char)val[ 0], (signed char)val[ 1], 
		                                              (signed char)val[ 2], (signed char)val[ 3], 
		                                              (signed char)val[ 4], (signed char)val[ 5], 
		                                              (signed char)val[ 6], (signed char)val[ 7],
		                                              (signed char)val[ 8], (signed char)val[ 9], 
		                                              (signed char)val[10], (signed char)val[11],
		                                              (signed char)val[12], (signed char)val[13], 
		                                              (signed char)val[14], (signed char)val[15]};
		return mipp::set<signed char>(val_bis);
	}

	// ---------------------------------------------------------------------------------------------------------- shuff
	template <>
	inline reg shuff<float>(const reg v, const reg cm) {
		signed char control_mask[mipp::nElReg<float>()*sizeof(float)];
		mipp::store<signed char>(control_mask, cm);

		float v_in[mipp::nElReg<float>()], v_out[mipp::nElReg<float>()];
		mipp::store<float>(v_in, v);

		for (auto i = 0; i < mipp::nElReg<float>(); i++)
			v_out[i] = v_in[control_mask[i*sizeof(float)]/sizeof(float)];
		auto res = mipp::load<float>(v_out);

		return res;
	}

	// --------------------------------------------------------------------------------------------------------- shuff2
#ifdef __AVX2__
	template <>
	inline reg shuff2<float>(const reg v, const reg cm) {
		return (reg)_mm256_shuffle_epi8((__m256i)v, (__m256i)cm);
	}

	template <>
	inline reg shuff2<int>(const reg v, const reg cm) {
		return (reg)_mm256_shuffle_epi8((__m256i)v, (__m256i)cm);
	}

	template <>
	inline reg shuff2<short>(const reg v, const reg cm) {
		return (reg)_mm256_shuffle_epi8((__m256i)v, (__m256i)cm);
	}

	template <>
	inline reg shuff2<signed char>(const reg v, const reg cm) {
		return (reg)_mm256_shuffle_epi8((__m256i)v, (__m256i)cm);
	}
#endif

	// -------------------------------------------------------------------------------------------------- interleavelo2
#ifdef __AVX2__
	template <>
	inline reg interleavelo2<double>(const reg v1, const reg v2) {
		return (reg)_mm256_unpacklo_epi64((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg interleavelo2<long long>(const reg v1, const reg v2) {
		return (reg)_mm256_unpacklo_epi64((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg interleavelo2<float>(const reg v1, const reg v2) {
		// v1  = [a0, b0, c0, d0, e0, f0, g0, h0], v2 = [a1, b1, c1, d1, e1, f1, g1, h1]
		// res = [a0, a1, b0, b1, e0, e1, f0, f1]
		return (reg)_mm256_unpacklo_epi32((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg interleavelo2<int>(const reg v1, const reg v2) {
		// v1  = [a0, b0, c0, d0, e0, f0, g0, h0], v2 = [a1, b1, c1, d1, e1, f1, g1, h1]
		// res = [a0, a1, b0, b1, e0, e1, f0, f1]
		return (reg)_mm256_unpacklo_epi32((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg interleavelo2<short>(const reg v1, const reg v2) {
		return (reg)_mm256_unpacklo_epi16((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg interleavelo2<signed char>(const reg v1, const reg v2) {
		return (reg)_mm256_unpacklo_epi8((__m256i)v1, (__m256i)v2);
	}
#endif

	// -------------------------------------------------------------------------------------------------- interleavehi2
#ifdef __AVX2__
	template <>
	inline reg interleavehi2<double>(const reg v1, const reg v2) {
		return (reg)_mm256_unpackhi_epi64((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg interleavehi2<long long>(const reg v1, const reg v2) {
		return (reg)_mm256_unpackhi_epi64((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg interleavehi2<float>(const reg v1, const reg v2) {
		// v1  = [a0, b0, c0, d0, e0, f0, g0, h0], v2 = [a1, b1, c1, d1, e1, f1, g1, h1]
		// res = [c0, c1, d0, d1, g0, g1, h0, h1]
		return (reg)_mm256_unpackhi_epi32((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg interleavehi2<int>(const reg v1, const reg v2) {
		// v1  = [a0, b0, c0, d0, e0, f0, g0, h0], v2 = [a1, b1, c1, d1, e1, f1, g1, h1]
		// res = [c0, c1, d0, d1, g0, g1, h0, h1]
		return (reg)_mm256_unpackhi_epi32((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg interleavehi2<short>(const reg v1, const reg v2) {
		return (reg)_mm256_unpackhi_epi16((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg interleavehi2<signed char>(const reg v1, const reg v2) {
		return (reg)_mm256_unpackhi_epi8((__m256i)v1, (__m256i)v2);
	}
#endif

	// ----------------------------------------------------------------------------------------------------- interleave
#ifdef __AVX2__
	template <>
	inline regx2 interleave<double>(const reg v1, const reg v2) {
		auto lo = (reg)_mm256_unpacklo_epi64((__m256i)v1, (__m256i)v2);
		auto hi = (reg)_mm256_unpackhi_epi64((__m256i)v1, (__m256i)v2);

		return {{(reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,2,0,0)),
		         (reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,3,0,1))}};
	}

	template <>
	inline regx2 interleave<long long>(const reg v1, const reg v2) {
		auto lo = (reg)_mm256_unpacklo_epi64((__m256i)v1, (__m256i)v2);
		auto hi = (reg)_mm256_unpackhi_epi64((__m256i)v1, (__m256i)v2);

		return {{(reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,2,0,0)),
		         (reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,3,0,1))}};
	}

	template <>
	inline regx2 interleave<float>(const reg v1, const reg v2) {
		// v1         = [a0, b0, c0, d0, e0, f0, g0, h0], v2         = [a1, b1, c1, d1, e1, f1, g1, h1]
		// res.val[0] = [a0, a1, b0, b1, c0, c1, d0, d1], res.val[1] = [e0, e1, f0, f1, g0, g1, h0, h1]
		auto lo = (reg)_mm256_unpacklo_epi32((__m256i)v1, (__m256i)v2);
		auto hi = (reg)_mm256_unpackhi_epi32((__m256i)v1, (__m256i)v2);

		return {{(reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,2,0,0)),
		         (reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,3,0,1))}};
	}

	template <>
	inline regx2 interleave<int>(const reg v1, const reg v2) {
		// v1         = [a0, b0, c0, d0, e0, f0, g0, h0], v2         = [a1, b1, c1, d1, e1, f1, g1, h1]
		// res.val[0] = [a0, a1, b0, b1, c0, c1, d0, d1], res.val[1] = [e0, e1, f0, f1, g0, g1, h0, h1]
		auto lo = (reg)_mm256_unpacklo_epi32((__m256i)v1, (__m256i)v2);
		auto hi = (reg)_mm256_unpackhi_epi32((__m256i)v1, (__m256i)v2);

		return {{(reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,2,0,0)),
		         (reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,3,0,1))}};
	}

	template <>
	inline regx2 interleave<short>(const reg v1, const reg v2) {
		auto lo = (reg)_mm256_unpacklo_epi16((__m256i)v1, (__m256i)v2);
		auto hi = (reg)_mm256_unpackhi_epi16((__m256i)v1, (__m256i)v2);

		return {{(reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,2,0,0)),
		         (reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,3,0,1))}};
	}

	template <>
	inline regx2 interleave<signed char>(const reg v1, const reg v2) {
		auto lo = (reg)_mm256_unpacklo_epi8((__m256i)v1, (__m256i)v2);
		auto hi = (reg)_mm256_unpackhi_epi8((__m256i)v1, (__m256i)v2);

		return {{(reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,2,0,0)),
		         (reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,3,0,1))}};
	}
#endif

	// ----------------------------------------------------------------------------------------------------- interleave
#ifdef __AVX2__
	template <>
	inline reg interleave<int>(const reg v) {
		auto v_permute = (reg)_mm256_permute2f128_si256((__m256i)v, (__m256i)v, _MM_SHUFFLE(0,0,0,3));
		
		auto lo = (reg)_mm256_unpacklo_epi32((__m256i)v, (__m256i)v_permute);
		auto hi = (reg)_mm256_unpackhi_epi32((__m256i)v, (__m256i)v_permute);

		return (reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,2,0,0));
	}

	template <>
	inline reg interleave<short>(const reg v) {
		auto v_permute = (reg)_mm256_permute2f128_si256((__m256i)v, (__m256i)v, _MM_SHUFFLE(0,0,0,3));
		
		auto lo = (reg)_mm256_unpacklo_epi16((__m256i)v, (__m256i)v_permute);
		auto hi = (reg)_mm256_unpackhi_epi16((__m256i)v, (__m256i)v_permute);

		return (reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,2,0,0));
	}

	template <>
	inline reg interleave<signed char>(const reg v) {
		auto v_permute = (reg)_mm256_permute2f128_si256((__m256i)v, (__m256i)v, _MM_SHUFFLE(0,0,0,3));
		
		auto lo = (reg)_mm256_unpacklo_epi8((__m256i)v, (__m256i)v_permute);
		auto hi = (reg)_mm256_unpackhi_epi8((__m256i)v, (__m256i)v_permute);

		return (reg)_mm256_permute2f128_si256((__m256i)lo, (__m256i)hi, _MM_SHUFFLE(0,2,0,0));
	}
#endif

	// ---------------------------------------------------------------------------------------------------- interleave2
	template <>
	inline regx2 interleave2<float>(const reg v1, const reg v2) {
		regx2 res = {{mipp::interleavelo2<float>(v1, v2), mipp::interleavehi2<float>(v1, v2)}};
		return res;
	}

	template <>
	inline regx2 interleave2<int>(const reg v1, const reg v2) {
		regx2 res = {{mipp::interleavelo2<int>(v1, v2), mipp::interleavehi2<int>(v1, v2)}};
		return res;
	}

	template <>
	inline regx2 interleave2<short>(const reg v1, const reg v2) {
		regx2 res = {{mipp::interleavelo2<short>(v1, v2), mipp::interleavehi2<short>(v1, v2)}};
		return res;
	}

	template <>
	inline regx2 interleave2<signed char>(const reg v1, const reg v2) {
		regx2 res = {{mipp::interleavelo2<signed char>(v1, v2), mipp::interleavehi2<signed char>(v1, v2)}};
		return res;
	}

	// --------------------------------------------------------------------------------------------------- interleavex2
#ifdef __AVX2__
	template <>
	inline regx2 interleavex2<float>(const reg v1, const reg v2) {
		// v1         = [a, b, c, d,| e, f, g, h] 
		// v2         = [A, B, C, D,| E, F, G, H]
		// =>
		// res.val[0] = [a, b, c, d,| A, B, C, D] 
		// res.val[1] = [e, f, g, h,| E, F, G, H]
		auto lo = (mipp::reg)_mm256_permute2f128_si256((__m256i)v1, (__m256i)v2, _MM_SHUFFLE(0,2,0,0));
		auto hi = (mipp::reg)_mm256_permute2f128_si256((__m256i)v1, (__m256i)v2, _MM_SHUFFLE(0,3,0,1));

		regx2 res = {{lo, hi}};
		return res;
	}
#endif

	// --------------------------------------------------------------------------------------------------- interleavex4
#ifdef __AVX2__
	template <>
	inline reg interleavex4<short>(const reg v) {
		// [a, b, c, d,| e, f, g, h,| i, j, k, l,| m, n, o, p] 
		// =>
		// [a, b, c, d,| i, j, k, l,| e, f, g, h,| m, n, o, p]
		return (mipp::reg)_mm256_permute4x64_epi64((__m256i)v, _MM_SHUFFLE(3,1,2,0));
	}

	template <>
	inline reg interleavex4<signed char>(const reg v) {
		return (mipp::reg)_mm256_permute4x64_epi64((__m256i)v, _MM_SHUFFLE(3,1,2,0));
	}
#endif

	// -------------------------------------------------------------------------------------------------- interleavex16
	template <>
	inline reg interleavex16<signed char>(const reg v) {
		return mipp::interleave<short>(v);
	}


	// ------------------------------------------------------------------------------------------------------ transpose
#ifdef __AVX2__
	template <>
	inline void transpose<float>(reg tab[nElReg<float>()]) {
		// Transpose the 8x8 matrix:
		// -------------------------
		// tab[0] = [a0, a1, a2, a3, a4, a5, a6, a7]        tab[0] = [a0, b0, c0, d0, e0, f0, g0, h0]
		// tab[1] = [b0, b1, b2, b3, b4, b5, b6, b7]        tab[1] = [a1, b1, c1, d1, e1, f1, g1, h1]
		// tab[2] = [c0, c1, c2, c3, c4, c5, c6, c7]        tab[2] = [a2, b2, c2, d2, e2, f2, g2, h2]
		// tab[3] = [d0, d1, d2, d3, d4, d5, d6, d7]        tab[3] = [a3, b3, c3, d3, e3, f3, g3, h3]
		// tab[4] = [e0, e1, e2, e3, e4, e5, e6, e7]   =>   tab[4] = [a4, b4, c4, d4, e4, f4, g4, h4]
		// tab[5] = [f0, f1, f2, f3, f4, f5, f6, f7]        tab[5] = [a5, b5, c5, d5, e5, f5, g5, h5]
		// tab[6] = [g0, g1, g2, g3, g4, g5, g6, g7]        tab[6] = [a6, b6, c6, d6, e6, f6, g6, h6]
		// tab[7] = [h0, h1, h2, h3, h4, h5, h6, h7]        tab[7] = [a7, b7, c7, d7, e7, f7, g7, h7]

		auto ab = mipp::interleave<float>(tab[0], tab[1]);
		auto cd = mipp::interleave<float>(tab[2], tab[3]);
		auto ef = mipp::interleave<float>(tab[4], tab[5]);
		auto gh = mipp::interleave<float>(tab[6], tab[7]);

		auto a03b03 = ab.val[0];
		auto c03d03 = cd.val[0];
		auto e03f03 = ef.val[0];
		auto g03h03 = gh.val[0];
		auto a47b47 = ab.val[1];
		auto c47d47 = cd.val[1];
		auto e47f47 = ef.val[1];
		auto g47h47 = gh.val[1];

		auto a03b03c03d03 = mipp::interleave<long long>(a03b03, c03d03);
		auto e03f03g03h03 = mipp::interleave<long long>(e03f03, g03h03);
		auto a47b47c47d47 = mipp::interleave<long long>(a47b47, c47d47);
		auto e47f47g47h47 = mipp::interleave<long long>(e47f47, g47h47);

		auto a01b01c01d01 = a03b03c03d03.val[0];
		auto a23b23c23d23 = a03b03c03d03.val[1];
		auto e01f01g01h01 = e03f03g03h03.val[0];
		auto e23f23g23h23 = e03f03g03h03.val[1];
		auto a45b45c45d45 = a47b47c47d47.val[0];
		auto a67b67c67d67 = a47b47c47d47.val[1];
		auto e45f45g45h45 = e47f47g47h47.val[0];
		auto e67f67g67h67 = e47f47g47h47.val[1];

		auto a01b01c01d01e01f01g01h01 = mipp::interleavex2<float>(a01b01c01d01, e01f01g01h01);
		auto a23b23c23d23e23f23g23h23 = mipp::interleavex2<float>(a23b23c23d23, e23f23g23h23);
		auto a45b45c45d45e45f45g45h45 = mipp::interleavex2<float>(a45b45c45d45, e45f45g45h45);
		auto a67b67c67d67e67f67g67h67 = mipp::interleavex2<float>(a67b67c67d67, e67f67g67h67);

		auto a0b0c0d0e0f0g0h0 = a01b01c01d01e01f01g01h01.val[0];
		auto a1b1c1d1e1f1g1h1 = a01b01c01d01e01f01g01h01.val[1];
		auto a2b2c2d2e2f2g2h2 = a23b23c23d23e23f23g23h23.val[0];
		auto a3b3c3d3e3f3g3h3 = a23b23c23d23e23f23g23h23.val[1];
		auto a4b4c4d4e4f4g4h4 = a45b45c45d45e45f45g45h45.val[0];
		auto a5b5c5d5e5f5g5h5 = a45b45c45d45e45f45g45h45.val[1];
		auto a6b6c6d6e6f6g6h6 = a67b67c67d67e67f67g67h67.val[0];
		auto a7b7c7d7e7f7g7h7 = a67b67c67d67e67f67g67h67.val[1];

		tab[0] = (reg)a0b0c0d0e0f0g0h0;
		tab[1] = (reg)a1b1c1d1e1f1g1h1;
		tab[2] = (reg)a2b2c2d2e2f2g2h2;
		tab[3] = (reg)a3b3c3d3e3f3g3h3;
		tab[4] = (reg)a4b4c4d4e4f4g4h4;
		tab[5] = (reg)a5b5c5d5e5f5g5h5;
		tab[6] = (reg)a6b6c6d6e6f6g6h6;
		tab[7] = (reg)a7b7c7d7e7f7g7h7;
	}
#endif

	// ----------------------------------------------------------------------------------------------------- transpose2
#ifdef __AVX2__
	template <>
	inline void transpose2<short>(reg tab[nElReg<short>()/2]) {
		// Transpose the 8x8 matrix:
		// -------------------------
		//
		// Input:
		// ------
		// tab[0] = [a0, a1, a2, a3, a4, a5, a6, a7,  A0, A1, A2, A3, A4, A5, A6, A7]        
		// tab[1] = [b0, b1, b2, b3, b4, b5, b6, b7,  B0, B1, B2, B3, B4, B5, B6, B7]        
		// tab[2] = [c0, c1, c2, c3, c4, c5, c6, c7,  C0, C1, C2, C3, C4, C5, C6, C7]        
		// tab[3] = [d0, d1, d2, d3, d4, d5, d6, d7,  D0, D1, D2, D3, D4, D5, D6, D7]        
		// tab[4] = [e0, e1, e2, e3, e4, e5, e6, e7,  E0, E1, E2, E3, E4, E5, E6, E7]   
		// tab[5] = [f0, f1, f2, f3, f4, f5, f6, f7,  F0, F1, F2, F3, F4, F5, F6, F7]        
		// tab[6] = [g0, g1, g2, g3, g4, g5, g6, g7,  G0, G1, G2, G3, G4, G5, G6, G7]        
		// tab[7] = [h0, h1, h2, h3, h4, h5, h6, h7,  H0, H1, H2, H3, H4, H5, H6, H7]        
		//
		// Output:
		// -------
		// tab[0] = [a0, b0, c0, d0, e0, f0, g0, h0,  A0, B0, C0, D0, E0, F0, G0, H0]
		// tab[1] = [a1, b1, c1, d1, e1, f1, g1, h1,  A1, B1, C1, D1, E1, F1, G1, H1]
		// tab[2] = [a2, b2, c2, d2, e2, f2, g2, h2,  A2, B2, C2, D2, E2, F2, G2, H2]
		// tab[3] = [a3, b3, c3, d3, e3, f3, g3, h3,  A3, B3, C3, D3, E3, F3, G3, H3]
		// tab[4] = [a4, b4, c4, d4, e4, f4, g4, h4,  A4, B4, C4, D4, E4, F4, G4, H4]
		// tab[5] = [a5, b5, c5, d5, e5, f5, g5, h5,  A5, B5, C5, D5, E5, F5, G5, H5]
		// tab[6] = [a6, b6, c6, d6, e6, f6, g6, h6,  A6, B6, C6, D6, E6, F6, G6, H6]
		// tab[7] = [a7, b7, c7, d7, e7, f7, g7, h7,  A7, B7, C7, D7, E7, F7, G7, H7]

		auto a03b03 = mipp::interleavelo2<short>(tab[0], tab[1]);
		auto c03d03 = mipp::interleavelo2<short>(tab[2], tab[3]);
		auto e03f03 = mipp::interleavelo2<short>(tab[4], tab[5]);
		auto g03h03 = mipp::interleavelo2<short>(tab[6], tab[7]);
		auto a47b47 = mipp::interleavehi2<short>(tab[0], tab[1]);
		auto c47d47 = mipp::interleavehi2<short>(tab[2], tab[3]);
		auto e47f47 = mipp::interleavehi2<short>(tab[4], tab[5]);
		auto g47h47 = mipp::interleavehi2<short>(tab[6], tab[7]);

		auto a01b01c01d01 = mipp::interleavelo2<int>(a03b03, c03d03);
		auto a23b23c23d23 = mipp::interleavehi2<int>(a03b03, c03d03);
		auto e01f01g01h01 = mipp::interleavelo2<int>(e03f03, g03h03);
		auto e23f23g23h23 = mipp::interleavehi2<int>(e03f03, g03h03);
		auto a45b45c45d45 = mipp::interleavelo2<int>(a47b47, c47d47);
		auto a67b67c67d67 = mipp::interleavehi2<int>(a47b47, c47d47);
		auto e45f45g45h45 = mipp::interleavelo2<int>(e47f47, g47h47);
		auto e67f67g67h67 = mipp::interleavehi2<int>(e47f47, g47h47);

		auto a0b0c0d0e0f0g0h0 = mipp::interleavelo2<long long>(a01b01c01d01, e01f01g01h01);
		auto a1b1c1d1e1f1g1h1 = mipp::interleavehi2<long long>(a01b01c01d01, e01f01g01h01);
		auto a2b2c2d2e2f2g2h2 = mipp::interleavelo2<long long>(a23b23c23d23, e23f23g23h23);
		auto a3b3c3d3e3f3g3h3 = mipp::interleavehi2<long long>(a23b23c23d23, e23f23g23h23);
		auto a4b4c4d4e4f4g4h4 = mipp::interleavelo2<long long>(a45b45c45d45, e45f45g45h45);
		auto a5b5c5d5e5f5g5h5 = mipp::interleavehi2<long long>(a45b45c45d45, e45f45g45h45);
		auto a6b6c6d6e6f6g6h6 = mipp::interleavelo2<long long>(a67b67c67d67, e67f67g67h67);
		auto a7b7c7d7e7f7g7h7 = mipp::interleavehi2<long long>(a67b67c67d67, e67f67g67h67);

		tab[0] = (reg)a0b0c0d0e0f0g0h0;
		tab[1] = (reg)a1b1c1d1e1f1g1h1;
		tab[2] = (reg)a2b2c2d2e2f2g2h2;
		tab[3] = (reg)a3b3c3d3e3f3g3h3;
		tab[4] = (reg)a4b4c4d4e4f4g4h4;
		tab[5] = (reg)a5b5c5d5e5f5g5h5;
		tab[6] = (reg)a6b6c6d6e6f6g6h6;
		tab[7] = (reg)a7b7c7d7e7f7g7h7;
	}
#endif

	// -------------------------------------------------------------------------------------------------- transpose28x8
	template <>
	inline void transpose28x8<signed char>(reg tab[8]) {
		mipp::transpose2<short>(tab);
	}

	// ----------------------------------------------------------------------------------------------------------- andb
	template <>
	inline reg andb<float>(const reg v1, const reg v2) {
		return _mm256_and_ps(v1, v2);
	}

	template <>
	inline reg andb<double>(const reg v1, const reg v2) {
		return (__m256) _mm256_and_pd((__m256d) v1, (__m256d) v2);
	}

	template <>
	inline reg andb<int>(const reg v1, const reg v2) {
		return _mm256_and_ps(v1, v2);
	}

#ifdef __AVX2__
	template <>
	inline reg andb<short>(const reg v1, const reg v2) {
		return (__m256) _mm256_and_si256((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg andb<signed char>(const reg v1, const reg v2) {
		return (__m256) _mm256_and_si256((__m256i) v1, (__m256i) v2);
	}
#endif

	// ---------------------------------------------------------------------------------------------------------- andnb
	template <>
	inline reg andnb<float>(const reg v1, const reg v2) {
		return _mm256_andnot_ps(v1, v2);
	}

	template <>
	inline reg andnb<double>(const reg v1, const reg v2) {
		return (__m256) _mm256_andnot_pd((__m256d) v1, (__m256d) v2);
	}

	template <>
	inline reg andnb<int>(const reg v1, const reg v2) {
		return _mm256_andnot_ps(v1, v2);
	}

#ifdef __AVX2__
	template <>
	inline reg andnb<short>(const reg v1, const reg v2) {
		return (__m256) _mm256_andnot_si256((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg andnb<signed char>(const reg v1, const reg v2) {
		return (__m256) _mm256_andnot_si256((__m256i) v1, (__m256i) v2);
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- notb
	template <>
	inline reg notb<float>(const reg v) {
		return andnb<float>(v, set1<int>(0xFFFFFFFF));
	}

	template <>
	inline reg notb<double>(const reg v) {
		return andnb<double>(v, set1<long long>(0xFFFFFFFFFFFFFFFF));
	}

	template <>
	inline reg notb<int>(const reg v) {
		return andnb<int>(v, set1<int>(0xFFFFFFFF));
	}

	template <>
	inline reg notb<short>(const reg v) {
		return andnb<short>(v, set1<short>(0xFFFF));
	}

	template <>
	inline reg notb<signed char>(const reg v) {
		return andnb<signed char>(v, set1<signed char>(0xFF));
	}

	// ------------------------------------------------------------------------------------------------------------ orb
	template <>
	inline reg orb<float>(const reg v1, const reg v2) {
		return _mm256_or_ps(v1, v2);
	}

	template <>
	inline reg orb<double>(const reg v1, const reg v2) {
		return (__m256) _mm256_or_pd((__m256d) v1, (__m256d) v2);
	}

	template <>
	inline reg orb<int>(const reg v1, const reg v2) {
		return _mm256_or_ps(v1, v2);
	}

#ifdef __AVX2__
	template <>
	inline reg orb<short>(const reg v1, const reg v2) {
		return (reg) _mm256_or_si256((__m256i)v1, (__m256i)v2);
	}

	template <>
	inline reg orb<signed char>(const reg v1, const reg v2) {
		return (reg) _mm256_or_si256((__m256i)v1, (__m256i)v2);
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- xorb
	template <>
	inline reg xorb<float>(const reg v1, const reg v2) {
		return _mm256_xor_ps(v1, v2);
	}

	template <>
	inline reg xorb<double>(const reg v1, const reg v2) {
		return (__m256) _mm256_xor_pd((__m256d) v1, (__m256d) v2);
	}

	template <>
	inline reg xorb<int>(const reg v1, const reg v2) {
		return _mm256_xor_ps(v1, v2);
	}

#ifdef __AVX2__
	template <>
	inline reg xorb<short>(const reg v1, const reg v2) {
		return (reg) _mm256_xor_si256((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg xorb<signed char>(const reg v1, const reg v2) {
		return (reg) _mm256_xor_si256((__m256i) v1, (__m256i) v2);
	}
#endif

	// --------------------------------------------------------------------------------------------------------- lshift
#ifdef __AVX2__
	template <>
	inline reg lshift<long long>(const reg v1, const int n) {
		return (reg) _mm256_slli_epi64 ((__m256i) v1, n);
	}

	template <>
	inline reg lshift<int>(const reg v1, const int n) {
		return (reg) _mm256_slli_epi32 ((__m256i) v1, n);
	}

	template <>
	inline reg lshift<short>(const reg v1, const int n) {
		return (reg) _mm256_slli_epi16 ((__m256i) v1, n);
	}

	template <>
	inline reg lshift<signed char>(const reg v1, const int n) {
		// TODO: Be careful this is not a shift 8 but a shift 16 bits...
		return (reg) _mm256_slli_epi16 ((__m256i) v1, n);
	}
#else
	template <>
	inline reg lshift<int>(const reg v1, const int n) {
		__m128 v1_lane0, v1_lane1;
		__m256 res = v1;

		// apply left shift on lane 0
		v1_lane0 = _mm256_castps256_ps128(v1);
		v1_lane0 = (__m128) _mm_slli_epi32 ((__m128i) v1_lane0, n);

		// permute lane 0 with lane 1
		//   -> _mm256_permute2f128_ps(a, a, _MM_SHUFFLE(0, 0, 0, 1)
		//            lane 0        lane 1
		//       in[7, 6, 5, 4, | 3, 2, 1, 0] =>
		//      out[3, 2, 1, 0, | 7, 6, 5, 4]
		res = _mm256_permute2f128_ps(v1, v1, _MM_SHUFFLE(0, 0, 0, 1));

		// apply left shift on lane 1
		v1_lane1 = _mm256_castps256_ps128(res);
		v1_lane1 = (__m128) _mm_slli_epi32 ((__m128i) v1_lane1, n);

		// convert two SSE-128 registers into one AVX-256 register
		res = _mm256_castps128_ps256(v1_lane0);
		res = _mm256_insertf128_ps(res, v1_lane1, 1);

		return res;
	}
#endif

	// --------------------------------------------------------------------------------------------------------- rshift
#ifdef __AVX2__
	template <>
	inline reg rshift<long long>(const reg v1, const int n) {
		return (reg) _mm256_srli_epi64 ((__m256i) v1, n);
	}

	template <>
	inline reg rshift<int>(const reg v1, const int n) {
		return (reg) _mm256_srli_epi32 ((__m256i) v1, n);
	}

	template <>
	inline reg rshift<short>(const reg v1, const int n) {
		return (reg) _mm256_srli_epi16 ((__m256i) v1, n);
	}

	template <>
	inline reg rshift<signed char>(const reg v1, const int n) {
		// TODO: Be careful this is not a shift 8 but a shift 16 bits...
		return (reg) _mm256_srli_epi16 ((__m256i) v1, n);
	}
#else
	template <>
	inline reg rshift<int>(const reg v1, const int n) {
		__m128 v1_lane0, v1_lane1;
		__m256 res = v1;

		// apply left shift on lane 0
		v1_lane0 = _mm256_castps256_ps128(v1);
		v1_lane0 = (__m128) _mm_srli_epi32 ((__m128i) v1_lane0, n);

		// permute lane 0 with lane 1
		//   -> _mm256_permute2f128_ps(a, a, _MM_SHUFFLE(0, 0, 0, 1))
		//            lane 0        lane 1
		//       in[7, 6, 5, 4, | 3, 2, 1, 0] =>
		//      out[3, 2, 1, 0, | 7, 6, 5, 4]
		res = _mm256_permute2f128_ps(v1, v1, _MM_SHUFFLE(0, 0, 0, 1));

		// apply left shift on lane 1
		v1_lane1 = _mm256_castps256_ps128(res);
		v1_lane1 = (__m128) _mm_srli_epi32 ((__m128i) v1_lane1, n);

		// convert two SSE-128 registers into one AVX-256 register
		res = _mm256_castps128_ps256(v1_lane0);
		res = _mm256_insertf128_ps(res, v1_lane1, 1);

		return res;
	}
#endif

	// ---------------------------------------------------------------------------------------------------------- cmpeq
	template <>
	inline reg cmpeq<float>(const reg v1, const reg v2) {
		return _mm256_cmp_ps(v1, v2, _CMP_EQ_OQ);
	}

	template <>
	inline reg cmpeq<double>(const reg v1, const reg v2) {
		return (reg) _mm256_cmp_pd((__m256d) v1, (__m256d) v2, _CMP_EQ_OQ);
	}

#ifdef __AVX2__
	template <>
	inline reg cmpeq<int>(const reg v1, const reg v2) {
		return (reg) _mm256_cmpeq_epi32((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg cmpeq<short>(const reg v1, const reg v2) {
		return (reg) _mm256_cmpeq_epi16((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg cmpeq<signed char>(const reg v1, const reg v2) {
		return (reg) _mm256_cmpeq_epi8((__m256i) v1, (__m256i) v2);
	}
#endif

	// --------------------------------------------------------------------------------------------------------- cmpneq
	template <>
	inline reg cmpneq<float>(const reg v1, const reg v2) {
		return _mm256_cmp_ps(v1, v2, _CMP_NEQ_OQ);
	}

	template <>
	inline reg cmpneq<double>(const reg v1, const reg v2) {
		return (reg) _mm256_cmp_pd((__m256d) v1, (__m256d) v2, _CMP_NEQ_OQ);
	}

	template <>
	inline reg cmpneq<int>(const reg v1, const reg v2) {
		return _mm256_cmp_ps(v1, v2, _CMP_NEQ_OQ);
	}

	// ---------------------------------------------------------------------------------------------------------- cmplt
	template <>
	inline reg cmplt<float>(const reg v1, const reg v2) {
		return _mm256_cmp_ps(v1, v2, _CMP_LT_OS);
	}

	template <>
	inline reg cmplt<double>(const reg v1, const reg v2) {
		return (reg) _mm256_cmp_pd((__m256d) v1, (__m256d) v2, _CMP_LT_OS);
	}

	// ---------------------------------------------------------------------------------------------------------- cmple
	template <>
	inline reg cmple<float>(const reg v1, const reg v2) {
		return _mm256_cmp_ps(v1, v2, _CMP_LE_OS);
	}

	template <>
	inline reg cmple<double>(const reg v1, const reg v2) {
		return (reg) _mm256_cmp_pd((__m256d) v1, (__m256d) v2, _CMP_LE_OS);
	}

	// ---------------------------------------------------------------------------------------------------------- cmpgt
	template <>
	inline reg cmpgt<float>(const reg v1, const reg v2) {
		return _mm256_cmp_ps(v1, v2, _CMP_GT_OS);
	}

	template <>
	inline reg cmpgt<double>(const reg v1, const reg v2) {
		return (reg) _mm256_cmp_pd((__m256d) v1, (__m256d) v2, _CMP_GT_OS);
	}

#ifdef __AVX2__
	template <>
	inline reg cmpgt<int>(const reg v1, const reg v2) {
		return (reg) _mm256_cmpgt_epi32((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg cmpgt<short>(const reg v1, const reg v2) {
		return (reg) _mm256_cmpgt_epi16((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg cmpgt<signed char>(const reg v1, const reg v2) {
		return (reg) _mm256_cmpgt_epi8((__m256i) v1, (__m256i) v2);
	}
#endif

	// ---------------------------------------------------------------------------------------------------------- cmpge
	template <>
	inline reg cmpge<float>(const reg v1, const reg v2) {
		return _mm256_cmp_ps(v1, v2, _CMP_GE_OS);
	}

	template <>
	inline reg cmpge<double>(const reg v1, const reg v2) {
		return (reg) _mm256_cmp_pd((__m256d) v1, (__m256d) v2, _CMP_GE_OS);
	}

	// ------------------------------------------------------------------------------------------------------------ add
	template <>
	inline reg add<float>(const reg v1, const reg v2) {
		return _mm256_add_ps(v1, v2);
	}

	template <>
	inline reg add<double>(const reg v1, const reg v2) {
		return (__m256) _mm256_add_pd((__m256d) v1, (__m256d) v2);
	}

#ifdef __AVX2__
	template <>
	inline reg add<int>(const reg v1, const reg v2) {
		return (__m256) _mm256_add_epi32((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg add<short>(const reg v1, const reg v2) {
		return (__m256) _mm256_adds_epi16((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg add<signed char>(const reg v1, const reg v2) {
		return (__m256) _mm256_adds_epi8((__m256i) v1, (__m256i) v2);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ sub
	template <>
	inline reg sub<float>(const reg v1, const reg v2) {
		return _mm256_sub_ps(v1, v2);
	}

	template <>
	inline reg sub<double>(const reg v1, const reg v2) {
		return (__m256) _mm256_sub_pd((__m256d) v1, (__m256d) v2);
	}

#ifdef __AVX2__
	template <>
	inline reg sub<short>(const reg v1, const reg v2) {
		return (__m256) _mm256_subs_epi16 ((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg sub<signed char>(const reg v1, const reg v2) {
		return (__m256) _mm256_subs_epi8 ((__m256i) v1, (__m256i) v2);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ mul
	template <>
	inline reg mul<float>(const reg v1, const reg v2) {
		return _mm256_mul_ps(v1, v2);
	}

	template <>
	inline reg mul<double>(const reg v1, const reg v2) {
		return (__m256) _mm256_mul_pd((__m256d) v1, (__m256d) v2);
	}

#ifdef __AVX2__
	template <>
	inline reg mul<int>(const reg v1, const reg v2) {
		return (__m256) _mm256_mul_epi32((__m256i) v1, (__m256i) v2);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ div
	template <>
	inline reg div<float>(const reg v1, const reg v2) {
		return _mm256_div_ps(v1, v2);
	}

	template <>
	inline reg div<double>(const reg v1, const reg v2) {
		return (__m256) _mm256_div_pd((__m256d) v1, (__m256d) v2);
	}

	// ------------------------------------------------------------------------------------------------------------ min
	template <>
	inline reg min<float>(const reg v1, const reg v2) {
		return _mm256_min_ps(v1, v2);
	}

	template <>
	inline reg min<double>(const reg v1, const reg v2) {
		return (__m256) _mm256_min_pd((__m256d) v1, (__m256d) v2);
	}

#ifdef __AVX2__
	template <>
	inline reg min<int>(const reg v1, const reg v2) {
		return (__m256) _mm256_min_epi32((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg min<short>(const reg v1, const reg v2) {
		return (__m256) _mm256_min_epi16((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg min<signed char>(const reg v1, const reg v2) {
		return (__m256) _mm256_min_epi8((__m256i) v1, (__m256i) v2);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ max
	template <>
	inline reg max<float>(const reg v1, const reg v2) {
		return _mm256_max_ps(v1, v2);
	}

	template <>
	inline reg max<double>(const reg v1, const reg v2) {
		return (__m256) _mm256_max_pd((__m256d) v1, (__m256d) v2);
	}

#ifdef __AVX2__
	template <>
	inline reg max<int>(const reg v1, const reg v2) {
		return (__m256) _mm256_max_epi32((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg max<short>(const reg v1, const reg v2) {
		return (__m256) _mm256_max_epi16((__m256i) v1, (__m256i) v2);
	}

	template <>
	inline reg max<signed char>(const reg v1, const reg v2) {
		return (__m256) _mm256_max_epi8((__m256i) v1, (__m256i) v2);
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

#ifdef __AVX2__
	template <>
	inline reg neg<int>(const reg v1, const reg v2) {
		reg v2_2 = orb<int>(v2, set1<int>(1)); // hack to avoid -0 case
		return (reg) _mm256_sign_epi32((__m256i) v1, (__m256i) v2_2);
	}

	template <>
	inline reg neg<short>(const reg v1, const reg v2) {
		reg v2_2 = orb<short>(v2, set1<short>(1)); // hack to avoid -0 case
		return (reg) _mm256_sign_epi16((__m256i) v1, (__m256i) v2_2);
	}

	template <>
	inline reg neg<signed char>(const reg v1, const reg v2) {
		reg v2_2 = orb<signed char>(v2, set1<signed char>(1)); // hack to avoid -0 case
		return (reg) _mm256_sign_epi8((__m256i) v1, (__m256i) v2_2);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ abs
	template <>
	inline reg abs<float>(const reg v1) {
		// abs_mask = 01111111111111111111111111111111 // 32 bits
		const reg abs_mask = set1<int>(0x7FFFFFFF);

		// indices = 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
		// mask    =  0  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1
		// v1      =  ù  €  è  é  à  &  z  y  x  w  v  u  t  s  r  q  p  o  n  m  l  k  j  i  h  g  f  e  d  c  b  a
		// v1      =  0  €  è  é  à  &  z  y  x  w  v  u  t  s  r  q  p  o  n  m  l  k  j  i  h  g  f  e  d  c  b  a
		// res is the sign because the first bit is the sign bit (0 = positive, 1 = negative)
		return andb<float>(v1, abs_mask);
	}

	template <>
	inline reg abs<double>(const reg v1) {
		// abs_mask = 0111111111111111111111111111111111111111111111111111111111111111 // 64 bits
		const reg abs_mask = set1<long long>(0x7FFFFFFFFFFFFFFF);

		// indices = 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32...
		// mask    =  0  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1...
		// v1      =  ù  €  è  é  à  &  z  y  x  w  v  u  t  s  r  q  p  o  n  m  l  k  j  i  h  g  f  e  d  c  b  a...
		// v1      =  0  €  è  é  à  &  z  y  x  w  v  u  t  s  r  q  p  o  n  m  l  k  j  i  h  g  f  e  d  c  b  a...
		// res is the sign because the first bit is the sign bit (0 = positive, 1 = negative)
		return andb<double>(v1, abs_mask);
	}

#ifdef __AVX2__
	template <>
	inline reg abs<short>(const reg v1) {
		return (reg) _mm256_abs_epi16((__m256i) v1);
	}

	template <>
	inline reg abs<signed char>(const reg v1) {
		return (reg) _mm256_abs_epi8((__m256i) v1);
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- sqrt
	template <>
	inline reg sqrt<float>(const reg v1) {
		return _mm256_sqrt_ps(v1);
	}

	template <>
	inline reg sqrt<double>(const reg v1) {
		return (__m256) _mm256_sqrt_pd((__m256d) v1);
	}

	// ---------------------------------------------------------------------------------------------------------- rsqrt
	template <>
	inline reg rsqrt<float>(const reg v1) {
		return _mm256_rsqrt_ps(v1);
	}

	template <>
	inline reg rsqrt<double>(const reg v1) {
		return div<double>(set1<double>(1.0), sqrt<double>(v1));
	}

	// ------------------------------------------------------------------------------------------------------------ log
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
	template <>
	inline reg log<float>(const reg v) {
		return (reg) _mm256_log_ps(v);
	}

	template <>
	inline reg log<double>(const reg v) {
		return (reg) _mm256_log_pd((__m256d) v);
	}
#else
	template <>
	inline reg log<float>(const reg v) {
		auto v_bis = v;
		return (reg) log256_ps(v_bis);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ exp
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
	template <>
	inline reg exp<float>(const reg v) {
		return (reg) _mm256_exp_ps(v);
	}

	template <>
	inline reg exp<double>(const reg v) {
		return (reg) _mm256_exp_pd((__m256d) v);
	}
#else
	template <>
	inline reg exp<float>(const reg v) {
		auto v_bis = v;
		return (reg) exp256_ps(v_bis);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ sin
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
	template <>
	inline reg sin<float>(const reg v) {
		return (reg) _mm256_sin_ps(v);
	}

	template <>
	inline reg sin<double>(const reg v) {
		return (reg) _mm256_sin_pd((__m256d) v);
	}
#else
	template <>
	inline reg sin<float>(const reg v) {
		auto v_bis = v;
		return (reg) sin256_ps(v_bis);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ cos
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
	template <>
	inline reg cos<float>(const reg v) {
		return (reg) _mm256_cos_ps(v);
	}

	template <>
	inline reg cos<double>(const reg v) {
		return (reg) _mm256_cos_pd((__m256d) v);
	}
#else
	template <>
	inline reg cos<float>(const reg v) {
		auto v_bis = v;
		return (reg) cos256_ps(v_bis);
	}
#endif

	// --------------------------------------------------------------------------------------------------------- sincos
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
	template <>
	inline void sincos<float>(const reg x, reg &s, reg &c) {
		s = _mm256_sincos_ps(&c, x);
	}

	template <>
	inline void sincos<double>(const reg x, reg &s, reg &c) {
		s = (reg)_mm256_sincos_pd((__m256d*) &c, (__m256d)x);
	}
#else
	template <>
	inline void sincos<float>(const reg x, reg &s, reg &c) {
		sincos256_ps(x, &s, &c);
	}
#endif

	// ---------------------------------------------------------------------------------------------------------- fmadd
#ifdef __FMA__
	template <>
	inline reg fmadd<float>(const reg v1, const reg v2, const reg v3) {
		return _mm256_fmadd_ps(v1, v2, v3);
	}

	template <>
	inline reg fmadd<double>(const reg v1, const reg v2, const reg v3) {
		return (__m256) _mm256_fmadd_pd((__m256d) v1, (__m256d) v2, (__m256d) v3);
	}
#else
	template <>
	inline reg fmadd<float>(const reg v1, const reg v2, const reg v3) {
		return add<float>(v3, mul<float>(v1, v2));
	}

	template <>
	inline reg fmadd<double>(const reg v1, const reg v2, const reg v3) {
		return add<double>(v3, mul<double>(v1, v2));
	}
#endif

	// --------------------------------------------------------------------------------------------------------- fnmadd
#ifdef __FMA__
	template <>
	inline reg fnmadd<float>(const reg v1, const reg v2, const reg v3) {
		return _mm256_fnmadd_ps(v1, v2, v3);
	}

	template <>
	inline reg fnmadd<double>(const reg v1, const reg v2, const reg v3) {
		return (__m256) _mm256_fnmadd_pd((__m256d) v1, (__m256d) v2, (__m256d) v3);
	}
#else
	template <>
	inline reg fnmadd<float>(const reg v1, const reg v2, const reg v3) {
		return sub<float>(v3, mul<float>(v1, v2));
	}

	template <>
	inline reg fnmadd<double>(const reg v1, const reg v2, const reg v3) {
		return sub<double>(v3, mul<double>(v1, v2));
	}
#endif

	// ---------------------------------------------------------------------------------------------------------- fmsub
#ifdef __FMA__
	template <>
	inline reg fmsub<float>(const reg v1, const reg v2, const reg v3) {
		return _mm256_fmsub_ps(v1, v2, v3);
	}

	template <>
	inline reg fmsub<double>(const reg v1, const reg v2, const reg v3) {
		return (__m256) _mm256_fmsub_pd((__m256d) v1, (__m256d) v2, (__m256d) v3);
	}
#else
	template <>
	inline reg fmsub<float>(const reg v1, const reg v2, const reg v3) {
		return sub<float>(mul<float>(v1, v2), v3);
	}

	template <>
	inline reg fmsub<double>(const reg v1, const reg v2, const reg v3) {
		return sub<double>(mul<double>(v1, v2), v3);
	}
#endif

	// ------------------------------------------------------------------------------------------------------------ rot
#ifdef __AVX2__
	template <>
	inline reg rot<float>(const reg v1) {
		// make a rotation in:[7, 6, 5, 4, 3, 2 , 1, 0] => out:[0, 7, 6, 5, 4, 3, 2, 1]
		return _mm256_permutevar8x32_ps(v1, _mm256_setr_epi32(7, 0, 1, 2, 3, 4, 5, 6));
	}

	template <>
	inline reg rot<double>(const reg v1) {
		// make a rotation in:[3, 2 , 1, 0] => out:[0, 3, 2, 1]
		return (__m256) _mm256_permute4x64_pd ((__m256d) v1, _MM_SHUFFLE(2 ,1, 0, 3));
	}
#else
	template <>
	inline reg rot<float>(const reg v1) {
		// make a rotation in:[7, 6, 5, 4, 3, 2 , 1, 0] =>  out:[0, 7, 6, 5, 4, 3, 2, 1]
		//
		//   -> _mm256_permute_ps(a, _MM_SHUFFLE(0, 3, 2, 1)) # rotation per lane of 128 bits
		//            lane 0        lane 1
		//       in[7, 6, 5, 4, | 3, 2, 1, 0] =>
		//      out[4, 5, 6, 7, | 0, 3, 2, 1]
		//
		//   -> _mm256_permute2f128_ps(a, a, _MM_SHUFFLE(0, 0, 0, 1)) # switch lanes
		//            lane 0        lane 1
		//       in[7, 6, 5, 4, | 3, 2, 1, 0] =>
		//      out[3, 2, 1, 0, | 7, 6, 5, 4]
		//
		//   -> _mm256_blend_ps(a, b, _MM_SHUFFLE(2, 0, 2, 0))
		//         ina[7a, 6a, 5a, 4a, 3a, 2a, 1a, 0a]
		//         inb[7b, 6b, 5b, 4b, 3b, 2b, 1b, 0b] =>
		//         out[7b, 6a, 5a, 4a, 3b, 2a, 1a, 0a]
		reg rTmp = _mm256_permute_ps(v1, _MM_SHUFFLE(0, 3, 2, 1));
		return _mm256_blend_ps(rTmp,
							   _mm256_permute2f128_ps(rTmp, rTmp, _MM_SHUFFLE(0, 0, 0, 1)),
							   _MM_SHUFFLE(2, 0, 2, 0));
	}

	template <>
	inline reg rot<double>(const reg v1) {
		// make a rotation in:[3, 2 , 1, 0] => out:[0, 3, 2, 1]
		//
		//   -> _mm256_permute_pd(a, _MM_SHUFFLE(1, 1, 1, 1)) # rotation per lane of 128 bits
		//      /!\ I think this is a wrong shuffle and the good one is: _MM_SHUFFLE(0, 1, 0, 1)
		//           l0      l1
		//       in[3, 2, | 1, 0] =>
		//      out[2, 3, | 0, 1]
		//
		//   -> _mm256_permute2f128_pd(a, a, _MM_SHUFFLE(0, 0, 0, 1)) # switch lanes
		//           l0     l1
		//       in[3, 2, | 1, 0] =>
		//      out[1, 0, | 3, 2]
		//
		//   -> _mm256_blend_pd(a, b, _MM_SHUFFLE(0, 0, 2, 2))
		//      ina[3a, 2a, 1a, 0a] and inb[3b, 2b, 1b, 0b] => out[3b, 2a, 1b, 0a]
		reg rTmp = (__m256) _mm256_permute_pd((__m256d) v1, _MM_SHUFFLE(1, 1, 1, 1));
		return (__m256) _mm256_blend_pd((__m256d) rTmp,
		                                _mm256_permute2f128_pd((__m256d) rTmp,
		                                                       (__m256d) rTmp,
		                                                       _MM_SHUFFLE(0, 0, 0, 1)),
		                                _MM_SHUFFLE(0, 0, 2, 2));
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- div2
	template <>
	inline reg div2<float>(const reg v1) {
		return mul<float>(v1, set1<float>(0.5f));
	}

	template <>
	inline reg div2<double>(const reg v1) {
		return mul<double>(v1, set1<double>(0.5));
	}

#ifdef __AVX2__
	template <>
	inline reg div2<int>(const reg v1) {
		return (reg) _mm256_srai_epi32 ((__m256i) v1, 1);
	}

	template <>
	inline reg div2<short>(const reg v1) {
		return (reg) _mm256_srai_epi16 ((__m256i) v1, 1);
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

#ifdef __AVX2__
	template <>
	inline reg div4<int>(const reg v1) {
		return (reg) _mm256_srai_epi32 ((__m256i) v1, 2);
	}

	template <>
	inline reg div4<short>(const reg v1) {
		return (reg) _mm256_srai_epi16 ((__m256i) v1, 2);
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

	// ---------------------------------------------------------------------------------------------------------- round
	template <>
	inline reg round<float>(const reg v) {
		return (reg) _mm256_round_ps(v, _MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC);
	}

	template <>
	inline reg round<double>(const reg v) {
		return (reg) _mm256_round_pd((__m256d) v, _MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC);
	}

	// ------------------------------------------------------------------------------------------------------------ cvt
	template <>
	inline reg cvt<float,int>(const reg v) {
		return (reg) _mm256_cvtps_epi32(v);
	}

	template <>
	inline reg cvt<int,float>(const reg v) {
		return (reg) _mm256_cvtepi32_ps((__m256i) v);
	}

#ifdef __AVX2__
	template <>
	inline reg cvt<signed char,short>(const reg_2 v) {
		return (reg) _mm256_cvtepi8_epi16((__m128i) v);
	}

	template <>
	inline reg cvt<short,int>(const reg_2 v) {
		return (reg) _mm256_cvtepi16_epi32((__m128i) v);
	}
#endif

	// ----------------------------------------------------------------------------------------------------------- pack
#ifdef __AVX2__
	template <>
	inline reg pack<int,short>(const reg v1, const reg v2) {
		return (reg) _mm256_permute4x64_epi64(_mm256_packs_epi32((__m256i) v1, (__m256i) v2), _MM_SHUFFLE(3, 1, 2, 0));
	}

	template <>
	inline reg pack<short,signed char>(const reg v1, const reg v2) {
		return (reg) _mm256_permute4x64_epi64(_mm256_packs_epi16((__m256i) v1, (__m256i) v2), _MM_SHUFFLE(3, 1, 2, 0));
	}
#endif

	// ------------------------------------------------------------------------------------------------------ reduction
	template <red_op<double> OP>
	struct _reduction<double,OP>
	{
		static reg apply(const reg v1) {
			auto val = v1;
			val = OP(val, (reg)_mm256_permute2f128_ps(val, val, _MM_SHUFFLE(0,0,0,1)));
			val = OP(val, (reg)_mm256_shuffle_ps     (val, val, _MM_SHUFFLE(1,0,3,2)));
			return val;
		}
	};

	template <Red_op<double> OP>
	struct _Reduction<double,OP>
	{
		static const Reg<double> apply(const Reg<double> v1) {
			auto val = v1;
			val = OP(val, Reg<double>((reg)_mm256_permute2f128_ps(val.r, val.r, _MM_SHUFFLE(0,0,0,1))));
			val = OP(val, Reg<double>((reg)_mm256_shuffle_ps     (val.r, val.r, _MM_SHUFFLE(1,0,3,2))));
			return val;
		}
	};

	template <red_op<float> OP>
	struct _reduction<float,OP>
	{
		static reg apply(const reg v1) {
			auto val = v1;
			val = OP(val, (reg)_mm256_permute2f128_ps(val, val, _MM_SHUFFLE(0,0,0,1)));
			val = OP(val, (reg)_mm256_shuffle_ps     (val, val, _MM_SHUFFLE(1,0,3,2)));
			val = OP(val, (reg)_mm256_shuffle_ps     (val, val, _MM_SHUFFLE(2,3,0,1)));
			return val;
		}
	};

	template <Red_op<float> OP>
	struct _Reduction<float,OP>
	{
		static Reg<float> apply(const Reg<float> v1) {
			auto val = v1;
			val = OP(val, Reg<float>((reg)_mm256_permute2f128_ps(val.r, val.r, _MM_SHUFFLE(0,0,0,1))));
			val = OP(val, Reg<float>((reg)_mm256_shuffle_ps     (val.r, val.r, _MM_SHUFFLE(1,0,3,2))));
			val = OP(val, Reg<float>((reg)_mm256_shuffle_ps     (val.r, val.r, _MM_SHUFFLE(2,3,0,1))));
			return val;
		}
	};

	template <red_op<int> OP>
	struct _reduction<int,OP>
	{
		static reg apply(const reg v1) {
			auto val = v1;
			val = OP(val, (reg)_mm256_permute2f128_ps(val, val, _MM_SHUFFLE(0,0,0,1)));
			val = OP(val, (reg)_mm256_shuffle_ps     (val, val, _MM_SHUFFLE(1,0,3,2)));
			val = OP(val, (reg)_mm256_shuffle_ps     (val, val, _MM_SHUFFLE(2,3,0,1)));
			return val;
		}
	};

	template <Red_op<int> OP>
	struct _Reduction<int,OP>
	{
		static Reg<int> apply(const Reg<int> v1) {
			auto val = v1;
			val = OP(val, Reg<int>((reg)_mm256_permute2f128_ps(val.r, val.r, _MM_SHUFFLE(0,0,0,1))));
			val = OP(val, Reg<int>((reg)_mm256_shuffle_ps     (val.r, val.r, _MM_SHUFFLE(1,0,3,2))));
			val = OP(val, Reg<int>((reg)_mm256_shuffle_ps     (val.r, val.r, _MM_SHUFFLE(2,3,0,1))));
			return val;
		}
	};

#ifdef __AVX2__
	template <red_op<short> OP>
	struct _reduction<short,OP>
	{
		static reg apply(const reg v1) {
			__m256i mask_16 = _mm256_set_epi8(29, 28, 31, 30, 25, 24, 27, 26, 21, 20, 23, 22, 17, 16, 19, 18, 13, 12, 15,
			                                  14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);

			auto val = v1;
			val = OP(val, (reg)_mm256_permute2f128_si256((__m256i)val, (__m256i)val, _MM_SHUFFLE(0,0,0,1)));
			val = OP(val, (reg)_mm256_permute4x64_epi64 ((__m256i)val,               _MM_SHUFFLE(2,3,0,1)));
			val = OP(val, (reg)_mm256_shuffle_epi32     ((__m256i)val,               _MM_SHUFFLE(2,3,0,1)));
			val = OP(val, (reg)_mm256_shuffle_epi8      ((__m256i)val,               mask_16));
			return val;
		}
	};

	template <Red_op<short> OP>
	struct _Reduction<short,OP>
	{
		static Reg<short> apply(const Reg<short> v1) {
			__m256i mask_16 = _mm256_set_epi8(29, 28, 31, 30, 25, 24, 27, 26, 21, 20, 23, 22, 17, 16, 19, 18, 13, 12, 15,
			                                  14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);

			auto val = v1;
			val = OP(val, Reg<short>((reg)_mm256_permute2f128_si256((__m256i)val.r, (__m256i)val.r, _MM_SHUFFLE(0,0,0,1))));
			val = OP(val, Reg<short>((reg)_mm256_permute4x64_epi64 ((__m256i)val.r,                 _MM_SHUFFLE(2,3,0,1))));
			val = OP(val, Reg<short>((reg)_mm256_shuffle_epi32     ((__m256i)val.r,                 _MM_SHUFFLE(2,3,0,1))));
			val = OP(val, Reg<short>((reg)_mm256_shuffle_epi8      ((__m256i)val.r,                 mask_16)));
			return val;
		}
	};
#endif

#ifdef __AVX2__
	template <red_op<signed char> OP>
	struct _reduction<signed char,OP>
	{
		static reg apply(const reg v1) {
			__m256i mask_16 = _mm256_set_epi8(29, 28, 31, 30, 25, 24, 27, 26, 21, 20, 23, 22, 17, 16, 19, 18, 13, 12, 15,
			                                  14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);
			__m256i mask_8  = _mm256_set_epi8(30, 31, 28, 29, 26, 27, 24, 25, 22, 23, 20, 21, 18, 19, 16, 17, 14, 15, 12,
			                                  13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);

			auto val = v1;
			val = OP(val, (reg)_mm256_permute2f128_si256((__m256i)val, (__m256i)val, _MM_SHUFFLE(0,0,0,1)));
			val = OP(val, (reg)_mm256_permute4x64_epi64 ((__m256i)val,               _MM_SHUFFLE(2,3,0,1)));
			val = OP(val, (reg)_mm256_shuffle_epi32     ((__m256i)val,               _MM_SHUFFLE(2,3,0,1)));
			val = OP(val, (reg)_mm256_shuffle_epi8      ((__m256i)val,               mask_16));
			val = OP(val, (reg)_mm256_shuffle_epi8      ((__m256i)val,               mask_8));
			return val;
		}
	};

	template <Red_op<signed char> OP>
	struct _Reduction<signed char,OP>
	{
		static Reg<signed char> apply(const Reg<signed char> v1) {
			__m256i mask_16 = _mm256_set_epi8(29, 28, 31, 30, 25, 24, 27, 26, 21, 20, 23, 22, 17, 16, 19, 18, 13, 12, 15,
			                                  14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2);
			__m256i mask_8  = _mm256_set_epi8(30, 31, 28, 29, 26, 27, 24, 25, 22, 23, 20, 21, 18, 19, 16, 17, 14, 15, 12,
			                                  13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);

			auto val = v1;
			val = OP(val, Reg<signed char>((reg)_mm256_permute2f128_si256((__m256i)val.r, (__m256i)val.r, _MM_SHUFFLE(0,0,0,1))));
			val = OP(val, Reg<signed char>((reg)_mm256_permute4x64_epi64 ((__m256i)val.r,                 _MM_SHUFFLE(2,3,0,1))));
			val = OP(val, Reg<signed char>((reg)_mm256_shuffle_epi32     ((__m256i)val.r,                 _MM_SHUFFLE(2,3,0,1))));
			val = OP(val, Reg<signed char>((reg)_mm256_shuffle_epi8      ((__m256i)val.r,                 mask_16)));
			val = OP(val, Reg<signed char>((reg)_mm256_shuffle_epi8      ((__m256i)val.r,                 mask_8)));
			return val;
		}
	};
#endif
#endif