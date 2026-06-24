/*!
 * \file
 * \brief Flexible bitonic merge-prune sorting networks for SCL decoder.
 *
 * Provides top-K partial sort using bitonic sorting networks with merge-pruning.
 * Generated at compile time for any power of two L and candidate size N.
 */
#ifndef SORTING_NETWORK_HPP
#define SORTING_NETWORK_HPP

#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>
#include <vector>

namespace aff3ct
{
namespace tools
{

// ============================================================================
// Key-value packing helpers
// ============================================================================
template<typename T, bool IsFloat = std::is_floating_point<T>::value>
struct kv_packer;

template<typename T>
struct kv_packer<T, true>
{
    static inline uint64_t pack(T value, int32_t index)
    {
        uint32_t bits;
        std::memcpy(&bits, &value, sizeof(float));
        return (static_cast<uint64_t>(bits) << 32) | static_cast<uint32_t>(index);
    }
};

template<typename T>
struct kv_packer<T, false>
{
    static inline uint64_t pack(T value, int32_t index)
    {
        uint32_t biased = static_cast<uint32_t>(
            static_cast<int64_t>(value) - static_cast<int64_t>(std::numeric_limits<T>::min()));
        return (static_cast<uint64_t>(biased) << 32) | static_cast<uint32_t>(index);
    }
};

// ============================================================================
// Bitonic Sorting Network Metaprogramming (C++11 compatible)
// ============================================================================
namespace bitonic
{

static inline __attribute__((always_inline)) void cas(uint64_t& a, uint64_t& b)
{
    const uint64_t va = a, vb = b;
    const bool do_swap = (va > vb);
    a = do_swap ? vb : va;
    b = do_swap ? va : vb;
}

template<int N, int DIST, int DIR>
struct Bitonic_Merge_Step {
    static inline void apply(uint64_t* d) {
        for (int i = 0; i < N; i += 2 * DIST) {
            for (int j = 0; j < DIST; ++j) {
                if (DIR == 0) cas(d[i + j], d[i + j + DIST]);
                else          cas(d[i + j + DIST], d[i + j]);
            }
        }
        Bitonic_Merge_Step<N, DIST / 2, DIR>::apply(d);
    }
};

template<int N, int DIR>
struct Bitonic_Merge_Step<N, 0, DIR> {
    static inline void apply(uint64_t*) {}
};

template<int N, int DIR>
struct Bitonic_Merge {
    static inline void apply(uint64_t* d) {
        Bitonic_Merge_Step<N, N / 2, DIR>::apply(d);
    }
};

template<int N, int DIR>
struct Bitonic_Sort {
    static inline void apply(uint64_t* d) {
        Bitonic_Sort<N / 2, 0>::apply(d);
        Bitonic_Sort<N / 2, 1>::apply(d + N / 2);
        Bitonic_Merge<N, DIR>::apply(d);
    }
};

template<int DIR>
struct Bitonic_Sort<1, DIR> {
    static inline void apply(uint64_t*) {}
};

template<int L>
struct Merge_Prune {
    static inline void apply(uint64_t* a, uint64_t* b) {
        for (int i = 0; i < L; ++i) {
            cas(a[i], b[L - 1 - i]);
        }
        Bitonic_Merge<L, 0>::apply(a);
    }
};

template<>
struct Merge_Prune<1> {
    static inline void apply(uint64_t* a, uint64_t* b) {
        cas(a[0], b[0]);
    }
};

template<int N, int L>
struct Sort_Blocks {
    static inline void apply(uint64_t* d) {
        Bitonic_Sort<L, 0>::apply(d);
        Sort_Blocks<N - L, L>::apply(d + L);
    }
};

template<int L>
struct Sort_Blocks<0, L> {
    static inline void apply(uint64_t*) {}
};

template<int N_BLOCKS, int L>
struct Tree_Merge_Prune {
    static inline void apply(uint64_t* d) {
        for (int i = 0; i < N_BLOCKS / 2; ++i) {
            Merge_Prune<L>::apply(d + i * L, d + (i + N_BLOCKS / 2) * L);
        }
        Tree_Merge_Prune<N_BLOCKS / 2, L>::apply(d);
    }
};

template<int L>
struct Tree_Merge_Prune<1, L> {
    static inline void apply(uint64_t*) {}
};

template<int N, int L>
struct Partial_Sort {
    static inline void apply(uint64_t* d) {
        Sort_Blocks<N, L>::apply(d);
        Tree_Merge_Prune<N / L, L>::apply(d);
    }
};

} // namespace bitonic

// ============================================================================
// Generic Sorting Network for any L and N
// ============================================================================

template<typename T>
class Sorting_network
{
  private:
    // Max workspace size. 1024 is enough for L=128, n_cands=8 (1024)
    // Adjust if larger L is needed.
    uint64_t kv[1024]; 

    static inline int32_t unpack_index(uint64_t packed)
    {
        return static_cast<int32_t>(packed & 0xFFFFFFFF);
    }

  public:
    Sorting_network() {}

    /*!
     * \brief Select the K smallest elements from N.
     *        Uses bitonic partial sort generated at compile time.
     */
    template<int L, int N>
    inline void partial_sort_template(const T* values, std::vector<int>& pos, int K = -1)
    {
        if (K == -1) K = L;

        for (int i = 0; i < N; i++)
            kv[i] = kv_packer<T>::pack(values[i], i);

        bitonic::Partial_Sort<N, L>::apply(kv);

        for (int i = 0; i < K; i++)
            pos[i] = unpack_index(kv[i]);
    }
};

} // namespace tools
} // namespace aff3ct

#endif /* SORTING_NETWORK_HPP */
