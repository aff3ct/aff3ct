#include <limits>
#include <sstream>
#include <typeinfo>

#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/Sigma.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template<typename R>
Sigma<R>
::Sigma(R sigma)
: Noise<R>(sigma),
  ebn0(std::numeric_limits<R>::infinity()),
  esn0(std::numeric_limits<R>::infinity())
{
	this->check();
}

template<typename R>
Sigma<R>
::Sigma(R sigma, R ebn0, R esn0)
: Noise<R>(sigma),
  ebn0(ebn0),
  esn0(esn0)
{
	this->check();
}

template <typename R>
void Sigma<R>
::copy(const Sigma<R>& other)
{
	this->ebn0 = other.ebn0;
	this->esn0 = other.esn0;
	Noise<R>::copy(other);
}

template <typename R>
void Sigma<R>
::copy(const Noise<R>& other)
{
	try
	{
		this->copy(dynamic_cast<const Sigma<R>&>(other));
	}
	catch (std::bad_cast&)
	{
		Noise<R>::copy(other);
	}
}

template <typename R>
const R Sigma<R>
::get_sigma() const
{
	return this->get_value();
}

template <typename R>
const R Sigma<R>
::get_ebn0() const
{
	if (this->ebn0 == std::numeric_limits<R>::infinity())
	{
		std::stringstream message;
		message << "'ebn0' is not set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->ebn0;
}

template <typename R>
const R Sigma<R>
::get_esn0() const
{
	if (this->esn0 == std::numeric_limits<R>::infinity())
	{
		std::stringstream message;
		message << "'esn0' is not set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->esn0;
}

template <typename R>
void Sigma<R>
::set_values(R sigma, R ebn0, R esn0)
{
	Noise<R>::set_value(sigma); // call check
	this->ebn0 = ebn0;
	this->esn0 = esn0;
}

template <typename R>
void Sigma<R>
::check() const
{
	auto value = this->get_value();
	if (value <= (R)0)
	{
		std::stringstream message;
		message << "The SIGMA noise 'value' has to be greater than 0 ('value' = " << value << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename R>
Noise_type Sigma<R>
::get_type() const
{
	return Noise_type::SIGMA;
}

template<typename R>
std::string Sigma<R>
::get_unity() const
{
	return "dB";
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Sigma<float>;
template class aff3ct::tools::Sigma<double>;
// ==================================================================================== explicit template instantiation
