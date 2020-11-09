/*!
 * \file
 * \brief Class module::Encoder_turbo_DB.
 */
#ifndef ENCODER_TURBO_DB_HPP_
#define ENCODER_TURBO_DB_HPP_

#include <memory>
#include <vector>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_turbo_DB : public Encoder<B>
{
protected:
	std::shared_ptr<Interleaver<B>> pi; // the interleaver

	std::shared_ptr<Encoder_RSC_DB<B>> enco_n; // encoder natural order
	std::shared_ptr<Encoder_RSC_DB<B>> enco_i; // encoder interleaved order

	std::vector<B> U_K_cpy;
	std::vector<B> U_K_i;   // internal buffer for the systematic bits in the interleaved domain
	std::vector<B> X_N_tmp;
	std::vector<B> par_n;   // internal buffer for the encoded    bits in the natural     domain
	std::vector<B> par_i;   // internal buffer for the encoded    bits in the interleaved domain

	bool is_codeword(const B *X_N);

public:
	Encoder_turbo_DB(const int& K, const int& N, const Encoder_RSC_DB<B> &enco_n, const Encoder_RSC_DB<B> &enco_i,
	                 const Interleaver<B> &pi);
	virtual ~Encoder_turbo_DB() = default;

	virtual Encoder_turbo_DB<B>* clone() const;

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void deep_copy(const Encoder_turbo_DB<B> &m);
	void _encode(const B *U_K, B *X_N, const size_t frame_id);
};
}
}

#endif // ENCODER_TURBO_HPP_
