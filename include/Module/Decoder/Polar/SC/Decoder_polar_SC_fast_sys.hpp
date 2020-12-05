/*!
 * \file
 * \brief Class module::Decoder_polar_SC_fast_sys.
 */
#ifndef DECODER_POLAR_SC_FAST_SYS_
#define DECODER_POLAR_SC_FAST_SYS_

#include <vector>
#include <mipp.h>

#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hpp"
#include "Tools/Code/Polar/Pattern_polar_parser.hpp"
#include "Tools/Code/Polar/API/API_polar_dynamic_seq.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, class API_polar>
class Decoder_polar_ASCL_fast_CA_sys;

template <typename B, typename R, class API_polar>
class Decoder_polar_ASCL_MEM_fast_CA_sys;

template <typename B = int, typename R = float,
          class API_polar = tools::API_polar_dynamic_seq<B, R, tools::f_LLR <  R>,
                                                               tools::g_LLR <B,R>,
                                                               tools::g0_LLR<  R>,
                                                               tools::h_LLR <B,R>,
                                                               tools::xo_STD<B  >>>
class Decoder_polar_SC_fast_sys : public Decoder_SIHO<B,R>, public tools::Interface_get_set_frozen_bits
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend Decoder_polar_ASCL_fast_CA_sys    <B,R,API_polar>;
	friend Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>;
#endif

protected:
	const int                m;            // graph depth
	      mipp::vector<R   > l;            // lambda, LR or LLR
	      mipp::vector<B   > s;            // bits, partial sums
	      mipp::vector<B   > s_bis;        // bits, partial sums
	      std::vector<bool> frozen_bits;  // frozen bits

	tools::Pattern_polar_parser polar_patterns;

public:
	Decoder_polar_SC_fast_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits);

	Decoder_polar_SC_fast_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits,
	                          const std::vector<tools::Pattern_polar_i*> &polar_patterns,
	                          const int idx_r0, const int idx_r1);

	virtual ~Decoder_polar_SC_fast_sys() = default;

	virtual Decoder_polar_SC_fast_sys<B,R,API_polar>* clone() const;

	virtual void set_frozen_bits(const std::vector<bool>& frozen_bits);
	virtual const std::vector<bool>& get_frozen_bits() const;

protected:
	        void _load          (const R *Y_N                               );
	virtual void _decode        (                                           );
	        int  _decode_siho   (const R *Y_N, B *V_K, const size_t frame_id);
	        int  _decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id);
	        void _store         (              B *V_K                       );
	        void _store_cw      (              B *V_N                       );

	virtual void recursive_decode(const int off_l, const int off_s, const int reverse_depth, int &node_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_fast_sys.hxx"
#endif

#endif /* DECODER_POLAR_SC_FAST_SYS_ */
