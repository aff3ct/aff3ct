#include <algorithm>
#include <cmath>
#include <sstream>
#include <streampu.hpp>
#include <string>

#include "Module/Encoder/Polar/Encoder_polar_bitpacked.hpp"
#include "Tools/Code/Polar/fb_assert.h"

using namespace aff3ct::module;

template<typename B>
alignas(64) uint8_t Encoder_polar_bitpacked<B>::bit_expand_lut[256][8];

template<typename B>
bool Encoder_polar_bitpacked<B>::lut_initialized = false;

template<typename B>
Encoder_polar_bitpacked<B>::Encoder_polar_bitpacked(const int& K, const int& N, const std::vector<bool>& frozen_bits)
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
}

template<typename B>
void
Encoder_polar_bitpacked<B>::pack(const B* bits_in, uint64_t* pack_out, const size_t N)
{
    const size_t n_words = N >> 6;
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
            reg.storeu(ptr);
        }

        for (size_t d_words = 1; d_words < n_words; d_words <<= 1)
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
    this->convert(U_K, X_N);
    this->light_encode(X_N);
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
