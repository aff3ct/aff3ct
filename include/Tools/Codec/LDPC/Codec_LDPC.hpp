/*!
 * \file
 * \brief Class tools::Codec_LDPC.
 */
#ifndef CODEC_LDPC_HPP_
#define CODEC_LDPC_HPP_

#include <memory>
#include <vector>

#include "Factory/Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Factory/Module/Puncturer/LDPC/Puncturer_LDPC.hpp"
#include "Factory/Module/Decoder/LDPC/Decoder_LDPC.hpp"
#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Standard/DVBS2/DVBS2_constants.hpp"
#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"
#include "Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_LDPC : public Codec_SISO<B,Q>
{
protected:
	std::shared_ptr<Sparse_matrix> H;
	std::shared_ptr<Sparse_matrix> G;
	std::shared_ptr<LDPC_matrix_handler::Positions_vector> info_bits_pos;
	std::shared_ptr<dvbs2_values> dvbs2;

public:
	Codec_LDPC(const factory::Encoder_LDPC   &enc_params,
	           const factory::Decoder_LDPC   &dec_params,
	                 factory::Puncturer_LDPC *pct_params);
	virtual ~Codec_LDPC() = default;

	virtual Codec_LDPC<B,Q>* clone() const;

	const Sparse_matrix& get_H() const;
	const Sparse_matrix& get_G() const;
	const LDPC_matrix_handler::Positions_vector& get_info_bits_pos() const;
	const dvbs2_values& get_DVBS2() const;
};
}
}

#endif /* CODEC_LDPC_HPP_ */
