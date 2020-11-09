/*!
 * \file
 * \brief Class module::Encoder_turbo_legacy.
 */
#ifndef ENCODER_TURBO_LEGACY_HPP_
#define ENCODER_TURBO_LEGACY_HPP_

#include <memory>
#include <vector>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_turbo_legacy : public Encoder_turbo<B>
{
protected:
	std::shared_ptr<Encoder<B>> sub_enc; // sub encoder
	std::vector<B>              X_N_n;   // internal buffer for the encoded bits in the natural     domain
	std::vector<B>              X_N_i;   // internal buffer for the encoded bits in the interleaved domain

public:
	Encoder_turbo_legacy(const int& K, const int& N, const Encoder<B> &sub_enc, const Interleaver<B> &pi);
	virtual ~Encoder_turbo_legacy() = default;
	virtual Encoder_turbo_legacy<B>* clone() const;

	bool is_codeword(const B *X_N);

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void deep_copy(const Encoder_turbo_legacy<B> &m);
	void _encode(const B *U_K, B *X_N, const size_t frame_id);
};
}
}

#endif // ENCODER_TURBO_LEGACY_HPP_
