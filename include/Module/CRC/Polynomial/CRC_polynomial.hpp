/*!
 * \file
 * \brief Class module::CRC_polynomial.
 */
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
	CRC_polynomial(const int K, const std::string &poly_key, const int size = 0);
	virtual ~CRC_polynomial() = default;
	virtual CRC_polynomial<B>* clone() const;

	static int         get_size (const std::string &poly_key);
	static std::string get_name (const std::string &poly_key);
	static unsigned    get_value(const std::string &poly_key);

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void _build       (const B *U_K1, B *U_K2, const size_t frame_id);
	virtual void _extract     (const B *V_K1, B *V_K2, const size_t frame_id);
	virtual bool _check       (const B *V_K          , const size_t frame_id);
	virtual bool _check_packed(const B *V_K          , const size_t frame_id);

	void _generate(const B *U_in,
	                     B *U_out,
	               const int off_in,
	               const int off_out,
	               const int loop_size);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/CRC/Polynomial/CRC_polynomial.hxx"
#endif

#endif /* CRC_POLYNOMIAL_HPP_ */
