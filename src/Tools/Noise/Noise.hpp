#ifndef NOISE_HPP__
#define NOISE_HPP__

#include <string>

namespace aff3ct
{
namespace tools
{
enum class Noise_type {SIGMA, ROP, EP};

inline Noise_type str_to_type(const std::string& str);

inline std::string type_to_str(Noise_type t);

template <typename R = float>
class Noise
{
public:
	Noise();
	explicit Noise(R noise);
	template<typename T>
	explicit Noise(const Noise<T>& other);
	virtual ~Noise() = default;

	virtual bool is_set() const noexcept; // return true if the Noise object has been correctly initialized
	bool has_noise() const noexcept; // return true if a noise value has been set

	R get_noise() const; // return the stocked noise, throw if not set

	virtual void set_noise(R noise); // set the noise val and call 'check' to check that it respects the rules

	Noise& operator= (const Noise<R>& other); // set this noise as the 'other' one
	virtual void copy(const Noise<R>& other); // set this noise as the 'other' one

	virtual std::string get_unity() const = 0; // return a string with the unity of the noise type
	virtual Noise_type  get_type () const = 0;

	bool is_of_type      (Noise_type t) const noexcept;
	void is_of_type_throw(Noise_type t) const;

	virtual Noise<R>* clone() const = 0;

protected:
	std::pair<R, bool> _n;

	virtual void check();
};
}
}

#include "Tools/Noise/Noise.hxx"

#endif // NOISE_HPP__