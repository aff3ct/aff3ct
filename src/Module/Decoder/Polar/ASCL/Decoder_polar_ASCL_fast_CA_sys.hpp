#ifndef DECODER_POLAR_ASCL_FAST_SYS_CA
#define DECODER_POLAR_ASCL_FAST_SYS_CA

#include "../SC/Decoder_polar_SC_fast_sys.hpp"
#include "../SCL/CRC/Decoder_polar_SCL_fast_CA_sys.hpp"

#include "Module/CRC/CRC.hpp"

template <typename B, typename R, class API_polar>
class Decoder_polar_ASCL_fast_CA_sys : public Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>
{
private:
	Decoder_polar_SC_fast_sys<B,R,API_polar> sc_decoder;
	const int L_max;
	const bool is_full_adaptive;

public:
	Decoder_polar_ASCL_fast_CA_sys(const int& K, const int& N, const int& max_L, const mipp::vector<B>& frozen_bits,
	                               CRC<B>& crc, const int n_frames = 1,
	                               const std::string name = "Decoder_polar_ASCL_fast_CA_sys");
	virtual ~Decoder_polar_ASCL_fast_CA_sys(){};

	void load       (const mipp::vector<R>& Y_N);
	void hard_decode(                          );
	void store      (mipp::vector<B>& V_N      ) const;
	void unpack     (mipp::vector<B>& V_N      ) const;
	void store_fast (mipp::vector<B>& V        ) const;
};

#include "Decoder_polar_ASCL_fast_CA_sys.hxx"

#endif /* DECODER_POLAR_ASCL_FAST_SYS_CA_CA */
