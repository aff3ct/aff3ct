#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Noise.hpp"

using namespace aff3ct;
using namespace tools;

template <typename R>
Noise<R>::
Noise()
{
	init();
}

template <typename R>
Noise<R>::
Noise(const R noise, const Noise_type t)
{
	init();
	set_noise(noise, t);
}

template <typename R>
Noise<R>::
Noise(const Noise<R>& other)
: _t   (other._t   ),
  _n   (other._n   ),
  _ebn0(other._ebn0),
  _esn0(other._esn0)
{
}

template <typename R>
Noise<R>::
Noise(Noise<R>&& other)
: _t   (std::move(other._t   )),
  _n   (std::move(other._n   )),
  _ebn0(std::move(other._ebn0)),
  _esn0(std::move(other._esn0))
{
}

template <typename R>
Noise<R>& Noise<R>::
operator=(const Noise<R>& other)
{
	_t    = other._t   ;
	_n    = other._n   ;
	_ebn0 = other._ebn0;
	_esn0 = other._esn0;
	return *this;
}

template <typename R>
Noise<R>& Noise<R>::
operator=(Noise<R>&& other)
{
	_t    = std::move(other._t   );
	_n    = std::move(other._n   );
	_ebn0 = std::move(other._ebn0);
	_esn0 = std::move(other._esn0);
	return *this;
}

template <typename R>
Noise<R>::
~Noise()
{

}

template <typename R>
bool Noise<R>::
is_set() const throw()
{
	return has_type() && has_noise();
}

template <typename R>
bool Noise<R>::
has_type() const throw()
{
	return _t.second;
}

template <typename R>
bool Noise<R>::
has_noise() const throw()
{
	return _n.second;
}

template <typename R>
Noise_type Noise<R>::
get_type() const
{
	if (!has_type())
	{
		std::stringstream message;
		message << "Ask for the noise type but it has not been set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return _t.first;
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
R Noise<R>::
get_sigma() const
{
	if (get_type() != Noise_type::SIGMA)
	{
		std::stringstream message;
		message << "Ask for the noise as SIGMA but it has another type ('_t' = " << type2str(get_type()) << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return get_noise();
}

template <typename R>
R Noise<R>::
get_rop() const
{
	if (get_type() != Noise_type::ROP)
	{
		std::stringstream message;
		message << "Ask for the noise as ROP but it has another type ('_t' = " << type2str(get_type()) << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return get_noise();
}

template <typename R>
R Noise<R>::
get_ep() const
{
	if (get_type() != Noise_type::EP)
	{
		std::stringstream message;
		message << "Ask for the noise as EP but it has another type ('_t' = " << type2str(get_type()) << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return get_noise();
}

template <typename R>
const R Noise<R>::
get_ebn0() const
{
	if (get_type() != Noise_type::SIGMA)
	{
		std::stringstream message;
		message << "Ask for the EB/N0 but noise has a type other than SIGMA ('_t' = " << type2str(get_type()) << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (!_ebn0.second)
	{
		std::stringstream message;
		message << "Ask for the EB/N0 but it has not been set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return _ebn0.first;
}

template <typename R>
const R Noise<R>::
get_esn0() const
{
	if (get_type() != Noise_type::SIGMA)
	{
		std::stringstream message;
		message << "Ask for the ES/N0 but noise has a type other than SIGMA ('_t' = " << type2str(get_type()) << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (!_esn0.second)
	{
		std::stringstream message;
		message << "Ask for the ES/N0 but it has not been set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return _esn0.first;
}

template <typename R>
void Noise<R>::
set_t(const Noise_type t)
{
	_t.first  = t;
	_t.second = true;
}

template <typename R>
void Noise<R>::
set_n(const R n)
{
	_n.first  = n;
	_n.second = true;
}

template <typename R>
void Noise<R>::
set_noise(const R noise, const Noise_type t)
{
	switch(t)
	{
		case Noise_type::SIGMA:
			set_sigma(noise);
		break;

		case Noise_type::EP:
			set_ep(noise);
		break;

		case Noise_type::ROP:
			set_rop(noise);
		break;
	}
}

template <typename R>
void Noise<R>::
set_rop(const R rop)
{
	set_n(rop);
	set_t(Noise_type::ROP);
	check();
}

template <typename R>
void Noise<R>::
set_ep(const R ep)
{
	set_n(ep);
	set_t(Noise_type::EP);
	check();
}

template <typename R>
void Noise<R>::
set_sigma(const R sigma)
{
	set_n(sigma);
	set_t(Noise_type::SIGMA);
	_ebn0.second = false;
	_esn0.second = false;
	check();
}

template <typename R>
void Noise<R>::
set_sigma(const R sigma, const R ebn0, const R esn0)
{
	set_n(sigma);
	set_t(Noise_type::SIGMA);
	_ebn0.first = ebn0;  _ebn0.second = true;
	_esn0.first = esn0;  _esn0.second = true;
	check();
}

template <typename R>
void Noise<R>::
check()
{
	auto n = get_noise();

	switch(get_type())
	{
		case Noise_type::SIGMA:
			if (n <= (R)0)
			{
				std::stringstream message;
				message << "The SIGMA noise '_n' has to be greater than 0 ('_n' = " << n << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		break;

		case Noise_type::EP:
			if (n < (R)-0.00001 || n > (R)1.00001)
			{
				std::stringstream message;
				message << "The EP noise '_n' has to be between [0,1] ('_n' = " << n << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		break;

		case Noise_type::ROP:
			// nothing to check
		break;
	}

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
type2str(const Noise_type& t)
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

	if (str.empty()) // if is a test outside the switch case (instead of default) to keep the compiler check that all
	                 // cases of 'Noise_type' are well represented.
	{
		std::stringstream message;
		message << "The type 't' does not represent a noise type ('t' = " << (int8_t)t << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return str;
}

template <typename R>
void Noise<R>::
init()
{
	_t.second    = false;
	_n.second    = false;
	_ebn0.second = false;
	_esn0.second = false;
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Noise<float>;
template class aff3ct::tools::Noise<double>;
// ==================================================================================== explicit template instantiation
