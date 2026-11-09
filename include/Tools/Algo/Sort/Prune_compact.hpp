/*!
 * \file
 * \brief Exact pruning of the SCL candidate set: threshold and branchless compaction of the survivors.
 *
 * The L paths of an SCL decoder produce L groups of candidates. One candidate per group gives L candidates whose
 * maximum, tau, bounds the L-th smallest candidate of the whole set: a candidate larger than tau is larger than at
 * least L others and cannot be selected. Computing tau costs one strided max-reduction.
 *
 * The compaction keeps the candidates not larger than tau in their original order, together with their positions.
 * It is branchless where the instruction set allows it: a native compress instruction on AVX-512 and RVV, a
 * permutation table on AVX2 and NEON. Fixed-point metrics are widened to 32-bit lanes on x86 and Arm. All variants
 * return the same survivors in the same order as the scalar loop.
 *
 * The kernels are written with intrinsics because the MIPP version used by AFF3CT (v1) is deprecated and MIPP v2
 * does not provide a compress primitive yet. They should be replaced by mipp::compress, together with a count of the
 * active lanes of a mask, once both exist in MIPP v2 and AFF3CT moves to it.
 */
#ifndef PRUNE_COMPACT_HPP
#define PRUNE_COMPACT_HPP

#include <cstddef>
#include <cstdint>

#if defined(__riscv_vector)
#include <riscv_vector.h>
#define AFF3CT_PRUNE_RVV
#elif defined(__aarch64__) && defined(__ARM_NEON) && !defined(_MSC_VER)
#include <arm_neon.h>
#define AFF3CT_PRUNE_NEON
#elif defined(__AVX512F__) && defined(__AVX512BW__) && defined(__AVX512VL__)
#include <immintrin.h>
#define AFF3CT_PRUNE_AVX512
#elif defined(__AVX2__)
#include <immintrin.h>
#define AFF3CT_PRUNE_AVX2
#endif
#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace aff3ct
{
namespace tools
{
namespace prune_detail
{
inline int
popcount(const unsigned m)
{
#if defined(_MSC_VER)
    return (int)__popcnt(m);
#else
    return __builtin_popcount(m);
#endif
}

// Scalar compaction of the positions c..nc-1, written from out_v[0] and out_i[0].
template<typename T>
inline int
compact_scalar(const T* v, int c, const int nc, const T t, T* out_v, int* out_i)
{
    int n = 0;
    for (; c < nc; c++)
        if (v[c] <= t)
        {
            out_v[n] = v[c];
            out_i[n] = c;
            n++;
        }
    return n;
}
} // namespace prune_detail

template<typename R>
struct prune_compact_base
{
    //! Largest of the L representatives v[0], v[stride], ..., v[(L-1) stride].
    static inline R tau(const R* v, const int L, const int stride)
    {
        R t = v[0];
        for (int p = 1; p < L; p++)
            if (v[stride * p] > t) t = v[stride * p];
        return t;
    }

    //! Keeps the nc candidates of v that are not larger than t, in order, and returns how many there are.
    static inline int compact_scalar(const R* v, const int nc, const R t, R* out_v, int* out_i)
    {
        return prune_detail::compact_scalar(v, 0, nc, t, out_v, out_i);
    }
};

/*!
 * \brief Threshold and compaction of a candidate set.
 *
 * compact() is the fastest available compaction. Its output arrays must hold at least nc + 16 elements, since the
 * vector kernels write full registers.
 */
template<typename R>
struct prune_compact : prune_compact_base<R>
{
    static inline int compact(const R* v, const int nc, const R t, R* out_v, int* out_i)
    {
        return prune_detail::compact_scalar(v, 0, nc, t, out_v, out_i);
    }

    static constexpr bool is_vectorized() { return false; }
};

#if defined(AFF3CT_PRUNE_RVV) || defined(AFF3CT_PRUNE_NEON) || defined(AFF3CT_PRUNE_AVX512) ||                         \
  defined(AFF3CT_PRUNE_AVX2)
namespace prune_detail
{
#if defined(AFF3CT_PRUNE_RVV)
// RVV compacts the metrics in their own width; the 32-bit positions use the same mask (SEW / LMUL = 8).
inline int
compact(const float* v, const int nc, const float t, float* out_v, int* out_i)
{
    int n = 0;
    for (int c = 0; c < nc;)
    {
        const size_t vl = __riscv_vsetvl_e32m4((size_t)(nc - c));
        const vfloat32m4_t x = __riscv_vle32_v_f32m4(v + c, vl);
        const vbool8_t m = __riscv_vmfle_vf_f32m4_b8(x, t, vl);
        const vuint32m4_t id = __riscv_vadd_vx_u32m4(__riscv_vid_v_u32m4(vl), (uint32_t)c, vl);
        const unsigned k = (unsigned)__riscv_vcpop_m_b8(m, vl);
        __riscv_vse32_v_f32m4(out_v + n, __riscv_vcompress_vm_f32m4(x, m, vl), k);
        __riscv_vse32_v_u32m4((uint32_t*)(out_i + n), __riscv_vcompress_vm_u32m4(id, m, vl), k);
        n += (int)k;
        c += (int)vl;
    }
    return n;
}

inline int
compact(const int16_t* v, const int nc, const int16_t t, int16_t* out_v, int* out_i)
{
    int n = 0;
    for (int c = 0; c < nc;)
    {
        const size_t vl = __riscv_vsetvl_e16m2((size_t)(nc - c));
        const vint16m2_t x = __riscv_vle16_v_i16m2(v + c, vl);
        const vbool8_t m = __riscv_vmsle_vx_i16m2_b8(x, t, vl);
        const vuint32m4_t id = __riscv_vadd_vx_u32m4(__riscv_vid_v_u32m4(vl), (uint32_t)c, vl);
        const unsigned k = (unsigned)__riscv_vcpop_m_b8(m, vl);
        __riscv_vse16_v_i16m2(out_v + n, __riscv_vcompress_vm_i16m2(x, m, vl), k);
        __riscv_vse32_v_u32m4((uint32_t*)(out_i + n), __riscv_vcompress_vm_u32m4(id, m, vl), k);
        n += (int)k;
        c += (int)vl;
    }
    return n;
}

inline int
compact(const int8_t* v, const int nc, const int8_t t, int8_t* out_v, int* out_i)
{
    int n = 0;
    for (int c = 0; c < nc;)
    {
        const size_t vl = __riscv_vsetvl_e8m1((size_t)(nc - c));
        const vint8m1_t x = __riscv_vle8_v_i8m1(v + c, vl);
        const vbool8_t m = __riscv_vmsle_vx_i8m1_b8(x, t, vl);
        const vuint32m4_t id = __riscv_vadd_vx_u32m4(__riscv_vid_v_u32m4(vl), (uint32_t)c, vl);
        const unsigned k = (unsigned)__riscv_vcpop_m_b8(m, vl);
        __riscv_vse8_v_i8m1(out_v + n, __riscv_vcompress_vm_i8m1(x, m, vl), k);
        __riscv_vse32_v_u32m4((uint32_t*)(out_i + n), __riscv_vcompress_vm_u32m4(id, m, vl), k);
        n += (int)k;
        c += (int)vl;
    }
    return n;
}
#elif defined(AFF3CT_PRUNE_NEON)
// Byte permutations that pack the selected 32-bit lanes of a 4-lane register, for the 16 masks.
struct lut4_t
{
    alignas(16) uint8_t p[16][16];
    lut4_t()
    {
        for (int m = 0; m < 16; m++)
        {
            int k = 0;
            for (int b = 0; b < 4; b++)
                if (m & (1 << b))
                {
                    for (int y = 0; y < 4; y++)
                        p[m][4 * k + y] = (uint8_t)(4 * b + y);
                    k++;
                }
            for (; k < 4; k++)
                for (int y = 0; y < 4; y++)
                    p[m][4 * k + y] = 0;
        }
    }
};

inline const lut4_t&
lut4()
{
    static const lut4_t l;
    return l;
}

// Packs the 4 lanes selected by le (positions c..c+3) and returns their number.
inline int
pack4(const uint8x16_t x, const uint32x4_t le, const int c, uint8x16_t* packed, int32x4_t* ids)
{
    const uint32x4_t bits = vsetq_lane_u32(8u, vsetq_lane_u32(4u, vsetq_lane_u32(2u, vdupq_n_u32(1u), 1), 2), 3);
    const int32x4_t iota = vsetq_lane_s32(3, vsetq_lane_s32(2, vsetq_lane_s32(1, vdupq_n_s32(0), 1), 2), 3);
    const unsigned m = vaddvq_u32(vandq_u32(le, bits));
    const uint8x16_t perm = vld1q_u8(lut4().p[m]);
    *packed = vqtbl1q_u8(x, perm);
    *ids = vreinterpretq_s32_u8(vqtbl1q_u8(vreinterpretq_u8_s32(vaddq_s32(iota, vdupq_n_s32(c))), perm));
    return popcount(m);
}

inline int
compact(const float* v, const int nc, const float t, float* out_v, int* out_i)
{
    const float32x4_t vt = vdupq_n_f32(t);
    int n = 0, c = 0;
    for (; c + 4 <= nc; c += 4)
    {
        const float32x4_t x = vld1q_f32(v + c);
        uint8x16_t p;
        int32x4_t id;
        const int k = pack4(vreinterpretq_u8_f32(x), vcleq_f32(x, vt), c, &p, &id);
        vst1q_f32(out_v + n, vreinterpretq_f32_u8(p));
        vst1q_s32(out_i + n, id);
        n += k;
    }
    return n + compact_scalar(v, c, nc, t, out_v + n, out_i + n);
}

inline int16x8_t
widen8(const int16_t* p)
{
    return vld1q_s16(p);
}

inline int16x8_t
widen8(const int8_t* p)
{
    return vmovl_s8(vld1_s8(p));
}

inline void
narrow4(int16_t* out, const int32x4_t x)
{
    vst1_s16(out, vmovn_s32(x));
}

inline void
narrow4(int8_t* out, const int32x4_t x) // writes 8 bytes, 4 of them meaningful
{
    vst1_s8(out, vmovn_s16(vcombine_s16(vmovn_s32(x), vdup_n_s16(0))));
}

template<typename T>
inline int
compact_fixed(const T* v, const int nc, const T t, T* out_v, int* out_i)
{
    const int32x4_t vt = vdupq_n_s32((int32_t)t);
    int n = 0, c = 0;
    for (; c + 8 <= nc; c += 8)
    {
        const int16x8_t w = widen8(v + c);
        const int32x4_t h[2] = { vmovl_s16(vget_low_s16(w)), vmovl_high_s16(w) };
        for (int j = 0; j < 2; j++)
        {
            uint8x16_t p;
            int32x4_t id;
            const int k = pack4(vreinterpretq_u8_s32(h[j]), vcleq_s32(h[j], vt), c + 4 * j, &p, &id);
            narrow4(out_v + n, vreinterpretq_s32_u8(p));
            vst1q_s32(out_i + n, id);
            n += k;
        }
    }
    return n + compact_scalar(v, c, nc, t, out_v + n, out_i + n);
}

inline int
compact(const int16_t* v, const int nc, const int16_t t, int16_t* out_v, int* out_i)
{
    return compact_fixed(v, nc, t, out_v, out_i);
}

inline int
compact(const int8_t* v, const int nc, const int8_t t, int8_t* out_v, int* out_i)
{
    return compact_fixed(v, nc, t, out_v, out_i);
}
#elif defined(AFF3CT_PRUNE_AVX512)
// Register form of the compress instructions (the memory form is microcoded on Zen 4).
inline int
compact(const float* v, const int nc, const float t, float* out_v, int* out_i)
{
    const __m512 vt = _mm512_set1_ps(t);
    const __m512i iota = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    int n = 0;
    for (int c = 0; c < nc; c += 16)
    {
        const int rem = nc - c;
        const __mmask16 ld = rem >= 16 ? (__mmask16)0xFFFF : (__mmask16)((1u << rem) - 1u);
        const __m512 x = _mm512_maskz_loadu_ps(ld, v + c);
        const __mmask16 m = _mm512_mask_cmp_ps_mask(ld, x, vt, _CMP_LE_OQ);
        _mm512_storeu_ps(out_v + n, _mm512_maskz_compress_ps(m, x));
        _mm512_storeu_si512((void*)(out_i + n),
                            _mm512_maskz_compress_epi32(m, _mm512_add_epi32(_mm512_set1_epi32(c), iota)));
        n += popcount((unsigned)m);
    }
    return n;
}

inline __m512i
widen16(const int16_t* p, const __mmask16 ld)
{
    return _mm512_cvtepi16_epi32(_mm256_maskz_loadu_epi16(ld, p));
}

inline __m512i
widen16(const int8_t* p, const __mmask16 ld)
{
    return _mm512_cvtepi8_epi32(_mm_maskz_loadu_epi8(ld, p));
}

inline void
narrow16(int16_t* out, const __m512i x)
{
    _mm256_storeu_si256((__m256i*)out, _mm512_cvtepi32_epi16(x));
}

inline void
narrow16(int8_t* out, const __m512i x)
{
    _mm_storeu_si128((__m128i*)out, _mm512_cvtepi32_epi8(x));
}

template<typename T>
inline int
compact_fixed(const T* v, const int nc, const T t, T* out_v, int* out_i)
{
    const __m512i vt = _mm512_set1_epi32((int32_t)t);
    const __m512i iota = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    int n = 0;
    for (int c = 0; c < nc; c += 16)
    {
        const int rem = nc - c;
        const __mmask16 ld = rem >= 16 ? (__mmask16)0xFFFF : (__mmask16)((1u << rem) - 1u);
        const __m512i x = widen16(v + c, ld);
        const __mmask16 m = _mm512_mask_cmple_epi32_mask(ld, x, vt);
        narrow16(out_v + n, _mm512_maskz_compress_epi32(m, x));
        _mm512_storeu_si512((void*)(out_i + n),
                            _mm512_maskz_compress_epi32(m, _mm512_add_epi32(_mm512_set1_epi32(c), iota)));
        n += popcount((unsigned)m);
    }
    return n;
}

inline int
compact(const int16_t* v, const int nc, const int16_t t, int16_t* out_v, int* out_i)
{
    return compact_fixed(v, nc, t, out_v, out_i);
}

inline int
compact(const int8_t* v, const int nc, const int8_t t, int8_t* out_v, int* out_i)
{
    return compact_fixed(v, nc, t, out_v, out_i);
}
#else // AFF3CT_PRUNE_AVX2
// Lane permutations that pack the selected 32-bit lanes of an 8-lane register, for the 256 masks.
struct lut8_t
{
    alignas(32) int32_t p[256][8];
    lut8_t()
    {
        for (int m = 0; m < 256; m++)
        {
            int k = 0;
            for (int b = 0; b < 8; b++)
                if (m & (1 << b)) p[m][k++] = b;
            for (; k < 8; k++)
                p[m][k] = 0;
        }
    }
};

inline const lut8_t&
lut8()
{
    static const lut8_t l;
    return l;
}

inline int
compact(const float* v, const int nc, const float t, float* out_v, int* out_i)
{
    const __m256 vt = _mm256_set1_ps(t);
    const __m256i iota = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
    int n = 0, c = 0;
    for (; c + 8 <= nc; c += 8)
    {
        const __m256 x = _mm256_loadu_ps(v + c);
        const int m = _mm256_movemask_ps(_mm256_cmp_ps(x, vt, _CMP_LE_OQ));
        const __m256i perm = _mm256_load_si256((const __m256i*)lut8().p[m]);
        _mm256_storeu_ps(out_v + n, _mm256_permutevar8x32_ps(x, perm));
        _mm256_storeu_si256((__m256i*)(out_i + n),
                            _mm256_permutevar8x32_epi32(_mm256_add_epi32(iota, _mm256_set1_epi32(c)), perm));
        n += popcount((unsigned)m);
    }
    return n + compact_scalar(v, c, nc, t, out_v + n, out_i + n);
}

inline __m256i
widen8(const int16_t* p)
{
    return _mm256_cvtepi16_epi32(_mm_loadu_si128((const __m128i*)p));
}

inline __m256i
widen8(const int8_t* p)
{
    return _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*)p));
}

inline void
narrow8(int16_t* out, const __m256i x)
{
    _mm_storeu_si128((__m128i*)out, _mm_packs_epi32(_mm256_castsi256_si128(x), _mm256_extracti128_si256(x, 1)));
}

inline void
narrow8(int8_t* out, const __m256i x)
{
    const __m128i w = _mm_packs_epi32(_mm256_castsi256_si128(x), _mm256_extracti128_si256(x, 1));
    _mm_storel_epi64((__m128i*)out, _mm_packs_epi16(w, w));
}

template<typename T>
inline int
compact_fixed(const T* v, const int nc, const T t, T* out_v, int* out_i)
{
    const __m256i vt = _mm256_set1_epi32((int32_t)t);
    const __m256i iota = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
    int n = 0, c = 0;
    for (; c + 8 <= nc; c += 8)
    {
        const __m256i x = widen8(v + c);
        const int m = ~_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpgt_epi32(x, vt))) & 0xFF;
        const __m256i perm = _mm256_load_si256((const __m256i*)lut8().p[m]);
        narrow8(out_v + n, _mm256_permutevar8x32_epi32(x, perm));
        _mm256_storeu_si256((__m256i*)(out_i + n),
                            _mm256_permutevar8x32_epi32(_mm256_add_epi32(iota, _mm256_set1_epi32(c)), perm));
        n += popcount((unsigned)m);
    }
    return n + compact_scalar(v, c, nc, t, out_v + n, out_i + n);
}

inline int
compact(const int16_t* v, const int nc, const int16_t t, int16_t* out_v, int* out_i)
{
    return compact_fixed(v, nc, t, out_v, out_i);
}

inline int
compact(const int8_t* v, const int nc, const int8_t t, int8_t* out_v, int* out_i)
{
    return compact_fixed(v, nc, t, out_v, out_i);
}
#endif
} // namespace prune_detail

template<>
struct prune_compact<float> : prune_compact_base<float>
{
    static inline int compact(const float* v, const int nc, const float t, float* out_v, int* out_i)
    {
        return prune_detail::compact(v, nc, t, out_v, out_i);
    }

    static constexpr bool is_vectorized() { return true; }
};

template<>
struct prune_compact<int16_t> : prune_compact_base<int16_t>
{
    static inline int compact(const int16_t* v, const int nc, const int16_t t, int16_t* out_v, int* out_i)
    {
        return prune_detail::compact(v, nc, t, out_v, out_i);
    }

    static constexpr bool is_vectorized() { return true; }
};

template<>
struct prune_compact<int8_t> : prune_compact_base<int8_t>
{
    static inline int compact(const int8_t* v, const int nc, const int8_t t, int8_t* out_v, int* out_i)
    {
        return prune_detail::compact(v, nc, t, out_v, out_i);
    }

    static constexpr bool is_vectorized() { return true; }
};
#endif

} // namespace tools
} // namespace aff3ct

#endif /* PRUNE_COMPACT_HPP */
