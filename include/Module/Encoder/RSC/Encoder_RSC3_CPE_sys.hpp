/*!
 * \file
 * \brief Class module::Encoder_RSC3_CPE_sys.
 */
#ifndef ENCODER_RSC3_CPE_SYS_HPP_
#define ENCODER_RSC3_CPE_SYS_HPP_

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_RSC3_CPE_sys : public Encoder_RSC_sys<B>
{
public:
	Encoder_RSC3_CPE_sys(const int& K, const int& N, const bool buffered_encoding = true);
	virtual ~Encoder_RSC3_CPE_sys() = default;

	virtual Encoder_RSC3_CPE_sys<B>* clone() const;

protected:
	int inner_encode(const int bit_sys, int &state);
	int tail_bit_sys(const int &state             );
};
}
}

#endif // ENCODER_RSC3_CPE_SYS_HPP_
