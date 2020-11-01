/*!
 * \file
 * \brief Class module::CRC_polynomial_inter.
 */
#ifndef CRC_POLYNOMIAL_INTER_HPP_
#define CRC_POLYNOMIAL_INTER_HPP_

#include <string>

#include "Module/CRC/CRC.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class CRC_polynomial_inter : public CRC_polynomial<B>
{
public:
	CRC_polynomial_inter(const int K, const std::string &poly_key, const int size);
	virtual ~CRC_polynomial_inter() = default;
	virtual CRC_polynomial_inter<B>* clone() const;

	virtual bool check(const B *V_K, const int n_frames = -1, const int frame_id = -1); using CRC<B>::check;

protected:
	void _generate_INTER(const B *U_in,
	                           B *U_out,
	                     const int off_in,
	                     const int off_out,
	                     const int loop_size,
	                     const int n_frames);
};
}
}

#endif /* CRC_POLYNOMIAL_INTER_HPP_ */
