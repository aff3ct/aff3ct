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
protected:
	std::vector<B> buff_crc_inter;

public:
	CRC_polynomial_inter(const int K, const std::string &poly_key, const int size);
	virtual ~CRC_polynomial_inter() = default;
	virtual CRC_polynomial_inter<B>* clone() const;

protected:
	virtual void _build       (const B *U_K1, B *U_K2, const size_t frame_id);
	virtual bool _check       (const B *V_K          , const size_t frame_id);
	virtual bool _check_packed(const B *V_K          , const size_t frame_id);
	virtual void _extract     (const B *V_K1, B *V_K2, const size_t frame_id);

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
