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

    // 1. Pack U_K directly into pack_buffer (fused convert + pack)
    this->pack_systematic(U_K, this->pack_buffer.data(), this->N);

    // 2. First transform
    this->transform_packed(this->pack_buffer.data(), this->N);

    // 3. Mask frozen bits in packed domain
    const size_t n_words = this->N >> 6;
    for (size_t i = 0; i < n_words; ++i)
        this->pack_buffer[i] &= this->packed_frozen_bits[i];

    // 4. Second transform
    this->transform_packed(this->pack_buffer.data(), this->N);

    // 5. Unpack result to X_N
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
