/*!
 * \file
 * \brief Class module::CRC_polynomial_fast.
 */
#ifndef CRC_POLYNOMIAL_FAST_HPP_
#define CRC_POLYNOMIAL_FAST_HPP_

#include <vector>
#include <string>

#include "Module/CRC/Polynomial/CRC_polynomial.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class CRC_polynomial_fast : public CRC_polynomial<B>
{
protected:
	std::vector<unsigned> lut_crc32;
	unsigned polynomial_packed_rev;

public:
	CRC_polynomial_fast(const int K, const std::string &poly_key, const int size = 0);
	virtual ~CRC_polynomial_fast() = default;
	virtual CRC_polynomial_fast<B>* clone() const;

protected:
	virtual void _build       (const B *U_K1, B *U_K2, const size_t frame_id);
	virtual bool _check       (const B *V_K          , const size_t frame_id);
	virtual bool _check_packed(const B *V_K          , const size_t frame_id);

private:
	inline unsigned compute_crc_v1(const void* data, const int n_bits);
	inline unsigned compute_crc_v2(const void* data, const int n_bits);
	inline unsigned compute_crc_v3(const void* data, const int n_bits);
};
}
}

#endif /* CRC_POLYNOMIAL_FAST_HPP_ */
