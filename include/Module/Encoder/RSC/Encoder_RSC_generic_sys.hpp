/*!
 * \file
 * \brief Class module::Encoder_RSC_generic_sys.
 */
#ifndef ENCODER_RSC_GENERIC_SYS_HPP_
#define ENCODER_RSC_GENERIC_SYS_HPP_

#include <vector>

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_RSC_generic_sys : public Encoder_RSC_sys<B>
{
private:
	std::vector<int> out_parity;
	std::vector<int> next_state;
	std::vector<int> sys_tail;

public:
	Encoder_RSC_generic_sys(const int& K, const int& N, const bool buffered_encoding = true,
	                        std::vector<int> poly = std::vector<int>({5,7}));
	virtual ~Encoder_RSC_generic_sys() = default;

	virtual Encoder_RSC_generic_sys<B>* clone() const;

protected:
	virtual int inner_encode(const int bit_sys, int &state);
	virtual int tail_bit_sys(const int &state             );
};
}
}

#endif // ENCODER_RSC_GENERIC_SYS_HPP_
