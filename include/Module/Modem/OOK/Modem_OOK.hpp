/*!
 * \file
 * \brief Class module::Modem_OOK.
 */
#ifndef MODEM_OOK_HPP_
#define MODEM_OOK_HPP_

#include "Module/Modem/Modem.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modem_OOK : public Modem<B,R,Q>
{
public:
	Modem_OOK(const int N);
	virtual ~Modem_OOK() = default;

	virtual Modem_OOK<B,R,Q>* clone() const;

	static bool is_complex_mod();
	static bool is_complex_fil();
	static int size_mod(const int N);
	static int size_fil(const int N);

protected:
	void _modulate(                 const B *X_N1, R *X_N2, const size_t frame_id);
	void   _filter(const float *CP, const R *Y_N1, R *Y_N2, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Modem/OOK/Modem_OOK.hxx"
#endif

#endif /* MODEM_OOK_HPP_ */
