#include "mipp.h"

// ------------------------------------------------------------------------------------------------------- ARM NEON-128
// --------------------------------------------------------------------------------------------------------------------
#if defined(__ARM_NEON__) || defined(__ARM_NEON)

	// ---------------------------------------------------------------------------------------------------------- loadu
	template <>
	inline reg loadu<float>(const float *mem_addr) {
		return vld1q_f32(mem_addr);
	}

	template <>
	inline reg loadu<int>(const int *mem_addr) {
		return (reg) vld1q_s32(mem_addr);
	}

	template <>
	inline reg loadu<short>(const short *mem_addr) {
		return (reg) vld1q_s16((int16_t*) mem_addr);
	}

	template <>
	inline reg loadu<signed char>(const signed char *mem_addr) {
		return (reg) vld1q_s8((int8_t*) mem_addr);
	}

	// ----------------------------------------------------------------------------------------------------------- load
#ifdef MIPP_ALIGNED_LOADS
	template <>
	inline reg load<float>(const float *mem_addr) {
		return vld1q_f32(mem_addr);
	}

	template <>
	inline reg load<int>(const int *mem_addr) {
		return (reg) vld1q_s32(mem_addr);
	}

	template <>
	inline reg load<short>(const short *mem_addr) {
		return (reg) vld1q_s16((int16_t*) mem_addr);
	}

	template <>
	inline reg load<signed char>(const signed char *mem_addr) {
		return (reg) vld1q_s8((int8_t*) mem_addr);
	}
#else
	template <>
	inline reg load<float>(const float *mem_addr) {
		return mipp::loadu<float>(mem_addr);
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
	template <>
	inline void storeu<float>(float *mem_addr, const reg v) {
		vst1q_f32(mem_addr, v);
	}

	template <>
	inline void storeu<long long>(long long *mem_addr, const reg v) {
		vst1q_f32((float*) mem_addr, v);
	}

	template <>
	inline void storeu<int>(int *mem_addr, const reg v) {
		vst1q_f32((float*) mem_addr, v);
	}

	template <>
	inline void storeu<short>(short *mem_addr, const reg v) {
		vst1q_s16((int16_t*) mem_addr, (int16x8_t) v);
	}

	template <>
	inline void storeu<signed char>(signed char *mem_addr, const reg v) {
		vst1q_s8((int8_t*) mem_addr, (int8x16_t) v);
	}

	// ---------------------------------------------------------------------------------------------------------- store
#ifdef MIPP_ALIGNED_LOADS
	template <>
	inline void store<float>(float *mem_addr, const reg v) {
		vst1q_f32(mem_addr, v);
	}

	template <>
	inline void store<long long>(long long *mem_addr, const reg v) {
		vst1q_f32((float*) mem_addr, v);
	}

	template <>
	inline void store<int>(int *mem_addr, const reg v) {
		vst1q_f32((float*) mem_addr, v);
	}

	template <>
	inline void store<short>(short *mem_addr, const reg v) {
		vst1q_s16((int16_t*) mem_addr, (int16x8_t) v);
	}

	template <>
	inline void store<signed char>(signed char *mem_addr, const reg v) {
		vst1q_s8((int8_t*) mem_addr, (int8x16_t) v);
	}
#else
	template <>
	inline void store<float>(float *mem_addr, const reg v) {
		mipp::storeu<float>(mem_addr, v);
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

	// ------------------------------------------------------------------------------------------------------------ set
	template <>
	inline reg set<float>(const float vals[nElReg<float>()]) {
		return mipp::load<float>(vals);
	}

	template <>
	inline reg set<int>(const int vals[nElReg<int>()]) {
		return mipp::load<int>(vals);
	}

	template <>
	inline reg set<short>(const short vals[nElReg<short>()]) {
		return mipp::load<short>(vals);
	}

	template <>
	inline reg set<signed char>(const signed char vals[nElReg<signed char>()]) {
		return mipp::load<signed char>(vals);
	}

	// ----------------------------------------------------------------------------------------------------------- set1
	template <>
	inline reg set1<float>(const float val) {
		return vdupq_n_f32(val);
	}

	template <>
	inline reg set1<int>(const int val) {
		return (reg) vdupq_n_s32(val);
	}

	template <>
	inline reg set1<short>(const short val) {
		return (reg) vdupq_n_s16(val);
	}

	template <>
	inline reg set1<signed char>(const signed char val) {
		return (reg) vdupq_n_s8(val);
	}

	// ----------------------------------------------------------------------------------------------------------- set0
	template <>
	inline reg set0<float>() {
		return vdupq_n_f32(0.f);
	}

	template <>
	inline reg set0<int>() {
		return (reg) vdupq_n_s32(0);
	}

	template <>
	inline reg set0<short>() {
		return (reg) vdupq_n_s16(0);
	}

	template <>
	inline reg set0<signed char>() {
		return (reg) vdupq_n_s8(0);
	}

	// ------------------------------------------------------------------------------------------------------------ low
	template <>
	inline reg_2 low<float>(const reg v) {
		return (reg_2) vget_low_f32((float32x4_t) v);
	}

	template <>
	inline reg_2 low<int>(const reg v) {
		return (reg_2) vget_low_f32((float32x4_t) v);
	}

	template <>
	inline reg_2 low<short>(const reg v) {
		return (reg_2) vget_low_f32((float32x4_t) v);
	}

	template <>
	inline reg_2 low<signed char>(const reg v) {
		return (reg_2) vget_low_f32((float32x4_t) v);
	}

	// ----------------------------------------------------------------------------------------------------------- high
	template <>
	inline reg_2 high<float>(const reg v) {
		return (reg_2) vget_high_f32((float32x4_t) v);
	}

	template <>
	inline reg_2 high<int>(const reg v) {
		return (reg_2) vget_high_f32((float32x4_t) v);
	}

	template <>
	inline reg_2 high<short>(const reg v) {
		return (reg_2) vget_high_f32((float32x4_t) v);
	}

	template <>
	inline reg_2 high<signed char>(const reg v) {
		return (reg_2) vget_high_f32((float32x4_t) v);
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
		                                              (signed char)(val[3]*4 + 2), (signed char)(val[3]*4 + 3)};
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
		                                              (signed char)(val[3]*4 + 2), (signed char)(val[3]*4 + 3)};
		return mipp::set<signed char>(val_bis);
	}

	template <>
	inline reg cmask<short>(const int val[nElReg<short>()]) {
		signed char val_bis[nElReg<signed char>()] = {(signed char)(val[0]*2 + 0), (signed char)(val[0]*2 + 1),
		                                              (signed char)(val[1]*2 + 0), (signed char)(val[1]*2 + 1),
		                                              (signed char)(val[2]*2 + 0), (signed char)(val[2]*2 + 1),
		                                              (signed char)(val[3]*2 + 0), (signed char)(val[3]*2 + 1),
		                                              (signed char)(val[4]*2 + 0), (signed char)(val[4]*2 + 1),
		                                              (signed char)(val[5]*2 + 0), (signed char)(val[5]*2 + 1),
		                                              (signed char)(val[6]*2 + 0), (signed char)(val[6]*2 + 1),
		                                              (signed char)(val[7]*2 + 0), (signed char)(val[7]*2 + 1)};
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
		                                              (signed char)val[14], (signed char)val[15]};
		return mipp::set<signed char>(val_bis);
	}

	// ---------------------------------------------------------------------------------------------------------- shuff
	template <>
	inline reg shuff<float>(const reg v, const reg cm) {
		uint8x8x2_t v2 = {vget_low_u8((uint8x16_t)v), vget_high_u8((uint8x16_t)v)};
		uint8x8_t low  = vtbl2_u8(v2, vget_low_u8 ((uint8x16_t)cm));
		uint8x8_t high = vtbl2_u8(v2, vget_high_u8((uint8x16_t)cm));

		return (reg)vcombine_u8(low, high);
	}

	template <>
	inline reg shuff<int>(const reg v, const reg cm) {
		uint8x8x2_t v2 = {vget_low_u8((uint8x16_t)v), vget_high_u8((uint8x16_t)v)};
		uint8x8_t low  = vtbl2_u8(v2, vget_low_u8 ((uint8x16_t)cm));
		uint8x8_t high = vtbl2_u8(v2, vget_high_u8((uint8x16_t)cm));

		return (reg)vcombine_u8(low, high);	
	}

	template <>
	inline reg shuff<short>(const reg v, const reg cm) {
		uint8x8x2_t v2 = {vget_low_u8((uint8x16_t)v), vget_high_u8((uint8x16_t)v)};
		uint8x8_t low  = vtbl2_u8(v2, vget_low_u8 ((uint8x16_t)cm));
		uint8x8_t high = vtbl2_u8(v2, vget_high_u8((uint8x16_t)cm));

		return (reg)vcombine_u8(low, high);
	}

	template <>
	inline reg shuff<signed char>(const reg v, const reg cm) {
		uint8x8x2_t v2 = {vget_low_u8((uint8x16_t)v), vget_high_u8((uint8x16_t)v)};
		uint8x8_t low  = vtbl2_u8(v2, vget_low_u8 ((uint8x16_t)cm));
		uint8x8_t high = vtbl2_u8(v2, vget_high_u8((uint8x16_t)cm));

		return (reg)vcombine_u8(low, high);
	}

	// --------------------------------------------------------------------------------------------------------- shuff2
	template <>
	inline reg shuff2<float>(const reg v, const reg cm) {
		return mipp::shuff<float>(v, cm);
	}

	template <>
	inline reg shuff2<int>(const reg v, const reg cm) {
		return mipp::shuff<int>(v, cm);	
	}

	template <>
	inline reg shuff2<short>(const reg v, const reg cm) {
		return mipp::shuff<short>(v, cm);
	}

	template <>
	inline reg shuff2<signed char>(const reg v, const reg cm) {
		return mipp::shuff<signed char>(v, cm);
	}

	// --------------------------------------------------------------------------------------------------- interleavelo
	template <>
	inline reg interleavelo<long long>(const reg v1, const reg v2) {
		// v1  = [a0, b0], v2 = [a1, b1]
		// res = [a0, a1]
		return (reg)vcombine_u64(vget_low_u64((uint64x2_t)v1), vget_low_u64((uint64x2_t)v2));
	}

	template <>
	inline reg interleavelo<float>(const reg v1, const reg v2) {
		// v1  = [a0, b0, c0, d0], v2 = [a1, b1, c1, d1]
		// res = [a0, a1, b0, b1]
		uint32x2x2_t res = vzip_u32(vget_low_u32((uint32x4_t)v1), vget_low_u32((uint32x4_t)v2));
		return (reg)vcombine_u32(res.val[0], res.val[1]);
	}

	template <>
	inline reg interleavelo<int>(const reg v1, const reg v2) {
		// v1  = [a0, b0, c0, d0], v2 = [a1, b1, c1, d1]
		// res = [a0, a1, b0, b1]
		uint32x2x2_t res = vzip_u32(vget_low_u32((uint32x4_t)v1), vget_low_u32((uint32x4_t)v2));
		return (reg)vcombine_u32(res.val[0], res.val[1]);
	}

	template <>
	inline reg interleavelo<short>(const reg v1, const reg v2) {
		uint16x4x2_t res = vzip_u16(vget_low_u16((uint16x8_t)v1), vget_low_u16((uint16x8_t)v2));
		return (reg)vcombine_u16(res.val[0], res.val[1]);
	}

	template <>
	inline reg interleavelo<signed char>(const reg v1, const reg v2) {
		uint8x8x2_t res = vzip_u8(vget_low_u8((uint8x16_t)v1), vget_low_u8((uint8x16_t)v2));
		return (reg)vcombine_u8(res.val[0], res.val[1]);
	}

	// --------------------------------------------------------------------------------------------------- interleavehi
	template <>
	inline reg interleavehi<long long>(const reg v1, const reg v2) {
		// v1  = [a0, b0], v2 = [a1, b1]
		// res = [b0, b1]
		return (reg)vcombine_u64(vget_high_u64((uint64x2_t)v1), vget_high_u64((uint64x2_t)v2));
	}

	template <>
	inline reg interleavehi<float>(const reg v1, const reg v2) {
		// v1  = [a0, b0, c0, d0], v2 = [a1, b1, c1, d1]
		// res = [c0, c1, d0, d1]
		uint32x2x2_t res = vzip_u32(vget_high_u32((uint32x4_t)v1), vget_high_u32((uint32x4_t)v2));
		return (reg)vcombine_u32(res.val[0], res.val[1]);
	}	

	template <>
	inline reg interleavehi<int>(const reg v1, const reg v2) {
		// v1  = [a0, b0, c0, d0], v2 = [a1, b1, c1, d1]
		// res = [c0, c1, d0, d1]
		uint32x2x2_t res = vzip_u32(vget_high_u32((uint32x4_t)v1), vget_high_u32((uint32x4_t)v2));
		return (reg)vcombine_u32(res.val[0], res.val[1]);
	}

	template <>
	inline reg interleavehi<short>(const reg v1, const reg v2) {
		uint16x4x2_t res = vzip_u16(vget_high_u16((uint16x8_t)v1), vget_high_u16((uint16x8_t)v2));
		return (reg)vcombine_u16(res.val[0], res.val[1]);
	}

	template <>
	inline reg interleavehi<signed char>(const reg v1, const reg v2) {
		uint8x8x2_t res = vzip_u8(vget_high_u8((uint8x16_t)v1), vget_high_u8((uint8x16_t)v2));
		return (reg)vcombine_u8(res.val[0], res.val[1]);
	}

	// ----------------------------------------------------------------------------------------------------- interleave
	template <>
	inline regx2 interleave<long long>(const reg v1, const reg v2) {
		// v1         = [a0, b0], v2         = [a1, b1]
		// res.val[0] = [a0, a1], res.val[1] = [b0, b1]
		regx2 res = {{(reg)vcombine_u64(vget_low_u64 ((uint64x2_t)v1), vget_low_u64 ((uint64x2_t)v2)),
		              (reg)vcombine_u64(vget_high_u64((uint64x2_t)v1), vget_high_u64((uint64x2_t)v2))}};
		return res;
	}

	template <>
	inline regx2 interleave<float>(const reg v1, const reg v2) {
		// v1         = [a0, b0, c0, d0], v2         = [a1, b1, c1, d1]
		// res.val[0] = [a0, a1, b0, b1], res.val[1] = [c0, c1, d0, d1]
		uint32x2x2_t res0 = vzip_u32(vget_low_u32 ((uint32x4_t)v1), vget_low_u32 ((uint32x4_t)v2));
		uint32x2x2_t res1 = vzip_u32(vget_high_u32((uint32x4_t)v1), vget_high_u32((uint32x4_t)v2));

		regx2 res = {{(reg)vcombine_u32(res0.val[0], res0.val[1]),
		              (reg)vcombine_u32(res1.val[0], res1.val[1])}};
		
		return res;
	}

	template <>
	inline regx2 interleave<int>(const reg v1, const reg v2) {
		// v1  = [a0, b0, c0, d0], v2 = [a1, b1, c1, d1]
		// res = [a0, a1, b0, b1]
		uint32x2x2_t res0 = vzip_u32(vget_low_u32 ((uint32x4_t)v1), vget_low_u32 ((uint32x4_t)v2));
		uint32x2x2_t res1 = vzip_u32(vget_high_u32((uint32x4_t)v1), vget_high_u32((uint32x4_t)v2));

		regx2 res = {{(reg)vcombine_u32(res0.val[0], res0.val[1]),
		              (reg)vcombine_u32(res1.val[0], res1.val[1])}};
		
		return res;
	}

	template <>
	inline regx2 interleave<short>(const reg v1, const reg v2) {
		uint16x4x2_t res0 = vzip_u16(vget_low_u16 ((uint16x8_t)v1), vget_low_u16 ((uint16x8_t)v2));
		uint16x4x2_t res1 = vzip_u16(vget_high_u16((uint16x8_t)v1), vget_high_u16((uint16x8_t)v2));

		regx2 res = {{(reg)vcombine_u16(res0.val[0], res0.val[1]),
		              (reg)vcombine_u16(res1.val[0], res1.val[1])}};
		
		return res;
	}

	template <>
	inline regx2 interleave<signed char>(const reg v1, const reg v2) {
		uint8x8x2_t res0 = vzip_u8(vget_low_u8 ((uint8x16_t)v1), vget_low_u8 ((uint8x16_t)v2));
		uint8x8x2_t res1 = vzip_u8(vget_high_u8((uint8x16_t)v1), vget_high_u8((uint8x16_t)v2));

		regx2 res = {{(reg)vcombine_u8(res0.val[0], res0.val[1]),
		              (reg)vcombine_u8(res1.val[0], res1.val[1])}};
		
		return res;
	}

	// ------------------------------------------------------------------------------------------------------ transpose
	template <>
	inline void transpose<short>(reg tab[nElReg<short>()]) {
		// /!\ this implementation can be further improved by using the dedicated VTRN instructions.
		//
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

		auto ab = mipp::interleave<short>(tab[0], tab[1]);
		auto cd = mipp::interleave<short>(tab[2], tab[3]);
		auto ef = mipp::interleave<short>(tab[4], tab[5]);
		auto gh = mipp::interleave<short>(tab[6], tab[7]);

		auto a03b03 = ab.val[0];
		auto c03d03 = cd.val[0];
		auto e03f03 = ef.val[0];
		auto g03h03 = gh.val[0];
		auto a47b47 = ab.val[1];
		auto c47d47 = cd.val[1];
		auto e47f47 = ef.val[1];
		auto g47h47 = gh.val[1];

		auto a03b03c03d03 = mipp::interleave<int>(a03b03, c03d03);
		auto e03f03g03h03 = mipp::interleave<int>(e03f03, g03h03);
		auto a47b47c47d47 = mipp::interleave<int>(a47b47, c47d47);
		auto e47f47g47h47 = mipp::interleave<int>(e47f47, g47h47);

		auto a01b01c01d01 = a03b03c03d03.val[0];
		auto a23b23c23d23 = a03b03c03d03.val[1];
		auto e01f01g01h01 = e03f03g03h03.val[0];
		auto e23f23g23h23 = e03f03g03h03.val[1];
		auto a45b45c45d45 = a47b47c47d47.val[0];
		auto a67b67c67d67 = a47b47c47d47.val[1];
		auto e45f45g45h45 = e47f47g47h47.val[0];
		auto e67f67g67h67 = e47f47g47h47.val[1];

		auto a01b01c01d01e01f01g01h01 = mipp::interleave<long long>(a01b01c01d01, e01f01g01h01);
		auto a23b23c23d23e23f23g23h23 = mipp::interleave<long long>(a23b23c23d23, e23f23g23h23);
		auto a45b45c45d45e45f45g45h45 = mipp::interleave<long long>(a45b45c45d45, e45f45g45h45);
		auto a67b67c67d67e67f67g67h67 = mipp::interleave<long long>(a67b67c67d67, e67f67g67h67);

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

	// --------------------------------------------------------------------------------------------------- transpose8x8
	template <>
	inline void transpose8x8<signed char>(reg tab[8]) {
		mipp::transpose<short>(tab);
	}

	// ----------------------------------------------------------------------------------------------------------- andb
	template <>
	inline reg andb<float>(const reg v1, const reg v2) {
		return (reg) vandq_u32((uint32x4_t) v1, (uint32x4_t) v2);
	}

	template <>
	inline reg andb<int>(const reg v1, const reg v2) {
		return (reg) vandq_u32((uint32x4_t) v1, (uint32x4_t) v2);
	}

	template <>
	inline reg andb<short>(const reg v1, const reg v2) {
		return (reg) vandq_u16((uint16x8_t) v1, (uint16x8_t) v2);
	}

	template <>
	inline reg andb<signed char>(const reg v1, const reg v2) {
		return (reg) vandq_u8((uint8x16_t) v1, (uint8x16_t) v2);
	}

	// ----------------------------------------------------------------------------------------------------------- notb

	template <>
	inline reg notb<float>(const reg v) {
		return (reg) vmvnq_u32((uint32x4_t) v);
	}

	template <>
	inline reg notb<int>(const reg v) {
		return (reg) vmvnq_u32((uint32x4_t) v);
	}

	template <>
	inline reg notb<short>(const reg v) {
		return (reg) vmvnq_u16((uint16x8_t) v);
	}

	template <>
	inline reg notb<signed char>(const reg v) {
		return (reg) vmvnq_u8((uint8x16_t) v);
	}

	// ---------------------------------------------------------------------------------------------------------- andnb
	template <>
	inline reg andnb<float>(const reg v1, const reg v2) {
		return (reg) vandq_u32(vmvnq_u32((uint32x4_t) v1), (uint32x4_t) v2);
	}

	template <>
	inline reg andnb<int>(const reg v1, const reg v2) {
		return (reg) vandq_u32(vmvnq_u32((uint32x4_t) v1), (uint32x4_t) v2);
	}

	template <>
	inline reg andnb<short>(const reg v1, const reg v2) {
		return (reg) vandq_u16(vmvnq_u16((uint16x8_t) v1), (uint16x8_t) v2);
	}

	template <>
	inline reg andnb<signed char>(const reg v1, const reg v2) {
		return (reg) vandq_u8(vmvnq_u8((uint8x16_t) v1), (uint8x16_t) v2);
	}

	// ------------------------------------------------------------------------------------------------------------ orb
	template <>
	inline reg orb<float>(const reg v1, const reg v2) {
		return (reg) vorrq_u32((uint32x4_t) v1, (uint32x4_t) v2);
	}

	template <>
	inline reg orb<int>(const reg v1, const reg v2) {
		return (reg) vorrq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	template <>
	inline reg orb<short>(const reg v1, const reg v2) {
		return (reg) vorrq_s16((int16x8_t) v1, (int16x8_t) v2);
	}

	template <>
	inline reg orb<signed char>(const reg v1, const reg v2) {
		return (reg) vorrq_s8((int8x16_t) v1, (int8x16_t) v2);
	}

	// ----------------------------------------------------------------------------------------------------------- xorb
	template <>
	inline reg xorb<float>(const reg v1, const reg v2) {
		return (reg) veorq_u32((uint32x4_t) v1, (uint32x4_t) v2);
	}

	template <>
	inline reg xorb<int>(const reg v1, const reg v2) {
		return (reg) veorq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	template <>
	inline reg xorb<short>(const reg v1, const reg v2) {
		return (reg) veorq_s16((int16x8_t) v1, (int16x8_t) v2);
	}

	template <>
	inline reg xorb<signed char>(const reg v1, const reg v2) {
		return (reg) veorq_s8((int8x16_t) v1, (int8x16_t) v2);
	}

	// --------------------------------------------------------------------------------------------------------- lshift
	template <>
	inline reg lshift<int>(const reg v1, const int n) {
		return (reg) vshlq_u32((uint32x4_t) v1, (int32x4_t)mipp::set1<int>(n));
	}

	template <>
	inline reg lshift<short>(const reg v1, const int n) {
		return (reg) vshlq_u16((uint16x8_t) v1, (int16x8_t)mipp::set1<short>((short) n));
	}

	template <>
	inline reg lshift<signed char>(const reg v1, const int n) {
		return (reg) vshlq_u8((uint8x16_t) v1, (int8x16_t)mipp::set1<signed char>((signed char) n));
	}

	// --------------------------------------------------------------------------------------------------------- rshift
	template <>
	inline reg rshift<int>(const reg v1, const int n) {
		return (reg) vshlq_u32((uint32x4_t) v1, (int32x4_t)mipp::set1<int>(-n));
	}

	template <>
	inline reg rshift<short>(const reg v1, const int n) {
		return (reg) vshlq_u16((uint16x8_t) v1, (int16x8_t)mipp::set1<short>((short)-n));
	}

	template <>
	inline reg rshift<signed char>(const reg v1, const int n) {
		return (reg) vshlq_u8((uint8x16_t) v1, (int8x16_t)mipp::set1<signed char>((signed char)-n));
	}

	// ---------------------------------------------------------------------------------------------------------- cmpeq
	template <>
	inline reg cmpeq<float>(const reg v1, const reg v2) {
		return (reg) vceqq_f32(v1, v2);
	}

	template <>
	inline reg cmpeq<int>(const reg v1, const reg v2) {
		return (reg) vceqq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	template <>
	inline reg cmpeq<short>(const reg v1, const reg v2) {
		return (reg) vceqq_s16((int16x8_t) v1, (int16x8_t) v2);
	}

	template <>
	inline reg cmpeq<signed char>(const reg v1, const reg v2) {
		return (reg) vceqq_s8((int8x16_t) v1, (int8x16_t) v2);
	}

	// --------------------------------------------------------------------------------------------------------- cmpneq
	template <>
	inline reg cmpneq<float>(const reg v1, const reg v2) {
		return (reg) vmvnq_u32((uint32x4_t) vceqq_f32(v1, v2));
	}

	template <>
	inline reg cmpneq<int>(const reg v1, const reg v2) {
		return (reg) vmvnq_u32((uint32x4_t) vceqq_s32((int32x4_t) v1, (int32x4_t) v2));
	}

	template <>
	inline reg cmpneq<short>(const reg v1, const reg v2) {
		return (reg) vmvnq_u16((uint16x8_t) vceqq_s16((int16x8_t) v1, (int16x8_t) v2));
	}

	template <>
	inline reg cmpneq<signed char>(const reg v1, const reg v2) {
		return (reg) vmvnq_u8((uint8x16_t) vceqq_s8((int8x16_t) v1, (int8x16_t) v2));
	}

	// ---------------------------------------------------------------------------------------------------------- cmplt
	template <>
	inline reg cmplt<float>(const reg v1, const reg v2) {
		return (reg) vcltq_f32(v1, v2);
	}

	template <>
	inline reg cmplt<int>(const reg v1, const reg v2) {
		return (reg) vcltq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	template <>
	inline reg cmplt<short>(const reg v1, const reg v2) {
		return (reg) vcltq_s16((int16x8_t) v1, (int16x8_t) v2);
	}

	template <>
	inline reg cmplt<signed char>(const reg v1, const reg v2) {
		return (reg) vcltq_s8((int8x16_t) v1, (int8x16_t) v2);
	}

	// ---------------------------------------------------------------------------------------------------------- cmple
	template <>
	inline reg cmple<float>(const reg v1, const reg v2) {
		return (reg) vcleq_f32(v1, v2);
	}

	template <>
	inline reg cmple<int>(const reg v1, const reg v2) {
		return (reg) vcleq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	template <>
	inline reg cmple<short>(const reg v1, const reg v2) {
		return (reg) vcleq_s16((int16x8_t) v1, (int16x8_t) v2);
	}

	template <>
	inline reg cmple<signed char>(const reg v1, const reg v2) {
		return (reg) vcleq_s8((int8x16_t) v1, (int8x16_t) v2);
	}

	// ---------------------------------------------------------------------------------------------------------- cmpgt
	template <>
	inline reg cmpgt<float>(const reg v1, const reg v2) {
		return (reg) vcgtq_f32(v1, v2);
	}

	template <>
	inline reg cmpgt<int>(const reg v1, const reg v2) {
		return (reg) vcgtq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	template <>
	inline reg cmpgt<short>(const reg v1, const reg v2) {
		return (reg) vcgtq_s16((int16x8_t) v1, (int16x8_t) v2);
	}

	template <>
	inline reg cmpgt<signed char>(const reg v1, const reg v2) {
		return (reg) vcgtq_s8((int8x16_t) v1, (int8x16_t) v2);
	}
	
	// ---------------------------------------------------------------------------------------------------------- cmpge
	template <>
	inline reg cmpge<float>(const reg v1, const reg v2) {
		return (reg) vcgeq_f32(v1, v2);
	}

	template <>
	inline reg cmpge<int>(const reg v1, const reg v2) {
		return (reg) vcgeq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	template <>
	inline reg cmpge<short>(const reg v1, const reg v2) {
		return (reg) vcgeq_s16((int16x8_t) v1, (int16x8_t) v2);
	}

	template <>
	inline reg cmpge<signed char>(const reg v1, const reg v2) {
		return (reg) vcgeq_s8((int8x16_t) v1, (int8x16_t) v2);
	}

	// ------------------------------------------------------------------------------------------------------------ add
	template <>
	inline reg add<float>(const reg v1, const reg v2) {
		return vaddq_f32(v1, v2);
	}

	template <>
	inline reg add<int>(const reg v1, const reg v2) {
		return (reg) vaddq_s32((int32x4_t) v1, (int32x4_t)v2);
	}

	template <>
	inline reg add<short>(const reg v1, const reg v2) {
		return (reg) vqaddq_s16((int16x8_t) v1, (int16x8_t)v2);
	}

	template <>
	inline reg add<signed char>(const reg v1, const reg v2) {
		return (reg) vqaddq_s8((int8x16_t) v1, (int8x16_t)v2);
	}

	// ------------------------------------------------------------------------------------------------------------ sub
	template <>
	inline reg sub<float>(const reg v1, const reg v2) {
		return vsubq_f32(v1, v2);
	}

	template <>
	inline reg sub<int>(const reg v1, const reg v2) {
		return (reg) vsubq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	template <>
	inline reg sub<short>(const reg v1, const reg v2) {
		return (reg) vqsubq_s16((int16x8_t) v1, (int16x8_t) v2);
	}

	template <>
	inline reg sub<signed char>(const reg v1, const reg v2) {
		return (reg) vqsubq_s8((int8x16_t) v1, (int8x16_t) v2);
	}

	// ------------------------------------------------------------------------------------------------------------ mul
	template <>
	inline reg mul<float>(const reg v1, const reg v2) {
		return vmulq_f32(v1, v2);
	}

	template <>
	inline reg mul<int>(const reg v1, const reg v2) {
		return (reg) vmulq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	// ------------------------------------------------------------------------------------------------------------ div
	template <>
	inline reg div<float>(const reg v1, const reg v2) {
		return mul<float>(v1, vrecpeq_f32(v2));
	}

	// ------------------------------------------------------------------------------------------------------------ min
	template <>
	inline reg min<float>(const reg v1, const reg v2) {
		return vminq_f32(v1, v2);
	}

	template <>
	inline reg min<int>(const reg v1, const reg v2) {
		return (reg) vminq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	template <>
	inline reg min<short>(const reg v1, const reg v2) {
		return (reg) vminq_s16((int16x8_t) v1, (int16x8_t) v2);
	}

	template <>
	inline reg min<signed char>(const reg v1, const reg v2) {
		return (reg) vminq_s8((int8x16_t) v1, (int8x16_t) v2);
	}

	// ------------------------------------------------------------------------------------------------------------ max
	template <>
	inline reg max<float>(const reg v1, const reg v2) {
		return vmaxq_f32(v1, v2);
	}

	template <>
	inline reg max<int>(const reg v1, const reg v2) {
		return (reg) vmaxq_s32((int32x4_t) v1, (int32x4_t) v2);
	}

	template <>
	inline reg max<short>(const reg v1, const reg v2) {
		return (reg) vmaxq_s16((int16x8_t) v1, (int16x8_t) v2);
	}

	template <>
	inline reg max<signed char>(const reg v1, const reg v2) {
		return (reg) vmaxq_s8((int8x16_t) v1, (int8x16_t) v2);
	}

	// ----------------------------------------------------------------------------------------------------------- sign
	template <>
	inline reg sign<float>(const reg v1) {
		// sign_mask = 10000000000000000000000000000000 // 32 bits
		const reg sign_mask = set1<int>(0x80000000);

		// indices   = 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
		// sign_mask =  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
		// v1        =  ù  €  è  é  à  &  z  y  x  w  v  u  t  s  r  q  p  o  n  m  l  k  j  i  h  g  f  e  d  c  b  a
		// res       =  ù  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
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
	inline reg sign<short>(const reg v1) {
		const reg sign_mask = set1<short>(0x8000);
		return andb<short>(v1, sign_mask);
	}

	template <>
	inline reg sign<short>(const reg v1, const reg v2) {
		reg sign_v1_v2 = xorb<short>(v1, v2);
		    sign_v1_v2 = sign<short>(sign_v1_v2);
		return sign_v1_v2;
	}

	template <>
	inline reg sign<signed char>(const reg v1) {
		const reg sign_mask = set1<signed char>(0x80);
		return andb<signed char>(v1, sign_mask);
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
	inline reg neg<short>(const reg v1, const reg v2) {
		reg neg_v1 = (reg) vqnegq_s16((int16x8_t) v1);
		reg v2_2   = orb  <short>(v2, set1<short>(1)); // hack to avoid -0 case
		reg mask   = cmplt<short>(v2_2, set0<short>());
		reg res1   = andb <short>(mask, neg_v1);
		reg res2   = andb <short>((reg) vmvnq_s16((int16x8_t) mask), v1);
		reg res    = orb  <short>(res1, res2);
		return res;
	}

	template <>
	inline reg neg<signed char>(const reg v1, const reg v2) {
		reg neg_v1 = (reg) vqnegq_s8((int8x16_t) v1);
		reg v2_2   = orb  <signed char>(v2, set1<signed char>(1)); // hack to avoid -0 case
		reg mask   = cmplt<signed char>(v2_2, set0<signed char>());
		reg res1   = andb <signed char>(mask, neg_v1);
		reg res2   = andb <signed char>((reg) vmvnq_s8((int8x16_t) mask), v1);
		reg res    = orb  <signed char>(res1, res2);
		return res;
	}

	// ------------------------------------------------------------------------------------------------------------ neg
	template <>
	inline reg neg<float>(const reg v) {
		return xorb<int>(v, mipp::set1<int>(0x80000000));
	}

	template <>
	inline reg neg<int>(const reg v) {
		return (reg) vqnegq_s32((int32x4_t) v);
	}

	template <>
	inline reg neg<short>(const reg v) {
		return (reg) vqnegq_s16((int16x8_t) v);
	}

	template <>
	inline reg neg<signed char>(const reg v) {
		return (reg) vqnegq_s8((int8x16_t) v);
	}

	// ------------------------------------------------------------------------------------------------------------ abs
	template <>
	inline reg abs<float>(const reg v1) {
		return vabsq_f32(v1);
	}

	template <>
	inline reg abs<short>(const reg v1) {
		return (reg) vabsq_s16((int16x8_t)v1);
	}

	template <>
	inline reg abs<signed char>(const reg v1) {
		return (reg) vabsq_s8((int8x16_t)v1);
	}

	// ---------------------------------------------------------------------------------------------------------- rsqrt
	template <>
	inline reg rsqrt<float>(const reg v1) {
		return vrsqrteq_f32(v1);
	}

	// ------------------------------------------------------------------------------------------------------------ log
	template <>
	inline reg log<float>(const reg v) {
		auto v_bis = v;
		return (reg) log_ps(v_bis);
	}

	// ------------------------------------------------------------------------------------------------------------ exp
	template <>
	inline reg exp<float>(const reg v) {
		auto v_bis = v;
		return (reg) exp_ps(v_bis);
	}

	// ------------------------------------------------------------------------------------------------------------ sin
	template <>
	inline reg sin<float>(const reg v) {
		auto v_bis = v;
		return (reg) sin_ps(v_bis);
	}

	// ------------------------------------------------------------------------------------------------------------ cos
	template <>
	inline reg cos<float>(const reg v) {
		auto v_bis = v;
		return (reg) cos_ps(v_bis);
	}

	// --------------------------------------------------------------------------------------------------------- sincos
	template <>
	inline void sincos<float>(const reg x, reg &s, reg &c) {
		sincos_ps(x, &s, &c);
	}

	// ----------------------------------------------------------------------------------------------------------- sqrt
	template <>
	inline reg sqrt<float>(const reg v1) {
		return vrecpeq_f32(rsqrt<float>(v1));
	}

	// ---------------------------------------------------------------------------------------------------------- fmadd
	template <>
	inline reg fmadd<float>(const reg v1, const reg v2, const reg v3) {
		return add<float>(v3, mul<float>(v1, v2));
	}

	// --------------------------------------------------------------------------------------------------------- fnmadd
	template <>
	inline reg fnmadd<float>(const reg v1, const reg v2, const reg v3) {
		return sub<float>(v3, mul<float>(v1, v2));
	}


	// ---------------------------------------------------------------------------------------------------------- fmsub
	template <>
	inline reg fmsub<float>(const reg v1, const reg v2, const reg v3) {
		return sub<float>(mul<float>(v1, v2), v3);
	}

	// ---------------------------------------------------------------------------------------------------------- blend
	template <>
	inline reg blend<float>(const reg v1, const reg v2, const reg m) {
		return (float32x4_t)vbslq_f32((uint32x4_t)m, (float32x4_t)v1, (float32x4_t)v2);
	}

	template <>
	inline reg blend<int>(const reg v1, const reg v2, const reg m) {
		return (float32x4_t)vbslq_u32((uint32x4_t)m, (uint32x4_t)v1, (uint32x4_t)v2);
	}

	template <>
	inline reg blend<short>(const reg v1, const reg v2, const reg m) {
		return (float32x4_t)vbslq_u16((uint16x8_t)m, (uint16x8_t)v1, (uint16x8_t)v2);
	}

	template <>
	inline reg blend<signed char>(const reg v1, const reg v2, const reg m) {
		return (float32x4_t)vbslq_u8((uint8x16_t)m, (uint8x16_t)v1, (uint8x16_t)v2);
	}

	// ------------------------------------------------------------------------------------------------------------ rot
	template <>
	inline reg rot<float>(const reg v1) {
		// make a rotation in:[3, 2 , 1, 0] => out:[2, 1, 0, 3]
		return vextq_f32(v1, v1, 3);
	}

	template <>
	inline reg rot<short>(const reg v1) {
		// make a rotation in:[7, 6, 5, 4, 3, 2 , 1, 0] => out:[6, 5, 4, 3, 3, 1, 0, 7]
		return (reg)vextq_u16((uint16x8_t)v1, (uint16x8_t)v1, 7);
	}

	// ----------------------------------------------------------------------------------------------------------- rotr
	template <>
	inline reg rotr<float>(const reg v1) {
		// make a rotation in:[3, 2 , 1, 0] => out:[0, 3, 2, 1]
		return vextq_f32(v1, v1, 1);
	}

	template <>
	inline reg rotr<short>(const reg v1) {
		// make a rotation in:[7, 6, 5, 4, 3, 2 , 1, 0] => out:[0, 7, 6, 5, 4, 3, 2, 1]
		return (reg)vextq_u16((uint16x8_t)v1, (uint16x8_t)v1, 1);
	}

	// ----------------------------------------------------------------------------------------------------------- div2
	template <>
	inline reg div2<float>(const reg v1) {
		return mul<float>(v1, set1<float>(0.5f));
	}

	template <>
	inline reg div2<int>(const reg v1) {
		reg abs_v1 = abs<int>(v1);
		reg sh = rshift<int>(abs_v1, 1);
		return neg<int>(sh, v1);
	}

	template <>
	inline reg div2<short>(const reg v1) {
		reg abs_v1 = abs<short>(v1);
		reg sh = rshift<short>(abs_v1, 1);
		return neg<short>(sh, v1);
	}

	template <>
	inline reg div2<signed char>(const reg v1) {
		reg abs_v1 = abs<signed char>(v1);
		reg sh = rshift<signed char>(abs_v1, 1);
		return neg<signed char>(sh, v1);
	}

	// ----------------------------------------------------------------------------------------------------------- div4
	template <>
	inline reg div4<float>(const reg v1) {
		return mul<float>(v1, set1<float>(0.25f));
	}

	template <>
	inline reg div4<int>(const reg v1) {
		reg abs_v1 = abs<int>(v1);
		reg sh = rshift<int>(abs_v1, 2);
		return neg<int>(sh, v1); 
	}    

	template <>
	inline reg div4<short>(const reg v1) {
		reg abs_v1 = abs<short>(v1);
		reg sh = rshift<short>(abs_v1, 2);
		return neg<short>(sh, v1); 
	}    

	template <>
	inline reg div4<signed char>(const reg v1) {
		reg abs_v1 = abs<signed char>(v1);
		reg sh = rshift<signed char>(abs_v1, 2);
		return neg<signed char>(sh, v1); 
	}

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

	// ------------------------------------------------------------------------------------------------------------ cvt
	template <>
	inline reg cvt<float,int>(const reg v) {
		return (reg) vcvtq_s32_f32((float32x4_t) v);
	}

	template <>
	inline reg cvt<int,float>(const reg v) {
		return (reg) vcvtq_f32_s32((int32x4_t) v);
	}

	template <>
	inline reg cvt<signed char,short>(const reg_2 v) {
		return (reg) vmovl_s8((int8x8_t) v);
	}

	template <>
	inline reg cvt<short,int>(const reg_2 v) {
		return (reg) vmovl_s16((int16x4_t) v);
	}

	// ---------------------------------------------------------------------------------------------------------- round
	template <>
	inline reg round<float>(const reg v) {
		auto half = mipp::orb<float>(mipp::sign<float>(v), mipp::set1<float>(0.5f));
		auto tmp = mipp::add<float>(v, half);
		return mipp::cvt<int,float>(mipp::cvt<float,int>(tmp));
	}

	// ----------------------------------------------------------------------------------------------------------- pack
	template <>
	inline reg pack<int,short>(const reg v1, const reg v2) {
		return (reg) vcombine_s16(vqmovn_s32((int32x4_t) v1), vqmovn_s32((int32x4_t) v2));
	}

	template <>
	inline reg pack<short,signed char>(const reg v1, const reg v2) {
		return (reg) vcombine_s8(vqmovn_s16((int16x8_t) v1), vqmovn_s16((int16x8_t) v2));
	}

	// ------------------------------------------------------------------------------------------------------ reduction
	template <red_op<float> OP>
	struct _reduction<float,OP>
	{
		static reg apply(const reg v1) {
			auto val = v1;

			val = OP(val, (reg) vextq_f32(val, val, 2));

			float32x2_t low1  = vrev64_f32(vget_low_f32 (val));
			float32x2_t high1 = vrev64_f32(vget_high_f32(val));
			val = OP(val, vcombine_f32(low1, high1));

			return val;
		}
	};

	template <Red_op<float> OP>
	struct _Reduction<float,OP>
	{
		static Reg<float> apply(const Reg<float> v1) {
			auto val = v1;

			val = OP(val, Reg<float>((reg) vextq_f32(val.r, val.r, 2)));

			float32x2_t low1  = vrev64_f32(vget_low_f32 (val.r));
			float32x2_t high1 = vrev64_f32(vget_high_f32(val.r));
			val = OP(val, Reg<float>(vcombine_f32(low1, high1)));

			return val;
		}
	};

	template <red_op<int> OP>
	struct _reduction<int,OP>
	{
		static reg apply(const reg v1) {
			auto val = v1;

			val = OP(val, (reg) vextq_s32((int32x4_t) val, (int32x4_t) val, 2));

			int32x2_t low1  = vrev64_s32((int32x2_t) vget_low_s32 ((int32x4_t) val));
			int32x2_t high1 = vrev64_s32((int32x2_t) vget_high_s32((int32x4_t) val));
			val = OP(val, (reg) vcombine_s32((int32x2_t) low1, (int32x2_t) high1));

			return val;
		}
	};

	template <Red_op<int> OP>
	struct _Reduction<int,OP>
	{
		static Reg<int> apply(const Reg<int> v1) {
			auto val = v1;

			val = OP(val, Reg<int>((reg) vextq_s32((int32x4_t) val.r, (int32x4_t) val.r, 2)));

			int32x2_t low1  = vrev64_s32((int32x2_t) vget_low_s32 ((int32x4_t) val.r));
			int32x2_t high1 = vrev64_s32((int32x2_t) vget_high_s32((int32x4_t) val.r));
			val = OP(val, Reg<int>((reg) vcombine_s32((int32x2_t) low1, (int32x2_t) high1)));

			return val;
		}
	};

	template <red_op<short> OP>
	struct _reduction<short,OP>
	{
		static reg apply(const reg v1) {
			auto val = v1;

			val = OP(val, (reg) vextq_s32((int32x4_t) val, (int32x4_t) val, 2));

			int32x2_t low1  = vrev64_s32((int32x2_t) vget_low_s32 ((int32x4_t) val));
			int32x2_t high1 = vrev64_s32((int32x2_t) vget_high_s32((int32x4_t) val));
			val = OP(val, (reg) vcombine_s32((int32x2_t) low1, (int32x2_t) high1));

			int16x4_t low2  = vrev32_s16((int16x4_t) vget_low_s32((int32x4_t) val));
			int16x4_t high2 = vrev32_s16((int16x4_t) vget_high_s32((int32x4_t) val));
			val = OP(val, (reg) vcombine_s32((int32x2_t) low2, (int32x2_t) high2));

			return val;
		}
	};

	template <Red_op<short> OP>
	struct _Reduction<short,OP>
	{
		static Reg<short> apply(const Reg<short> v1) {
			auto val = v1;

			val = OP(val, Reg<short>((reg) vextq_s32((int32x4_t) val.r, (int32x4_t) val.r, 2)));

			int32x2_t low1  = vrev64_s32((int32x2_t) vget_low_s32 ((int32x4_t) val.r));
			int32x2_t high1 = vrev64_s32((int32x2_t) vget_high_s32((int32x4_t) val.r));
			val = OP(val, Reg<short>((reg) vcombine_s32((int32x2_t) low1, (int32x2_t) high1)));

			int16x4_t low2  = vrev32_s16((int16x4_t) vget_low_s32((int32x4_t) val.r));
			int16x4_t high2 = vrev32_s16((int16x4_t) vget_high_s32((int32x4_t) val.r));
			val = OP(val, Reg<short>((reg) vcombine_s32((int32x2_t) low2, (int32x2_t) high2)));

			return val;
		}
	};

	template <red_op<signed char> OP>
	struct _reduction<signed char,OP>
	{
		static reg apply(const reg v1) {
			auto val = v1;

			val = OP(val, (reg) vextq_s32((int32x4_t) val, (int32x4_t) val, 2));

			int32x2_t low1  = vrev64_s32((int32x2_t) vget_low_s32 ((int32x4_t) val));
			int32x2_t high1 = vrev64_s32((int32x2_t) vget_high_s32((int32x4_t) val));
			val = OP(val, (reg) vcombine_s32((int32x2_t) low1, (int32x2_t) high1));

			int16x4_t low2  = vrev32_s16((int16x4_t) vget_low_s32((int32x4_t) val));
			int16x4_t high2 = vrev32_s16((int16x4_t) vget_high_s32((int32x4_t) val));
			val = OP(val, (reg) vcombine_s32((int32x2_t) low2, (int32x2_t) high2));

			int8x8_t low3  = vrev16_s8((int8x8_t) vget_low_s32((int32x4_t) val));
			int8x8_t high3 = vrev16_s8((int8x8_t) vget_high_s32((int32x4_t) val));
			val = OP(val, (reg) vcombine_s32((int32x2_t) low3, (int32x2_t) high3));

			return val;
		}
	};

	template <Red_op<signed char> OP>
	struct _Reduction<signed char,OP>
	{
		static Reg<signed char> apply(const Reg<signed char> v1) {
			auto val = v1;

			val = OP(val, Reg<signed char>((reg) vextq_s32((int32x4_t) val.r, (int32x4_t) val.r, 2)));

			int32x2_t low1  = vrev64_s32((int32x2_t) vget_low_s32 ((int32x4_t) val.r));
			int32x2_t high1 = vrev64_s32((int32x2_t) vget_high_s32((int32x4_t) val.r));
			val = OP(val, Reg<signed char>((reg) vcombine_s32((int32x2_t) low1, (int32x2_t) high1)));

			int16x4_t low2  = vrev32_s16((int16x4_t) vget_low_s32((int32x4_t) val.r));
			int16x4_t high2 = vrev32_s16((int16x4_t) vget_high_s32((int32x4_t) val.r));
			val = OP(val, Reg<signed char>((reg) vcombine_s32((int32x2_t) low2, (int32x2_t) high2)));

			int8x8_t low3  = vrev16_s8((int8x8_t) vget_low_s32((int32x4_t) val.r));
			int8x8_t high3 = vrev16_s8((int8x8_t) vget_high_s32((int32x4_t) val.r));
			val = OP(val, Reg<signed char>((reg) vcombine_s32((int32x2_t) low3, (int32x2_t) high3)));

			return val;
		}
	};
#endif
