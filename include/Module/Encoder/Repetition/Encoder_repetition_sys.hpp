/*!
 * \file
 * \brief Class module::Encoder_repetition_sys.
 */
#ifndef ENCODER_REPETITION_SYS_HPP_
#define ENCODER_REPETITION_SYS_HPP_

#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_repetition_sys : public Encoder<B>
{
protected:
	const int rep_count; // number of repetition

	const bool buffered_encoding;

public:
	Encoder_repetition_sys(const int& K, const int& N, const bool buffered_encoding = true);
	virtual ~Encoder_repetition_sys() = default;

	virtual Encoder_repetition_sys<B>* clone() const;

	bool is_codeword(const B *X_N);

protected:
	void _encode(const B *U_K, B *X_N, const size_t frame_id);
};
}
}

#endif // ENCODER_REPETITION_SYS_HPP_
