#ifndef CRC_POLYNOMIAL_INTER_HPP_
#define CRC_POLYNOMIAL_INTER_HPP_

#include <map>

#include "CRC_polynomial.hpp"

template <typename B>
class CRC_polynomial_inter : public CRC_polynomial<B>
{
public:
	CRC_polynomial_inter(const int K, std::string poly_key, const int n_frames, 
	                     const std::string name = "CRC_polynomial_inter");
	virtual ~CRC_polynomial_inter(){};

	virtual bool check(const mipp::vector<B>& V_K, const int n_frames = -1);

protected:
	void _generate_INTER(const mipp::vector<B>& U_in, 
	                           mipp::vector<B>& U_out, 
	                     const int off_in, 
	                     const int off_out, 
	                     const int loop_size);
};

#endif /* CRC_POLYNOMIAL_INTER_HPP_ */
