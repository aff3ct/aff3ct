/*!
 * \file
 * \brief Class module::Encoder_LDPC_DVBS2.
 */
#ifndef ENCODER_LDPC_DVBS2_HPP_
#define ENCODER_LDPC_DVBS2_HPP_

#include "Tools/Code/LDPC/Standard/DVBS2/DVBS2_constants.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Encoder_LDPC_DVBS2 : public Encoder_LDPC<B>
{
	const tools::dvbs2_values& dvbs2;

public:
	Encoder_LDPC_DVBS2(const tools::dvbs2_values& dvbs2);
	virtual ~Encoder_LDPC_DVBS2() = default;

	virtual Encoder_LDPC_DVBS2<B>* clone() const;

protected:
	void _encode(const B *U_K, B *X_N, const size_t frame_id);
};

}
}

#endif /* ENCODER_LDPC_DVBS2_HPP_ */
