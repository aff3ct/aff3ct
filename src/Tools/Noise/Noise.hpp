#ifndef NOISE_HPP__
#define NOISE_HPP__

#include <utility>
#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"


namespace aff3ct
{
namespace tools
{

enum class Noise_type {SIGMA, ROP, EP};

inline Noise_type str_to_type(const std::string& str)
{
	Noise_type t;

	if (str == "SIGMA")
		t = Noise_type::SIGMA;
	else if (str == "ROP")
		t = Noise_type::ROP;
	else if (str == "EP")
		t = Noise_type::EP;
	else
	{
		std::stringstream message;
		message << "The string 'str' does not represent a noise type ('str' = " << str << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return t;
}

inline std::string type_to_str(Noise_type t)
{
	std::string str;

	switch(t)
	{
		case Noise_type::SIGMA:
			str = "SIGMA";
			break;
		case Noise_type::EP:
			str = "EP";
			break;
		case Noise_type::ROP:
			str = "ROP";
			break;
	}

	if (str.empty()) // this 'if' is a test outside the switch case (instead of default) to keep the compiler check that all
		// cases of 'Noise_type' are well represented.
	{
		std::stringstream message;
		message << "The type 't' does not represent a noise type ('t' = " << (int8_t)t << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return str;
}


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

#endif // NOISE_HPP__