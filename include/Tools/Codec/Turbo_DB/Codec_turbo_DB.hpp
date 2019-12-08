/*!
 * \file
 * \brief Class tools::Codec_turbo_DB.
 */
#ifndef CODEC_TURBO_DB_HPP_
#define CODEC_TURBO_DB_HPP_

#include <vector>
#include <memory>

#include "Module/CRC/CRC.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"
#include "Factory/Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_turbo_DB : public Codec_SIHO<B,Q>
{
protected:
	std::shared_ptr<std::vector<std::vector<int>>> trellis;

public:
	Codec_turbo_DB(const factory::Encoder_turbo_DB   &enc_params,
	               const factory::Decoder_turbo_DB   &dec_params,
	               const factory::Interleaver        &itl_params,
	               const factory::Puncturer_turbo_DB *pct_params = nullptr,
	               const module::CRC<B>              *crc        = nullptr);
	virtual ~Codec_turbo_DB() = default;

	virtual Codec_turbo_DB<B,Q>* clone() const;

	const std::vector<std::vector<int>>& get_trellis() const;
};
}
}

#endif /* CODEC_TURBO_DB_HPP_ */
