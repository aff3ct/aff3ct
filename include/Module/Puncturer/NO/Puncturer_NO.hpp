/*!
 * \file
 * \brief Class module::Puncturer_NO.
 */
#ifndef PUNCTURER_NO_HPP_
#define PUNCTURER_NO_HPP_

#include "Module/Puncturer/Puncturer.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Puncturer_NO : public Puncturer<B,Q>
{
public:
	Puncturer_NO(const int K, const int N);
	virtual ~Puncturer_NO() = default;

	virtual Puncturer_NO<B,Q>* clone() const;

protected:
	void   _puncture(const B *X_N1, B *X_N2, const size_t frame_id) const;
	void _depuncture(const Q *Y_N1, Q *Y_N2, const size_t frame_id) const;
};
}
}

#endif /* PUNCTURER_NO_HPP_ */
