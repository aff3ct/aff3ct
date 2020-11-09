/*!
 * \file
 * \brief Class module::Encoder_turbo.
 */
#ifndef ENCODER_TURBO_HPP_
#define ENCODER_TURBO_HPP_

#include <memory>
#include <vector>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_turbo : public Encoder<B>
{
protected:
	std::shared_ptr<Interleaver<B>> pi; // the interleaver

	std::shared_ptr<Encoder<B>> enco_n; // polar systematic encoder
	std::shared_ptr<Encoder<B>> enco_i; // sub encoder

	std::vector<B> U_K_i; // internal buffer for the systematic bits in the interleaved domain
	std::vector<B> X_N_tmp;

public:
	Encoder_turbo(const int& K, const int& N, const Encoder<B> &enco_n, const Encoder<B> &enco_i,
	              const Interleaver<B> &pi);
	virtual ~Encoder_turbo() = default;
	virtual Encoder_turbo<B>* clone() const;

	int tail_length() const;

	virtual bool is_codeword(const B *X_N);

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void deep_copy(const Encoder_turbo<B> &m);
	virtual void _encode(const B *U_K, B *X_N, const size_t frame_id);
};
}
}

#endif // ENCODER_TURBO_HPP_
