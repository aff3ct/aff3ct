/*!
 * \file
 * \brief Class module::Encoder_BCH_inter.
 */
#ifndef ENCODER_BCH_INTER_HPP_
#define ENCODER_BCH_INTER_HPP_

#include <vector>

#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"
#include "Module/Encoder/BCH/Encoder_BCH.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_BCH_inter : public Encoder_BCH<B>
{
protected:
	std::vector<B> U_K_reordered;
	std::vector<B> par_reordered;

	std::vector<mipp::Reg<B>> regs_par;

public:
	Encoder_BCH_inter(const int& K, const int& N, const tools::BCH_polynomial_generator<B>& GF);

	virtual ~Encoder_BCH_inter() = default;

	virtual Encoder_BCH_inter<B>* clone() const;

protected:
	virtual void  _encode(const B *U_K, B *X_N, const size_t frame_id);
	virtual void __encode(const B *U_K, B *bb);
};
}
}

#endif // ENCODER_BCH_INTER_HPP_
