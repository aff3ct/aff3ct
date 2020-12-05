/*!
 * \file
 * \brief Class module::Decoder_polar_MK_SCL_naive_CA_sys.
 */
#ifndef DECODER_POLAR_MK_SCL_NAIVE_CA_SYS_
#define DECODER_POLAR_MK_SCL_NAIVE_CA_SYS_

#include <functional>
#include <vector>

#include "Tools/Code/Polar/Polar_code.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Polar_MK/SCL/CRC/Decoder_polar_MK_SCL_naive_CA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B=int, typename R=float>
class Decoder_polar_MK_SCL_naive_CA_sys : public Decoder_polar_MK_SCL_naive_CA<B,R>
{

public:
	Decoder_polar_MK_SCL_naive_CA_sys(const int& K,
	                                  const int& N,
	                                  const int& L,
	                                  const tools::Polar_code& code,
	                                  const std::vector<bool>& frozen_bits,
	                                  const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
                                                                                    const std::vector<B> &bits)>>> &lambdas,
	                                  const CRC<B>& crc);

	Decoder_polar_MK_SCL_naive_CA_sys(const int& K,
	                                  const int& N,
	                                  const int& L,
	                                  const tools::Polar_code& code,
	                                  const std::vector<bool>& frozen_bits,
	                                  const CRC<B>& crc);

	virtual ~Decoder_polar_MK_SCL_naive_CA_sys() = default;

	virtual Decoder_polar_MK_SCL_naive_CA_sys<B,R>* clone() const;

protected:
	virtual void select_best_path(const size_t frame_id);
	virtual void _store(B *V, bool coded = false) const;
};
}
}

#endif /* DECODER_POLAR_MK_SCL_NAIVE_CA_SYS_ */
