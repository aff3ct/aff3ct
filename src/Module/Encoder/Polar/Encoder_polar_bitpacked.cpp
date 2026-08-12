#include <algorithm>
#include <cmath>
#include <sstream>
#include <streampu.hpp>
#include <string>

#include "Module/Encoder/Polar/Encoder_polar_bitpacked.hpp"
#include "Tools/Code/Polar/fb_assert.h"
#include "Tools/Code/Polar/Pattern_polar_parser.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_spc.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_std.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template<typename B>
alignas(64) uint8_t Encoder_polar_bitpacked<B>::bit_expand_lut[256][8];

template<typename B>
bool Encoder_polar_bitpacked<B>::lut_initialized = false;

template<typename B>
Encoder_polar_bitpacked<B>::Encoder_polar_bitpacked(const int& K,
                                                     const int& N,
                                                     const std::vector<bool>& frozen_bits)
  : Encoder_polar<B>(K, N, frozen_bits)
  , packed_frozen_bits(this->N >> 6, 0)
  , pack_buffer(this->N >> 6, 0)
{
    const std::string name = "Encoder_polar_bitpacked";
    this->set_name(name);

    if (!lut_initialized)
    {
        init_lut();
        lut_initialized = true;
    }

    this->set_frozen_bits(frozen_bits);
}

template<typename B>
Encoder_polar_bitpacked<B>*
Encoder_polar_bitpacked<B>::clone() const
{
    auto m = new Encoder_polar_bitpacked(*this);
    m->deep_copy(*this);
    return m;
}

template<typename B>
void
Encoder_polar_bitpacked<B>::init_lut()
{
    for (int i = 0; i < 256; ++i)
        for (int j = 0; j < 8; ++j)
            bit_expand_lut[i][j] = (i >> (7 - j)) & 1;
}

template<typename B>
void
Encoder_polar_bitpacked<B>::set_frozen_bits(const std::vector<bool>& frozen_bits)
{
    Encoder_polar<B>::set_frozen_bits(frozen_bits);

    this->packed_frozen_bits.assign(this->N >> 6, 0);

    std::vector<B> notfb(this->N, 0);
    for (unsigned i = 0; i < static_cast<unsigned>(this->N); ++i)
        notfb[i] = !this->frozen_bits[i];

    pack(notfb.data(), this->packed_frozen_bits.data(), this->N);

    build_tree_execution_plan();
}

template<typename B>
void
Encoder_polar_bitpacked<B>::build_tree_execution_plan()
{
    this->execution_plan.clear();

    std::vector<aff3ct::tools::Pattern_polar_i*> patterns = {
        new aff3ct::tools::Pattern_polar_r0(),
        new aff3ct::tools::Pattern_polar_r1(),
        new aff3ct::tools::Pattern_polar_rep(),
        new aff3ct::tools::Pattern_polar_spc(),
        new aff3ct::tools::Pattern_polar_std()
    };

    aff3ct::tools::Pattern_polar_parser parser(this->frozen_bits, patterns, 0, 1, true);
    const auto& leaves = parser.get_leaves_pattern_types();

    int bit_offset = 0;
    int u_offset = 0;

    for (const auto& leaf : leaves)
    {
        polar_node_t p_type = static_cast<polar_node_t>(leaf.first);
        int p_size = leaf.second;

        Tree_node_info info;
        info.type = static_cast<int>(p_type);
        info.size = p_size;
        info.off_bit = bit_offset;
        info.off_u = u_offset;

        this->execution_plan.push_back(info);

        bit_offset += p_size;

        if (p_type == polar_node_t::RATE_0 || p_type == polar_node_t::RATE_0_LEFT)
        {
            // 0 info bits
        }
        else if (p_type == polar_node_t::RATE_1)
        {
            u_offset += p_size;
        }
        else if (p_type == polar_node_t::REP || p_type == polar_node_t::REP_LEFT)
        {
            u_offset += 1;
        }
        else if (p_type == polar_node_t::SPC)
        {
            u_offset += (p_size - 1);
        }
        else // STANDARD
        {
            for (int i = 0; i < p_size; ++i)
                if (!this->frozen_bits[info.off_bit + i])
                    u_offset++;
        }
    }
}

template<typename B>
void
Encoder_polar_bitpacked<B>::encode_tree_bitpacked(const B* U_K, uint64_t* pack_data)
{
    const size_t n_words = this->N >> 6;
    std::fill(pack_data, pack_data + n_words, 0ULL);

    int u_idx = 0;

    for (const auto& node : this->execution_plan)
    {
        polar_node_t p_type = static_cast<polar_node_t>(node.type);

        if (p_type == polar_node_t::RATE_0 || p_type == polar_node_t::RATE_0_LEFT) // R0 Node: all frozen (0s). Complete skip!
        {
            continue;
        }
        else if (p_type == polar_node_t::REP || p_type == polar_node_t::REP_LEFT) // REP Node: last bit is info bit
        {
            uint64_t u_last = static_cast<uint64_t>(U_K[u_idx++]) & 1u;
            int pos = node.off_bit + node.size - 1;
            size_t w = pos >> 6;
            size_t bit_pos = 63 - (pos & 63);
            pack_data[w] |= (u_last << bit_pos);
        }
        else if (p_type == polar_node_t::RATE_1) // R1 Node: all bits are information bits
        {
            for (int i = 0; i < node.size; ++i)
            {
                int pos = node.off_bit + i;
                uint64_t b = static_cast<uint64_t>(U_K[u_idx++]) & 1u;
                size_t w = pos >> 6;
                size_t bit_pos = 63 - (pos & 63);
                pack_data[w] |= (b << bit_pos);
            }
        }
        else if (p_type == polar_node_t::SPC) // SPC Node: first bit is frozen, remaining are info bits
        {
            for (int i = 0; i < node.size; ++i)
            {
                int pos = node.off_bit + i;
                if (i > 0)
                {
                    uint64_t b = static_cast<uint64_t>(U_K[u_idx++]) & 1u;
                    size_t w = pos >> 6;
                    size_t bit_pos = 63 - (pos & 63);
                    pack_data[w] |= (b << bit_pos);
                }
            }
        }
        else // STANDARD Node: mixed frozen/info node
        {
            for (int i = 0; i < node.size; ++i)
            {
                int pos = node.off_bit + i;
                if (!this->frozen_bits[pos])
                {
                    uint64_t b = static_cast<uint64_t>(U_K[u_idx++]) & 1u;
                    size_t w = pos >> 6;
                    size_t bit_pos = 63 - (pos & 63);
                    pack_data[w] |= (b << bit_pos);
                }
            }
        }
    }

    // Single SIMD Bitpacked Butterfly Transformation pass over the packed sequence
    this->transform_packed(pack_data, this->N);
}

template<typename B>
void
Encoder_polar_bitpacked<B>::pack(const B* bits_in, uint64_t* pack_out, const size_t N)
{
    const size_t n_words = N >> 6;

#if defined(__AVX2__)
    if (sizeof(B) == 4)
    {
        static const uint8_t rev8[256] = {
            0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
            0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
            0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
            0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
            0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
            0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
            0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
            0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
            0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
            0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
            0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
            0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
            0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
            0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
            0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
            0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
        };

        const __m256i zero = _mm256_setzero_si256();
        for (size_t w = 0; w < n_words; ++w)
        {
            const auto* in = reinterpret_cast<const __m256i*>(bits_in + (w << 6));
            uint64_t symb = 0;
            for (int i = 0; i < 8; ++i)
            {
                __m256i v = _mm256_loadu_si256(in + i);
                __m256i cmp = _mm256_cmpeq_epi32(v, zero);
                uint8_t raw = static_cast<uint8_t>(~_mm256_movemask_ps(_mm256_castsi256_ps(cmp)));
                symb = (symb << 8) | rev8[raw];
            }
            pack_out[w] = symb;
        }
        return;
    }
#endif

    for (size_t w = 0; w < n_words; ++w)
    {
        uint64_t symb = 0;
        const B* in = bits_in + (w << 6);
        for (size_t j = 0; j < 64; ++j)
        {
            symb <<= 1;
            symb |= (static_cast<uint64_t>(in[j]) & 1u);
        }
        pack_out[w] = symb;
    }
}

template<typename B>
void
Encoder_polar_bitpacked<B>::pack_systematic(const B* U_K, uint64_t* pack_out, const size_t N) const
{
    const size_t n_words = N >> 6;
    size_t k_idx = 0;
    for (size_t w = 0; w < n_words; ++w)
    {
        uint64_t symb = 0;
        const size_t base_idx = w << 6;
        for (size_t j = 0; j < 64; ++j)
        {
            symb <<= 1;
            if (!this->frozen_bits[base_idx + j])
            {
                symb |= (static_cast<uint64_t>(U_K[k_idx++]) & 1u);
            }
        }
        pack_out[w] = symb;
    }
}

template<typename B>
void
Encoder_polar_bitpacked<B>::unpack(const uint64_t* pack_in, B* bits_out, const size_t N)
{
    const size_t n_words = N >> 6;

#if defined(__AVX2__)
    if (sizeof(B) == 4)
    {
        const __m256i bit_masks = _mm256_setr_epi32(0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01);
        const __m256i zero = _mm256_setzero_si256();
        const __m256i one = _mm256_set1_epi32(1);

        for (size_t w = 0; w < n_words; ++w)
        {
            uint64_t s = pack_in[w];
            auto* out_ptr = reinterpret_cast<__m256i*>(bits_out + (w << 6));
            for (size_t b = 0; b < 8; ++b)
            {
                uint8_t byte = static_cast<uint8_t>((s >> ((7 - b) * 8)) & 0xFF);
                __m256i vbit = _mm256_set1_epi32(byte);
                __m256i test = _mm256_and_si256(vbit, bit_masks);
                __m256i cmp = _mm256_cmpeq_epi32(test, zero);
                __m256i res = _mm256_andnot_si256(cmp, one);
                _mm256_storeu_si256(out_ptr + b, res);
            }
        }
        return;
    }
#endif

    for (size_t w = 0; w < n_words; ++w)
    {
        uint64_t s = pack_in[w];
        B* out = bits_out + (w << 6);
        for (size_t b = 0; b < 8; ++b)
        {
            uint8_t byte = static_cast<uint8_t>((s >> ((7 - b) * 8)) & 0xFF);
            const uint8_t* lut = bit_expand_lut[byte];
            for (size_t j = 0; j < 8; ++j)
                out[(b << 3) + j] = static_cast<B>(lut[j]);
        }
    }
}

template<typename B>
void
Encoder_polar_bitpacked<B>::transform_packed(uint64_t* pack_data, const size_t N)
{
    const size_t n_words = N >> 6;
    if (n_words == 0) return;

    static const uint64_t masks[6] = {
        0xAAAAAAAAAAAAAAAAULL, // d = 1
        0xCCCCCCCCCCCCCCCCULL, // d = 2
        0xF0F0F0F0F0F0F0F0ULL, // d = 4
        0xFF00FF00FF00FF00ULL, // d = 8
        0xFFFF0000FFFF0000ULL, // d = 16
        0xFFFFFFFF00000000ULL  // d = 32
    };

    constexpr int W_reg = mipp::N<uint64_t>();
    const size_t n_simd = n_words / W_reg;

    if (n_simd > 0)
    {
        const mipp::Reg<uint64_t> m0 = masks[0];
        const mipp::Reg<uint64_t> m1 = masks[1];
        const mipp::Reg<uint64_t> m2 = masks[2];
        const mipp::Reg<uint64_t> m3 = masks[3];
        const mipp::Reg<uint64_t> m4 = masks[4];
        const mipp::Reg<uint64_t> m5 = masks[5];

        for (size_t r = 0; r < n_simd; ++r)
        {
            uint64_t* ptr = pack_data + r * W_reg;
            mipp::Reg<uint64_t> reg;
            reg.loadu(ptr);
            reg ^= ((reg << 1) & m0);
            reg ^= ((reg << 2) & m1);
            reg ^= ((reg << 4) & m2);
            reg ^= ((reg << 8) & m3);
            reg ^= ((reg << 16) & m4);
            reg ^= ((reg << 32) & m5);

#if defined(__AVX512F__)
            if (W_reg == 8)
            {
                __m512i vreg = *reinterpret_cast<__m512i*>(&reg.r);
                static const __m512i idx_d1 = _mm512_setr_epi64(1, 0, 3, 2, 5, 4, 7, 6);
                static const __m512i mask_d1 = _mm512_setr_epi64(-1LL, 0LL, -1LL, 0LL, -1LL, 0LL, -1LL, 0LL);
                __m512i perm_d1 = _mm512_permutexvar_epi64(idx_d1, vreg);
                vreg = _mm512_xor_si512(vreg, _mm512_and_si512(perm_d1, mask_d1));

                static const __m512i idx_d2 = _mm512_setr_epi64(2, 3, 0, 1, 6, 7, 4, 5);
                static const __m512i mask_d2 = _mm512_setr_epi64(-1LL, -1LL, 0LL, 0LL, -1LL, -1LL, 0LL, 0LL);
                __m512i perm_d2 = _mm512_permutexvar_epi64(idx_d2, vreg);
                vreg = _mm512_xor_si512(vreg, _mm512_and_si512(perm_d2, mask_d2));

                static const __m512i idx_d4 = _mm512_setr_epi64(4, 5, 6, 7, 0, 1, 2, 3);
                static const __m512i mask_d4 = _mm512_setr_epi64(-1LL, -1LL, -1LL, -1LL, 0LL, 0LL, 0LL, 0LL);
                __m512i perm_d4 = _mm512_permutexvar_epi64(idx_d4, vreg);
                vreg = _mm512_xor_si512(vreg, _mm512_and_si512(perm_d4, mask_d4));

                *reinterpret_cast<__m512i*>(&reg.r) = vreg;
            }
#elif defined(__AVX2__)
            if (W_reg == 4)
            {
                __m256i vreg = *reinterpret_cast<__m256i*>(&reg.r);
                __m256i perm_d1 = _mm256_permute4x64_epi64(vreg, _MM_SHUFFLE(2, 3, 0, 1));
                static const __m256i mask_d1 = _mm256_setr_epi64x(-1LL, 0LL, -1LL, 0LL);
                vreg = _mm256_xor_si256(vreg, _mm256_and_si256(perm_d1, mask_d1));

                __m256i perm_d2 = _mm256_permute4x64_epi64(vreg, _MM_SHUFFLE(1, 0, 3, 2));
                static const __m256i mask_d2 = _mm256_setr_epi64x(-1LL, -1LL, 0LL, 0LL);
                vreg = _mm256_xor_si256(vreg, _mm256_and_si256(perm_d2, mask_d2));

                *reinterpret_cast<__m256i*>(&reg.r) = vreg;
            }
#endif

            reg.storeu(ptr);
        }

        const size_t start_d_words = (W_reg >= 4) ? W_reg : 1;

        for (size_t d_words = start_d_words; d_words < n_words; d_words <<= 1)
        {
            if (d_words < static_cast<size_t>(W_reg))
            {
                const size_t block_size = d_words << 1;
                for (size_t b = 0; b < n_words; b += block_size)
                {
                    for (size_t i = 0; i < d_words; ++i)
                    {
                        pack_data[b + i] ^= pack_data[b + d_words + i];
                    }
                }
            }
            else
            {
                const size_t d_regs = d_words / W_reg;
                const size_t block_regs = d_regs << 1;
                for (size_t b = 0; b < n_simd; b += block_regs)
                {
                    for (size_t i = 0; i < d_regs; ++i)
                    {
                        uint64_t* ptr_top = pack_data + (b + i) * W_reg;
                        uint64_t* ptr_bot = pack_data + (b + d_regs + i) * W_reg;
                        mipp::Reg<uint64_t> reg_top, reg_bot;
                        reg_top.loadu(ptr_top);
                        reg_bot.loadu(ptr_bot);
                        reg_top ^= reg_bot;
                        reg_top.storeu(ptr_top);
                    }
                }
            }
        }
    }
    else
    {
        for (int s = 0; s < 6; ++s)
        {
            const int d = 1 << s;
            const uint64_t mask = masks[s];
            for (size_t w = 0; w < n_words; ++w)
            {
                pack_data[w] ^= ((pack_data[w] << d) & mask);
            }
        }
        for (size_t d_words = 1; d_words < n_words; d_words <<= 1)
        {
            const size_t block_size = d_words << 1;
            for (size_t b = 0; b < n_words; b += block_size)
            {
                for (size_t i = 0; i < d_words; ++i)
                {
                    pack_data[b + i] ^= pack_data[b + d_words + i];
                }
            }
        }
    }
}

template<typename B>
void
Encoder_polar_bitpacked<B>::light_encode(B* bits)
{
    if (this->N < 64)
    {
        Encoder_polar<B>::light_encode(bits);
        return;
    }

    pack(bits, this->pack_buffer.data(), this->N);
    transform_packed(this->pack_buffer.data(), this->N);
    unpack(this->pack_buffer.data(), bits, this->N);
}

template<typename B>
void
Encoder_polar_bitpacked<B>::_encode(const B* U_K, B* X_N, const size_t /*frame_id*/)
{
    if (this->N < 64)
    {
        Encoder_polar<B>::_encode(U_K, X_N, 0);
        return;
    }

    encode_tree_bitpacked(U_K, this->pack_buffer.data());
    unpack(this->pack_buffer.data(), X_N, this->N);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_polar_bitpacked<B_8>;
template class aff3ct::module::Encoder_polar_bitpacked<B_16>;
template class aff3ct::module::Encoder_polar_bitpacked<B_32>;
template class aff3ct::module::Encoder_polar_bitpacked<B_64>;
#else
template class aff3ct::module::Encoder_polar_bitpacked<B>;
#endif
// ==================================================================================== explicit template instantiation
