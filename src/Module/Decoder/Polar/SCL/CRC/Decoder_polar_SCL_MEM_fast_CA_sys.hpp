#ifndef DECODER_POLAR_SCL_MEM_FAST_SYS_CA
#define DECODER_POLAR_SCL_MEM_FAST_SYS_CA

#include "Tools/Code/Polar/API/API_polar_dynamic_seq.hpp"
#include "Module/CRC/CRC.hpp"

#include "../Decoder_polar_SCL_MEM_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float,
          class API_polar = tools::API_polar_dynamic_seq<B, R, f_LLR<R>, g_LLR<B,R>, g0_LLR<R>, h_LLR<B,R>, xo_STD<B>>>
class Decoder_polar_SCL_MEM_fast_CA_sys : public Decoder_polar_SCL_MEM_fast_sys<B,R,API_polar>
{
protected:
	CRC<B>& crc;
	mipp::vector<B> U_test;

public:
	Decoder_polar_SCL_MEM_fast_CA_sys(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits, CRC<B>& crc,
	                                  const int n_frames = 1, const std::string name = "Decoder_polar_SCL_MEM_fast_CA_sys");
	virtual ~Decoder_polar_SCL_MEM_fast_CA_sys(){};

protected:
	        bool crc_check       (mipp::vector<B> &s);
	virtual int  select_best_path(                  );
};
}
}

#include "Decoder_polar_SCL_MEM_fast_CA_sys.hxx"

#endif /* DECODER_POLAR_SCL_MEM_FAST_SYS_CA_CA */
