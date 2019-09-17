#ifndef CRC_POLYNOMIAL_HPP_
#define CRC_POLYNOMIAL_HPP_

#include <string>
#include <vector>
#include <tuple>
#include <map>

#include "Module/CRC/CRC.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class CRC_polynomial : public CRC<B>
{
protected:
	const static std::map<std::string, std::tuple<unsigned, int>> known_polynomials;
	std::vector<B> polynomial;
	unsigned       polynomial_packed;
	std::vector<B> buff_crc;

public:
	CRC_polynomial(const int K, std::string poly_key, const int size = 0, const int n_frames = 1);
	virtual ~CRC_polynomial() = default;

	static int         get_size (std::string poly_key);
	static std::string get_name (std::string poly_key);
	static unsigned    get_value(std::string poly_key);

protected:
	virtual void _build       (const B *U_K1, B *U_K2, const int frame_id);
	virtual void _extract     (const B *V_K1, B *V_K2, const int frame_id);
	virtual bool _check       (const B *V_K          , const int frame_id);
	virtual bool _check_packed(const B *V_K          , const int frame_id);

	void _generate(const B *U_in,
	                     B *U_out,
	               const int off_in,
	               const int off_out,
	               const int loop_size);
};
}
}

#include "Module/CRC/Polynomial/CRC_polynomial.hxx"

#endif /* CRC_POLYNOMIAL_HPP_ */
