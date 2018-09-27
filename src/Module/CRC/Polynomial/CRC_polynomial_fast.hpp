#ifndef CRC_POLYNOMIAL_FAST_HPP_
#define CRC_POLYNOMIAL_FAST_HPP_

#include <map>

#include "CRC_polynomial.hpp"

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
	CRC_polynomial_fast(const int K, std::string poly_key, const int size = 0, const int n_frames = 1);
	virtual ~CRC_polynomial_fast() = default;

protected:
	virtual void _build       (const B *U_K1, B *U_K2, const int frame_id);
	virtual bool _check       (const B *V_K          , const int frame_id);
	virtual bool _check_packed(const B *V_K          , const int frame_id);

private:
	inline unsigned compute_crc_v1(const void* data, const int n_bits);
	inline unsigned compute_crc_v2(const void* data, const int n_bits);
	inline unsigned compute_crc_v3(const void* data, const int n_bits);
};
}
}

#endif /* CRC_POLYNOMIAL_FAST_HPP_ */
