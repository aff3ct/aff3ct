/*!
 * \file
 * \brief Class module::Encoder_RA.
 */
#ifndef ENCODER_RA_HPP_
#define ENCODER_RA_HPP_

#include <vector>
#include <memory>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_RA : public Encoder<B>
{
protected:
	const int rep_count; // number of repetitions
	std::vector<B> U, tmp_X_N;

	std::shared_ptr<Interleaver<B>> interleaver;

public:
	Encoder_RA(const int& K, const int& N, const Interleaver<B>& interleaver);

	virtual ~Encoder_RA() = default;

	virtual Encoder_RA<B>* clone() const;

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void deep_copy(const Encoder_RA<B> &m);
	void _encode(const B *U_K, B *X_N, const size_t frame_id);
};
}
}

#endif // ENCODER_RA_HPP_
