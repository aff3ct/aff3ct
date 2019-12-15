/*!
 * \file
 * \brief Class tools::Sigma.
 */
#ifndef SIGMA_HPP_
#define SIGMA_HPP_

#include <utility>

#include "Tools/Noise/Noise.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Sigma : public Noise<R>
{
protected:
	R ebn0;
	R esn0;

public:
	Sigma() = default;
	explicit Sigma(R sigma);
	Sigma(R sigma, R ebn0, R esn0);
	virtual ~Sigma() = default;

	const R get_sigma() const;
	const R get_ebn0 () const; // return EB/N0 when has been set, throw if it is not
	const R get_esn0 () const; // return ES/N0 when has been set, throw if it is not

	void set_values(R sigma, R ebn0, R esn0); // set the noise (sigma) and also EB/N0 and ES/N0

	virtual void copy(const Sigma<R>& other); // set this noise as the 'other' one
	virtual void copy(const Noise<R>& other);

	virtual std::string get_unity() const;
	virtual Noise_type get_type() const;

protected:
	virtual void check() const;
};

}
}

#endif //SIGMA_HPP_
