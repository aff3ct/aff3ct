/*!
 * \file
 * \brief Class module::Encoder_NO.
 */
#ifndef ENCODER_NO_HPP_
#define ENCODER_NO_HPP_

#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_NO : public Encoder<B>
{
public:
	Encoder_NO(const int K);
	virtual ~Encoder_NO() = default;

	virtual Encoder_NO<B>* clone() const;

	bool is_codeword(const B *X_K);

protected:
	void _encode(const B *U_K, B *X_K, const size_t frame_id);
};
}
}

#endif /* ENCODER_NO_HPP_ */
