/*!
 * \file
 * \brief Class module::Decoder_polar_ASCL_fast_CA_sys.
 */
#ifndef DECODER_POLAR_ASCL_FAST_SYS_CA
#define DECODER_POLAR_ASCL_FAST_SYS_CA

#include <vector>
#include <memory>

#include "Tools/Code/Polar/API/API_polar_dynamic_seq.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_fast_sys.hpp"
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_fast_CA_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float,
          class API_polar = tools::API_polar_dynamic_seq<B, R, tools::f_LLR <  R>,
                                                               tools::g_LLR <B,R>,
                                                               tools::g0_LLR<  R>,
                                                               tools::h_LLR <B,R>,
                                                               tools::xo_STD<B  >>>
class Decoder_polar_ASCL_fast_CA_sys : public Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>
{
private:
	std::shared_ptr<Decoder_polar_SC_fast_sys<B,R,API_polar>> sc_decoder;
	const int L_max;
	const bool is_full_adaptive;

public:
	Decoder_polar_ASCL_fast_CA_sys(const int& K, const int& N, const int& max_L, const std::vector<bool>& frozen_bits,
	                               const CRC<B>& crc, const bool is_full_adaptive = true);

	Decoder_polar_ASCL_fast_CA_sys(const int& K, const int& N, const int& max_L, const std::vector<bool>& frozen_bits,
	                               const std::vector<tools::Pattern_polar_i*> &polar_patterns,
	                               const int idx_r0, const int idx_r1,
	                               const CRC<B>& crc, const bool is_full_adaptive = true);

	virtual ~Decoder_polar_ASCL_fast_CA_sys() = default;

	virtual Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>* clone() const;

	virtual void set_frozen_bits(const std::vector<bool>& frozen_bits);
	virtual const std::vector<bool>& get_frozen_bits() const;

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void deep_copy(const Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar> &m);

	void _decode        (const R *Y_N, B *V_K, const size_t frame_id);
	int  _decode_siho   (const R *Y_N, B *V_K, const size_t frame_id);
	int  _decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/ASCL/Decoder_polar_ASCL_fast_CA_sys.hxx"
#endif

#endif /* DECODER_POLAR_ASCL_FAST_SYS_CA_CA */
