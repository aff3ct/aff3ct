#ifndef CODEC_LDPC_HPP_
#define CODEC_LDPC_HPP_

#include <cstdint>

#include "Factory/Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Factory/Module/Decoder/LDPC/Decoder_LDPC.hpp"

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

#include "../Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_LDPC : public Codec_SISO_SIHO<B,Q>
{
protected:
	tools::Sparse_matrix H;
	tools::Sparse_matrix G;
	std::vector<uint32_t> info_bits_pos;

public:
	Codec_LDPC(const factory::Encoder_LDPC::parameters &enc_params,
	           const factory::Decoder_LDPC::parameters &dec_params,
	           const std::string name = "Codec_LDPC");
	virtual ~Codec_LDPC();

protected:
	void _extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id);
};
}
}

#endif /* CODEC_LDPC_HPP_ */
