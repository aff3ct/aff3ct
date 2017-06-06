#ifndef CODEC_LDPC_HPP_
#define CODEC_LDPC_HPP_

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"

#include "../Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_LDPC : public Codec_SISO<B,Q>
{
protected:
	AList_reader alist_data;
	mipp::vector<B> info_bits_pos;

	std::vector<module::Decoder_SISO<B,Q>*> decoder_siso;

public:
	Codec_LDPC(const parameters& params);
	virtual ~Codec_LDPC();

	module::Encoder_LDPC<B  >* build_encoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::Decoder_SISO<B,Q>* build_siso   (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                  module::CRC        <B  >* crc = nullptr);
	module::Decoder     <B,Q>* build_decoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                  module::CRC        <B  >* crc = nullptr);

	void extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par);
};
}
}

#endif /* CODEC_LDPC_HPP_ */
