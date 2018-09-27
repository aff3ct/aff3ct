#ifndef DECODER_POLAR_ASCL_FAST_SYS_CA
#define DECODER_POLAR_ASCL_FAST_SYS_CA

#include "Tools/Code/Polar/API/API_polar_dynamic_seq.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"

#include "../SC/Decoder_polar_SC_fast_sys.hpp"
#include "../SCL/CRC/Decoder_polar_SCL_fast_CA_sys.hpp"

#include "Module/CRC/CRC.hpp"

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
	Decoder_polar_SC_fast_sys<B,R,API_polar> sc_decoder;
	const int L_max;
	const bool is_full_adaptive;

public:
	Decoder_polar_ASCL_fast_CA_sys(const int& K, const int& N, const int& max_L, const std::vector<bool>& frozen_bits,
	                               CRC<B>& crc, const bool is_full_adaptive = true, const int n_frames = 1);

	Decoder_polar_ASCL_fast_CA_sys(const int& K, const int& N, const int& max_L, const std::vector<bool>& frozen_bits,
	                               std::vector<std::unique_ptr<tools::Pattern_polar_i>> &&polar_patterns,
	                               const int idx_r0, const int idx_r1,
	                               CRC<B>& crc, const bool is_full_adaptive = true, const int n_frames = 1);

	virtual ~Decoder_polar_ASCL_fast_CA_sys() = default;

	virtual void notify_frozenbits_update();

protected:
	void _decode        (const R *Y_N, B *V_K, const int frame_id);
	void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);
};
}
}

#include "Decoder_polar_ASCL_fast_CA_sys.hxx"

#endif /* DECODER_POLAR_ASCL_FAST_SYS_CA_CA */
