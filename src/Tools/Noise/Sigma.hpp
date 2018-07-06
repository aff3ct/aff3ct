#ifndef SIGMA_HPP_
#define SIGMA_HPP_

#include "Noise.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class Sigma : public Noise<R>
{
public:
	Sigma();
	explicit Sigma(R noise);
	Sigma(R noise, R ebn0, R esn0);
	template<typename T>
	explicit Sigma(const Sigma<T>& other);
	virtual ~Sigma() = default;

	virtual bool is_set() const noexcept; // return true sigma, ebn0 and esn0 have been initialized

	const R get_ebn0() const; // return EB/N0 when has been set, throw if it is not
	const R get_esn0() const; // return ES/N0 when has been set, throw if it is not

	bool has_ebn0() const noexcept; // return true if ebn0 value has been set
	bool has_esn0() const noexcept; // return true if esn0 value has been set

	using Noise<R>::set_noise;
	void set_noise(R sigma, R ebn0, R esn0); // set the noise (sigma) and also EB/N0 and ES/N0

	virtual void copy(const Sigma& other); // set this noise as the 'other' one

	virtual std::string get_unity() const;
	virtual Noise_type get_type() const;

	virtual Sigma<R>* clone() const;

protected:
	std::pair<R, bool> _ebn0;
	std::pair<R, bool> _esn0;

	virtual void check();
};

}
}

#endif //SIGMA_HPP_
