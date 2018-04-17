#ifndef NOISE_HPP__
#define NOISE_HPP__

#include <utility>
#include <string>
#include <limits>

namespace aff3ct
{
namespace tools
{

enum class Noise_type {SIGMA, ROP, EP};

template <typename R = float>
class Noise
{
public:
	static const R erased_symbol_val;
	static const R erased_llr_val;

public:
	Noise();
	explicit Noise(R noise);
	Noise(const Noise<R>& other);
	Noise(Noise<R>&& other) noexcept;
	virtual ~Noise() = default;

	virtual bool is_set() const noexcept; // return true if the Noise object has been correctly initialized
	bool has_noise() const noexcept; // return true if a noise value has been set

	R get_noise() const; // return the stocked noise, throw if not set

	virtual void set_noise(R noise); // set the noise val and call 'check' to check that it respects the rules

	Noise& operator=(const Noise<R>&  other);          // set this noise as the 'other' one
	Noise& operator=(      Noise<R>&& other) noexcept; // set this noise as the 'other' one
	virtual void copy(const Noise&  other);          // set this noise as the 'other' one
	virtual void copy(      Noise&& other) noexcept; // set this noise as the 'other' one

	virtual Noise_type get_type() const = 0;
	bool is_of_type(Noise_type t) const noexcept;
	void is_of_type_throw(Noise_type t) const;

	static Noise_type str2type(const std::string &str);
	static std::string type2str(Noise_type t);
	std::string type2str();

	virtual Noise<R>* clone() const = 0;

protected:
	std::pair<R, bool> _n;

	virtual void check();
};

}
}

#include "Sigma.hpp"
#include "Received_optical_power.hpp"
#include "Erased_probability.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T, typename R>
Noise<T>* cast(const Noise<R>& n)
{
	Noise<T> * cast_n = nullptr;

//	switch(n.get_type())
//	{
//		case Noise<R>::Noise_type::SIGMA: cast_n = dynamic_cast<const Sigma                 <R>&>(n).cast<T>(); break;
//		case Noise<R>::Noise_type::EP:    cast_n = dynamic_cast<const Erased_probability    <R>&>(n).cast<T>(); break;
//		case Noise<R>::Noise_type::ROP:   cast_n = dynamic_cast<const Received_optical_power<R>&>(n).cast<T>(); break;
//	}

	return cast_n;
}

}
}
#endif // NOISE_HPP__