/*!
 * \file
 * \brief Class module::Encoder_polar_bitpacked.
 */
#ifndef ENCODER_POLAR_BITPACKED_HPP_
#define ENCODER_POLAR_BITPACKED_HPP_

#include <cstdint>
#include <vector>
#include <mipp.h>

#include "Module/Encoder/Polar/Encoder_polar.hpp"

namespace aff3ct
{
namespace module
{
template<typename B = int>
class Encoder_polar_bitpacked : public Encoder_polar<B>
{
protected:
    mipp::vector<uint64_t> packed_frozen_bits;
    mipp::vector<uint64_t> pack_buffer;

    alignas(64) static uint8_t bit_expand_lut[256][8];
    static bool lut_initialized;

public:
    Encoder_polar_bitpacked(const int& K, const int& N, const std::vector<bool>& frozen_bits);
    virtual ~Encoder_polar_bitpacked() = default;

    virtual Encoder_polar_bitpacked<B>* clone() const;

    virtual void light_encode(B* bits);

    virtual void set_frozen_bits(const std::vector<bool>& frozen_bits);

protected:
    virtual void _encode(const B* U_K, B* X_N, const size_t frame_id);

    static void init_lut();
    static void pack(const B* bits_in, uint64_t* pack_out, const size_t N);
    void pack_systematic(const B* U_K, uint64_t* pack_out, const size_t N) const;
    static void unpack(const uint64_t* pack_in, B* bits_out, const size_t N);
    static void transform_packed(uint64_t* pack_data, const size_t N);
};
}
}

#endif // ENCODER_POLAR_BITPACKED_HPP_
