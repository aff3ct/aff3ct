#include <string>

#include "Module/Encoder/Polar/Encoder_polar_bitpacked_sys.hpp"

using namespace aff3ct::module;

template<typename B>
Encoder_polar_bitpacked_sys<B>::Encoder_polar_bitpacked_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits)
  : Encoder_polar_bitpacked<B>(K, N, frozen_bits)
{
    const std::string name = "Encoder_polar_bitpacked_sys";
    this->set_name(name);
    this->set_sys(true);
}

template<typename B>
Encoder_polar_bitpacked_sys<B>*
Encoder_polar_bitpacked_sys<B>::clone() const
{
    auto m = new Encoder_polar_bitpacked_sys(*this);
    m->deep_copy(*this);
    return m;
}

template<typename B>
void
Encoder_polar_bitpacked_sys<B>::_encode(const B* U_K, B* X_N, const size_t /*frame_id*/)
{
    if (this->N < 64)
    {
        this->convert(U_K, X_N);
        Encoder_polar<B>::light_encode(X_N);
        for (auto i = 0; i < this->N; i++)
            X_N[i] = (B)(!this->frozen_bits[i]) && X_N[i];
        Encoder_polar<B>::light_encode(X_N);
        return;
    }

    // 1. Tree-pattern conversion + 1st pass transform (with Rate-0 pruning & REP broadcast)
    this->encode_tree_bitpacked(U_K, this->pack_buffer.data());

    // 2. Mask frozen bits (packed_frozen_bits has 1s at info positions and 0s at frozen positions)
    const size_t n_words = this->N >> 6;
    for (size_t i = 0; i < n_words; ++i)
        this->pack_buffer[i] &= this->packed_frozen_bits[i];

    // 3. Second pass butterfly transform
    this->transform_packed(this->pack_buffer.data(), this->N);

    // 4. SIMD unpack result to X_N
    this->unpack(this->pack_buffer.data(), X_N, this->N);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_polar_bitpacked_sys<B_8>;
template class aff3ct::module::Encoder_polar_bitpacked_sys<B_16>;
template class aff3ct::module::Encoder_polar_bitpacked_sys<B_32>;
template class aff3ct::module::Encoder_polar_bitpacked_sys<B_64>;
#else
template class aff3ct::module::Encoder_polar_bitpacked_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
