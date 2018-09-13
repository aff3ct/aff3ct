#ifndef CRC_POLYNOMIAL_INTER_HPP_
#define CRC_POLYNOMIAL_INTER_HPP_

#include <map>

#include "CRC_polynomial.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class CRC_polynomial_inter : public CRC_polynomial<B>
{
public:
	CRC_polynomial_inter(const int K, std::string poly_key, const int size, const int n_frames);
	virtual ~CRC_polynomial_inter() = default;

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
