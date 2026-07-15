/*!
 * \file
 * \brief AVX2 Bitonic Merge Sorter for L=32 SCL decoding.
 *
 * This implementation provides an AVX2-optimized Bitonic merge network
 * for selecting the best 32 path metrics from 64/128/256 candidates.
 * Designed specifically for Polar SCL decoder with L=32.
 */
#ifndef BITONIC_SORTER_AVX2_HPP
#define BITONIC_SORTER_AVX2_HPP

#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

#ifdef __AVX2__
#include <immintrin.h>
#endif

namespace aff3ct
{
namespace tools
{

#ifdef __AVX2__

// ============================================================================
// AVX2 Bitonic Sort primitives for float32
// ============================================================================

// Compare-and-exchange: put min in a, max in b (ascending order)
inline void
bitonic_cas_ascending(__m256& a, __m256& b)
{
    __m256 min_val = _mm256_min_ps(a, b);
    __m256 max_val = _mm256_max_ps(a, b);
    a = min_val;
    b = max_val;
}

// Compare-and-exchange: put max in a, min in b (descending order)
inline void
bitonic_cas_descending(__m256& a, __m256& b)
{
    __m256 min_val = _mm256_min_ps(a, b);
    __m256 max_val = _mm256_max_ps(a, b);
    a = max_val;
    b = min_val;
}

// Reverse the order of 8 floats in a 256-bit register
inline __m256
reverse_ps(__m256 v)
{
    // Permute within 128-bit lanes: 3,2,1,0 -> indices 0x1B = 27
    v = _mm256_permute_ps(v, 0x1B);
    // Swap the two 128-bit lanes
    v = _mm256_permute2f128_ps(v, v, 0x01);
    return v;
}

// ============================================================================
// Bitonic merge for 8 elements (1 AVX register) - full sort
// ============================================================================
inline void
bitonic_sort_8(__m256& data)
{
    // Stage 1: pairs
    __m256 shuffled = _mm256_shuffle_ps(data, data, 0xB1); // swap adjacent pairs
    __m256 min_val = _mm256_min_ps(data, shuffled);
    __m256 max_val = _mm256_max_ps(data, shuffled);
    data = _mm256_blend_ps(min_val, max_val, 0xAA); // 10101010

    // Stage 2: quads
    shuffled = _mm256_shuffle_ps(data, data, 0x4E); // swap adjacent quads within 128-bit
    min_val = _mm256_min_ps(data, shuffled);
    max_val = _mm256_max_ps(data, shuffled);
    data = _mm256_blend_ps(min_val, max_val, 0xCC); // 11001100

    shuffled = _mm256_shuffle_ps(data, data, 0xB1);
    min_val = _mm256_min_ps(data, shuffled);
    max_val = _mm256_max_ps(data, shuffled);
    data = _mm256_blend_ps(min_val, max_val, 0xAA);

    // Stage 3: octets (across 128-bit lanes)
    shuffled = _mm256_permute2f128_ps(data, data, 0x01); // swap 128-bit lanes
    min_val = _mm256_min_ps(data, shuffled);
    max_val = _mm256_max_ps(data, shuffled);
    data = _mm256_blend_ps(min_val, max_val, 0xF0); // 11110000

    shuffled = _mm256_shuffle_ps(data, data, 0x4E);
    min_val = _mm256_min_ps(data, shuffled);
    max_val = _mm256_max_ps(data, shuffled);
    data = _mm256_blend_ps(min_val, max_val, 0xCC);

    shuffled = _mm256_shuffle_ps(data, data, 0xB1);
    min_val = _mm256_min_ps(data, shuffled);
    max_val = _mm256_max_ps(data, shuffled);
    data = _mm256_blend_ps(min_val, max_val, 0xAA);
}

// ============================================================================
// Bitonic merge step for two AVX registers (merge 16 -> 16 sorted)
// ============================================================================
inline void
bitonic_merge_16(__m256& lo, __m256& hi)
{
    // Step 1: Compare lo with reversed hi
    __m256 hi_rev = reverse_ps(hi);
    bitonic_cas_ascending(lo, hi_rev);
    hi = reverse_ps(hi_rev);

    // Step 2: Recursively merge within each register
    // lo merge
    __m256 lo_hi_half = _mm256_permute2f128_ps(lo, lo, 0x01);
    bitonic_cas_ascending(lo, lo_hi_half);
    lo = _mm256_blend_ps(lo, lo_hi_half, 0xF0);

    __m256 lo_shuffled = _mm256_shuffle_ps(lo, lo, 0x4E);
    __m256 lo_min = _mm256_min_ps(lo, lo_shuffled);
    __m256 lo_max = _mm256_max_ps(lo, lo_shuffled);
    lo = _mm256_blend_ps(lo_min, lo_max, 0xCC);

    lo_shuffled = _mm256_shuffle_ps(lo, lo, 0xB1);
    lo_min = _mm256_min_ps(lo, lo_shuffled);
    lo_max = _mm256_max_ps(lo, lo_shuffled);
    lo = _mm256_blend_ps(lo_min, lo_max, 0xAA);

    // hi merge
    __m256 hi_hi_half = _mm256_permute2f128_ps(hi, hi, 0x01);
    bitonic_cas_ascending(hi, hi_hi_half);
    hi = _mm256_blend_ps(hi, hi_hi_half, 0xF0);

    __m256 hi_shuffled = _mm256_shuffle_ps(hi, hi, 0x4E);
    __m256 hi_min = _mm256_min_ps(hi, hi_shuffled);
    __m256 hi_max = _mm256_max_ps(hi, hi_shuffled);
    hi = _mm256_blend_ps(hi_min, hi_max, 0xCC);

    hi_shuffled = _mm256_shuffle_ps(hi, hi, 0xB1);
    hi_min = _mm256_min_ps(hi, hi_shuffled);
    hi_max = _mm256_max_ps(hi, hi_shuffled);
    hi = _mm256_blend_ps(hi_min, hi_max, 0xAA);
}

// ============================================================================
// Class for L=32 AVX2 Bitonic sorting with SIMD key-value pairs
// ============================================================================

class Bitonic_sorter_avx2_L32
{
  private:
    // Key-value pair: pack float value and int32 index into 64 bits
    // Using uint64_t where:
    //   - lower 32 bits: index (as int32)
    //   - upper 32 bits: value (as float bit-pattern, reinterpreted)
    // This allows us to sort pairs using 64-bit integer comparisons
    // (works correctly for positive floats)

    uint64_t kv_pairs[256];

    // Pack a value-index pair into 64-bit representation
    static inline uint64_t pack_kv(float value, int32_t index)
    {
        uint32_t value_bits;
        std::memcpy(&value_bits, &value, sizeof(float));
        return (static_cast<uint64_t>(value_bits) << 32) | static_cast<uint32_t>(index);
    }

    // Extract index from packed pair
    static inline int32_t unpack_index(uint64_t kv) { return static_cast<int32_t>(kv & 0xFFFFFFFF); }

    // Extract value from packed pair
    static inline float unpack_value(uint64_t kv)
    {
        uint32_t value_bits = static_cast<uint32_t>(kv >> 32);
        float value;
        std::memcpy(&value, &value_bits, sizeof(float));
        return value;
    }

  public:
    Bitonic_sorter_avx2_L32() = default;

    /*!
     * \brief Partial sort to find the K smallest elements and their indices.
     * \param values Input array of metric values.
     * \param pos Output vector of indices of the K smallest values.
     * \param n_elmts Number of elements to consider.
     * \param K Number of smallest elements to find (must be 32 for this implementation).
     */
    inline void partial_sort(const float* values, std::vector<int>& pos, int n_elmts, int K)
    {
        // This implementation is specifically optimized for K=32 (L=32)
        // For other K values, fall back to a simpler approach
        if (K != 32 || n_elmts < 32)
        {
            // Fallback: use std::partial_sort with index tracking
            std::vector<std::pair<float, int>> indexed(n_elmts);
            for (int i = 0; i < n_elmts; i++)
                indexed[i] = { values[i], i };

            std::partial_sort(indexed.begin(),
                              indexed.begin() + K,
                              indexed.end(),
                              [](const std::pair<float, int>& a, const std::pair<float, int>& b)
                              { return a.first < b.first; });

            for (int i = 0; i < K; i++)
                pos[i] = indexed[i].second;
            return;
        }

        // Pack values with indices
        for (int i = 0; i < n_elmts; i++)
            kv_pairs[i] = pack_kv(values[i], i);

        // For n_elmts == 64: REP node case (2L candidates)
        if (n_elmts == 64)
        {
            bitonic_sort_kv_64();
        }
        // For n_elmts == 128: R1 node case (4L candidates)
        else if (n_elmts == 128)
        {
            bitonic_sort_kv_128();
        }
        // For n_elmts == 256: SPC node case (8L candidates)
        else if (n_elmts == 256)
        {
            bitonic_sort_kv_256();
        }
        else
        {
            // Fallback for other sizes: simple sort
            std::sort(kv_pairs, kv_pairs + n_elmts, [](uint64_t a, uint64_t b) { return (a >> 32) < (b >> 32); });
        }

        // Extract top K indices
        for (int i = 0; i < K; i++)
            pos[i] = unpack_index(kv_pairs[i]);
    }

  private:
    // ========================================================================
    // SIMD compare-and-swap for 4 key-value pairs at once
    // Uses AVX2 to process 4 x 64-bit pairs = 256 bits
    // ========================================================================

    // Compare-and-swap ascending: swap if kv[i] > kv[j]
    // Note: We compare the upper 32 bits (float as bits)
    inline void cas_asc_simd_4(int i)
    {
        // Load 4 pairs starting at i and i+4
        __m256i a = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&kv_pairs[i]));
        __m256i b = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&kv_pairs[i + 4]));

        // Extract the float values (upper 32 bits of each 64-bit element)
        // Shift right by 32 bits to get the value in the lower 32 bits
        __m256i a_vals = _mm256_srli_epi64(a, 32);
        __m256i b_vals = _mm256_srli_epi64(b, 32);

        // Compare: mask is -1 where a > b (for unsigned 32-bit comparison)
        // We treat the float bit patterns as unsigned integers
        // This works correctly for positive floats (which metrics are)
        __m256i cmp = _mm256_cmpgt_epi32(a_vals, b_vals);

        // Extend 32-bit comparison result to 64-bit masks
        // Need to duplicate each 32-bit result to fill 64 bits
        cmp = _mm256_shuffle_epi32(cmp, 0xA0); // 10 10 00 00 pattern

        // Blend: select b where a > b (make ascending)
        __m256i min_val = _mm256_blendv_epi8(a, b, cmp);
        __m256i max_val = _mm256_blendv_epi8(b, a, cmp);

        // Store back
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(&kv_pairs[i]), min_val);
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(&kv_pairs[i + 4]), max_val);
    }

    // Scalar compare-and-swap for non-vectorizable cases
    inline void cas_asc(int i, int j)
    {
        if ((kv_pairs[i] >> 32) > (kv_pairs[j] >> 32)) std::swap(kv_pairs[i], kv_pairs[j]);
    }

    inline void cas_desc(int i, int j)
    {
        if ((kv_pairs[i] >> 32) < (kv_pairs[j] >> 32)) std::swap(kv_pairs[i], kv_pairs[j]);
    }

    // ========================================================================
    // Tree-based Merge-Pruning Optimization
    // ========================================================================

    // AVX2 SIMD compare-and-swap for a contiguous range of elements
    // Compares pairs[off_a + k] with pairs[off_b + k] for k in [0, count-1]
    // Stores min in off_a, max in off_b (Ascending logic)
    // Count must be multiple of 4
    inline void cas_asc_simd_range(int off_a, int off_b, int count)
    {
        for (int k = 0; k < count; k += 4)
        {
            __m256i a = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&kv_pairs[off_a + k]));
            __m256i b = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&kv_pairs[off_b + k]));

            // Compare signed 64-bit integers (a > b) -> mask -1
            __m256i cmp = _mm256_cmpgt_epi64(a, b);

            // Blend: if cmp is true (a > b), swap
            __m256i min_v = _mm256_blendv_epi8(a, b, cmp); // selects b if cmp
            __m256i max_v = _mm256_blendv_epi8(b, a, cmp); // selects a if cmp

            _mm256_storeu_si256(reinterpret_cast<__m256i*>(&kv_pairs[off_a + k]), min_v);
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(&kv_pairs[off_b + k]), max_v);
        }
    }

    // AVX2 SIMD compare-and-swap for a contiguous range (Descending)
    // Compares pairs[off_a + k] with pairs[off_b + k]
    // Stores MAX in off_a, MIN in off_b (Descending logic)
    inline void cas_desc_simd_range(int off_a, int off_b, int count)
    {
        for (int k = 0; k < count; k += 4)
        {
            __m256i a = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&kv_pairs[off_a + k]));
            __m256i b = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&kv_pairs[off_b + k]));

            // Compare a > b
            __m256i cmp = _mm256_cmpgt_epi64(a, b);

            // Blend
            __m256i min_v = _mm256_blendv_epi8(a, b, cmp);
            __m256i max_v = _mm256_blendv_epi8(b, a, cmp);

            // Store MAX in off_a, MIN in off_b for Descending
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(&kv_pairs[off_a + k]), max_v);
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(&kv_pairs[off_b + k]), min_v);
        }
    }

    // Sort 32 elements starting at offset
    inline void bitonic_sort_32(int offset, bool ascending)
    {
        // Stage 1: Sort pairs
        for (int i = offset; i < offset + 32; i += 2)
            if (ascending == ((i / 2) % 2 == 0))
                cas_asc(i, i + 1);
            else
                cas_desc(i, i + 1);

        // Stages 2-5: Merge passes
        for (int size = 4; size <= 32; size *= 2)
        {
            for (int i = offset; i < offset + 32; i += size)
            {
                bool asc = (ascending == ((i / size) % 2 == 0));
                if (size == 32) asc = ascending;

                // Merge pass
                for (int dist = size / 2; dist >= 1; dist /= 2)
                {
                    // Vectorize for dist >= 4 (contiguous range >= 4)
                    if (dist >= 4)
                    {
                        for (int k = 0; k < size; k += 2 * dist)
                        {
                            if (asc)
                                cas_asc_simd_range(i + k, i + k + dist, dist);
                            else
                                cas_desc_simd_range(i + k, i + k + dist, dist);
                        }
                    }
                    else
                    {
                        // Scalar for small strides
                        for (int k = 0; k < size; k += 2 * dist)
                        {
                            for (int j = 0; j < dist; j++)
                            {
                                if (asc)
                                    cas_asc(i + k + j, i + k + j + dist);
                                else
                                    cas_desc(i + k + j, i + k + j + dist);
                            }
                        }
                    }
                }
            }
        }
    }

    // Merge two 32-element blocks (A sorted Asc, B sorted Desc)
    // Keep best 32 in A, sorted in requested output direction
    // Assumes [off_a..off_a+31] is Asc, [off_b..off_b+31] is Desc
    inline void bitonic_merge_prune_64(int off_a, int off_b, bool out_ascending)
    {
        // Step 1: Compare A[i] with B[i] (Bitonic merge of 64 elements, first stage)
        // This is a contiguous comparison of 32 elements! Perfect for SIMD.
        cas_asc_simd_range(off_a, off_b, 32);

        // After this, off_a contains the min 32 elements.
        // Finish sorting off_a.

        int offset = off_a;
        for (int dist = 16; dist >= 1; dist /= 2)
        {
            // Vectorize if dist >= 4
            if (dist >= 4)
            {
                for (int k = 0; k < 32; k += 2 * dist)
                {
                    if (out_ascending)
                        cas_asc_simd_range(offset + k, offset + k + dist, dist);
                    else
                        cas_desc_simd_range(offset + k, offset + k + dist, dist);
                }
            }
            else
            {
                for (int k = 0; k < 32; k += 2 * dist)
                {
                    for (int j = 0; j < dist; j++)
                    {
                        if (out_ascending)
                            cas_asc(offset + k + j, offset + k + j + dist);
                        else
                            cas_desc(offset + k + j, offset + k + j + dist);
                    }
                }
            }
        }
    }

    // Full Bitonic sort for 64 elements (Optimized for top 32)
    inline void bitonic_sort_kv_64()
    {
        // Block 0: Asc, Block 1: Desc
        bitonic_sort_32(0, true);
        bitonic_sort_32(32, false);

        // Merge-Prune to keep best 32 in 0-31 (Asc)
        bitonic_merge_prune_64(0, 32, true);

        // Note: The rest (32-63) is garbage/ignored, but indices are still valid in 0-31
    }

    // Optimized for 128 elements -> top 32
    inline void bitonic_sort_kv_128()
    {
        // Level 1: Sort 4 blocks
        bitonic_sort_32(0, true);
        bitonic_sort_32(32, false); // Pair 0-1

        bitonic_sort_32(64, true);
        bitonic_sort_32(96, false); // Pair 2-3

        // Level 2: Merge-Prune pairs
        bitonic_merge_prune_64(0, 32, true);   // Best of 0-63 now in 0-31 (Asc)
        bitonic_merge_prune_64(64, 96, false); // Best of 64-127 now in 64-95 (Desc)

        // Level 3: Final Merge-Prune
        bitonic_merge_prune_64(0, 64, true); // Best of all now in 0-31 (Asc)
    }

    // Optimized for 256 elements -> top 32
    inline void bitonic_sort_kv_256()
    {
        // Level 1: Sort 8 blocks
        bitonic_sort_32(0, true);
        bitonic_sort_32(32, false);
        bitonic_sort_32(64, true);
        bitonic_sort_32(96, false);
        bitonic_sort_32(128, true);
        bitonic_sort_32(160, false);
        bitonic_sort_32(192, true);
        bitonic_sort_32(224, false);

        // Level 2: Merge-Prune (8->4 blocks)
        bitonic_merge_prune_64(0, 32, true);     // 0-31 Asc
        bitonic_merge_prune_64(64, 96, false);   // 64-95 Desc
        bitonic_merge_prune_64(128, 160, true);  // 128-159 Asc
        bitonic_merge_prune_64(192, 224, false); // 192-223 Desc

        // Level 3: Merge-Prune (4->2 blocks)
        bitonic_merge_prune_64(0, 64, true);     // 0-31 Asc
        bitonic_merge_prune_64(128, 192, false); // 128-159 Desc

        // Level 4: Final Merge-Prune
        bitonic_merge_prune_64(0, 128, true); // 0-31 Asc
    }
};

#else // !__AVX2__

// Fallback implementation when AVX2 is not available
class Bitonic_sorter_avx2_L32
{
  public:
    Bitonic_sorter_avx2_L32() = default;

    inline void partial_sort(const float* values, std::vector<int>& pos, int n_elmts, int K)
    {
        std::vector<std::pair<float, int>> indexed(n_elmts);
        for (int i = 0; i < n_elmts; i++)
            indexed[i] = { values[i], i };

        std::partial_sort(indexed.begin(),
                          indexed.begin() + K,
                          indexed.end(),
                          [](const std::pair<float, int>& a, const std::pair<float, int>& b)
                          { return a.first < b.first; });

        for (int i = 0; i < K; i++)
            pos[i] = indexed[i].second;
    }
};

#endif // __AVX2__

} // namespace tools
} // namespace aff3ct

#endif /* BITONIC_SORTER_AVX2_HPP */
