#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Noise.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
Noise<R>::
Noise()
: _n((R)0,false)
{
}

template <typename R>
Noise<R>::
Noise(R noise)
: _n((R)0,false)
{
	set_noise(noise);
}

template <typename R>
template <typename T>
Noise<R>::
Noise(const Noise<T>& other)
{
	if (other.has_noise())
	{
		set_noise((R)other.get_noise());
	}
	else
	{
		_n.first  = (R)0;
		_n.second = false;
	}
}

template <typename R>
Noise<R>& Noise<R>::
operator=(const Noise<R>& other)
{
	this->copy(other);
	return *this;
}

template <typename R>
void Noise<R>::
copy(const Noise<R>& other)
{
	this->_n = other._n;
}

template <typename R>
bool Noise<R>::
is_set() const noexcept
{
	return has_noise();
}

template <typename R>
bool Noise<R>::
has_noise() const noexcept
{
	return _n.second;
}

template <typename R>
void Noise<R>::
set_noise(R noise)
{
	_n.first  = noise;
	_n.second = true;
	this->check();
}

template <typename R>
R Noise<R>::
get_noise() const
{
	if (!has_noise())
	{
		std::stringstream message;
		message << "Ask for the noise value but it has not been set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return _n.first;
}

template <typename R>
void Noise<R>::
check()
{
	// nothing to check
}

template <typename R>
Noise_type Noise<R>::
str2type(const std::string& str)
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

template <typename R>
std::string Noise<R>::
type2str()
{
	return type2str(this->get_type());
}

template <typename R>
std::string Noise<R>::
type2str(Noise_type t)
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

template<typename R>
bool Noise<R>
::is_of_type(Noise_type t) const noexcept
{
	return this->get_type() == t;
}

template<typename R>
void Noise<R>
::is_of_type_throw(Noise_type t) const
{
	if (!is_of_type(t))
	{
		std::stringstream message;
		message << "The given noise value does not represent a '" << type2str(t) << "' noise type.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}


// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Noise<float>;
template class aff3ct::tools::Noise<double>;

template aff3ct::tools::Noise<double>::Noise(const Noise<float >&);
template aff3ct::tools::Noise<float >::Noise(const Noise<double>&);
// ==================================================================================== explicit template instantiation
