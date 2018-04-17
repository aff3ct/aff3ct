#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Sigma.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template<typename R>
Sigma<R>::Sigma()
: Noise<R>(),
  _ebn0((R)0,false),
  _esn0((R)0,false)
{

}

template<typename R>
Sigma<R>::Sigma(R noise)
: Noise<R>(noise),
  _ebn0((R)0,false),
  _esn0((R)0,false)
{

}

template<typename R>
Sigma<R>::Sigma(R noise, R ebn0, R esn0)
: Noise<R>(noise),
  _ebn0(ebn0,true),
  _esn0(esn0,true)
{

}

template <typename R>
Sigma<R>::
Sigma(const Sigma<R>& other)
: Noise<R>(other),
  _ebn0(other._ebn0),
  _esn0(other._esn0)
{
}

template <typename R>
Sigma<R>::
Sigma(Sigma<R>&& other)
: Noise<R>(std::move(other)),
  _ebn0(std::move(other._ebn0)),
  _esn0(std::move(other._esn0))
{
}

template <typename R>
void Sigma<R>::
copy(const Sigma<R>& other)
{
	_ebn0 = other._ebn0;
	_esn0 = other._esn0;
	Noise<R>::copy(other);
}

template <typename R>
void Sigma<R>::
copy(Sigma<R>&& other) noexcept
{
	_ebn0 = std::move(other._ebn0);
	_esn0 = std::move(other._esn0);
	Noise<R>::copy(std::move(other));
}

template <typename R>
const R Sigma<R>::
get_ebn0() const
{
	if (!_ebn0.second)
	{
		std::stringstream message;
		message << "Ask for the EB/N0 but it has not been set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return _ebn0.first;
}

template <typename R>
const R Sigma<R>::
get_esn0() const
{
	if (!_esn0.second)
	{
		std::stringstream message;
		message << "Ask for the ES/N0 but it has not been set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return _esn0.first;
}

template <typename R>
void Sigma<R>::
set_noise(R sigma, R ebn0, R esn0)
{
	Noise<R>::set_noise(sigma); // call check
	_esn0.first = esn0;
	_esn0.second = true;
	_ebn0.first = ebn0;
	_ebn0.second = true;
}

template <typename R>
void Sigma<R>::
check()
{
	auto n = this->get_noise();
	if (n <= (R)0)
	{
		std::stringstream message;
		message << "The SIGMA noise '_n' has to be greater than 0 ('_n' = " << n << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename R>
bool Sigma<R>::is_set() const noexcept
{
	return Noise<R>::is_set() && has_ebn0() && has_esn0();
}

template<typename R>
bool Sigma<R>::has_esn0() const noexcept
{
	return _esn0.second;
}

template<typename R>
bool Sigma<R>::has_ebn0() const noexcept
{
	return _ebn0.second;
}

template<typename R>
Noise_type Sigma<R>::get_type() const
{
	return Noise_type::SIGMA;
}

template<typename R>
Sigma<R>* Sigma<R>::clone() const
{
	return new Sigma(*this);
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Sigma<float>;
template class aff3ct::tools::Sigma<double>;
// ==================================================================================== explicit template instantiation
