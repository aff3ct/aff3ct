#ifndef CRC_POLYNOMIAL_FAST_HPP_
#define CRC_POLYNOMIAL_FAST_HPP_

#include <map>

#include "CRC_polynomial.hpp"

template <typename B>
class CRC_polynomial_fast : public CRC_polynomial<B>
{
protected:
	unsigned polybyte;
	unsigned polybyte_rev;

public:
	CRC_polynomial_fast(const int K, std::string poly_key, const int n_frames = 1, const std::string name = "CRC_polynomial_fast");
	virtual ~CRC_polynomial_fast(){};

	virtual bool check_packed(const mipp::vector<B>& V_K, const int n_frames = -1);
};

#endif /* CRC_POLYNOMIAL_FAST_HPP_ */
