#ifndef DECODER_POLAR_SCL_FAST_SYS_CA
#define DECODER_POLAR_SCL_FAST_SYS_CA

#include "../Decoder_polar_SCL_fast_sys.hpp"
#include "Module/CRC/CRC.hpp"

template <typename B, typename R, class API_polar>
class Decoder_polar_SCL_fast_CA_sys : public Decoder_polar_SCL_fast_sys<B,R,API_polar>
{
protected:
	CRC<B>& crc;
	mipp::vector<B> U_test;

public:
	Decoder_polar_SCL_fast_CA_sys(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits, CRC<B>& crc,
	                              const int n_frames = 1, const std::string name = "Decoder_polar_SCL_fast_CA_sys");

	Decoder_polar_SCL_fast_CA_sys(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits,
	                              const std::vector<Pattern_polar_i*> polar_patterns, const int idx_r0, const int idx_r1,
	                              CRC<B>& crc, const int n_frames = 1, const std::string name = "Decoder_polar_SCL_fast_CA_sys");

	virtual ~Decoder_polar_SCL_fast_CA_sys(){};

protected:
	        bool crc_check       (mipp::vector<B> &s);
	virtual int  select_best_path(                  );
};

#include "Decoder_polar_SCL_fast_CA_sys.hxx"

#endif /* DECODER_POLAR_SCL_FAST_SYS_CA_CA */
