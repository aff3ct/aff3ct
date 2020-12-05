/*!
 * \file
 * \brief Class module::Decoder_polar_MK_SCL_naive_CA.
 */
#ifndef DECODER_POLAR_MK_SCL_NAIVE_CA_
#define DECODER_POLAR_MK_SCL_NAIVE_CA_

#include <functional>
#include <vector>

#include "Tools/Code/Polar/Polar_code.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Polar_MK/SCL/Decoder_polar_MK_SCL_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B=int, typename R=float>
class Decoder_polar_MK_SCL_naive_CA : public Decoder_polar_MK_SCL_naive<B,R>
{
protected:
	std::shared_ptr<CRC<B>> crc;

public:
	Decoder_polar_MK_SCL_naive_CA(const int& K,
	                              const int& N,
	                              const int& L,
	                              const tools::Polar_code& code,
	                              const std::vector<bool>& frozen_bits,
	                              const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
                                                                                const std::vector<B> &bits)>>> &lambdas,
	                              const CRC<B>& crc);

	Decoder_polar_MK_SCL_naive_CA(const int& K,
	                              const int& N,
	                              const int& L,
	                              const tools::Polar_code& code,
	                              const std::vector<bool>& frozen_bits,
	                              const CRC<B>& crc);

	virtual ~Decoder_polar_MK_SCL_naive_CA() = default;

	virtual Decoder_polar_MK_SCL_naive_CA<B,R>* clone() const;

protected:
	virtual void deep_copy(const Decoder_polar_MK_SCL_naive_CA<B,R>& m);
	virtual void select_best_path(const size_t frame_id);
};
}
}

#endif /* DECODER_POLAR_MK_SCL_NAIVE_CA_ */
