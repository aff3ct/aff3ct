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
		message << "This noise type ('" << type_to_str(get_type()) << "') is not same as the given checked one ('"
				<< type_to_str(t) << "').";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}


// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Noise<float>;
template class aff3ct::tools::Noise<double>;

template aff3ct::tools::Noise<double>::Noise(const Noise<float >&);
template aff3ct::tools::Noise<float >::Noise(const Noise<double>&);
// ==================================================================================== explicit template instantiation
