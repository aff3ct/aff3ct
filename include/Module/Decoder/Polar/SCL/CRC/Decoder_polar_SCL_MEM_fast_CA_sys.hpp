/*!
 * \file
 * \brief Class module::Decoder_polar_SCL_MEM_fast_CA_sys.
 */
#ifndef DECODER_POLAR_SCL_MEM_FAST_SYS_CA
#define DECODER_POLAR_SCL_MEM_FAST_SYS_CA

#include <memory>
#include <vector>
#include <mipp.h>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Code/Polar/API/API_polar_dynamic_seq.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_MEM_fast_sys.hpp"

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
class Decoder_polar_SCL_MEM_fast_CA_sys : public Decoder_polar_SCL_MEM_fast_sys<B,R,API_polar>
{
private:
	bool fast_store;

protected:
	std::shared_ptr<CRC<B>> crc;
	mipp::vector<B> U_test;

public:
	Decoder_polar_SCL_MEM_fast_CA_sys(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
	                                  const CRC<B>& crc);

	Decoder_polar_SCL_MEM_fast_CA_sys(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
	                                  const std::vector<tools::Pattern_polar_i*> &polar_patterns,
	                                  const int idx_r0, const int idx_r1, const CRC<B>& crc);

	virtual ~Decoder_polar_SCL_MEM_fast_CA_sys() = default;

	virtual Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>* clone() const;

protected:
	virtual void deep_copy(const Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar> &m);
	        bool crc_check       (mipp::vector<B> &s, const size_t frame_id);
	virtual int  select_best_path(const size_t frame_id);

	virtual void init_buffers();
	virtual void _store(B *V_K) const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_MEM_fast_CA_sys.hxx"
#endif

#endif /* DECODER_POLAR_SCL_MEM_FAST_SYS_CA_CA */
