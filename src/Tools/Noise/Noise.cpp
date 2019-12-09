#include <limits>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/Noise.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
Noise<R>
::Noise()
: value(std::numeric_limits<R>::infinity())
{
}

template <typename R>
Noise<R>
::Noise(R value)
: value(value)
{
	this->check();
}

template <typename R>
void Noise<R>
::copy(const Noise<R>& other)
{
	this->value = other.value;
	this->callback_update.notify();
}

template <typename R>
bool Noise<R>
::is_set() const
{
	if (this->value == std::numeric_limits<R>::infinity())
		return false;
	else
		return true;
}

template <typename R>
void Noise<R>
::set_value(const R value)
{
	this->value = value;
	this->check();
	this->callback_update.notify();
}

template <typename R>
R Noise<R>
::get_value() const
{
	if (!this->is_set())
	{
		std::stringstream message;
		message << "'value' is not set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->value;
}

template <typename R>
void Noise<R>
::check() const
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

template<typename R>
uint32_t Noise<R>
::record_callback_update(std::function<void()> callback)
{
	return this->callback_update.record(callback);
}

template<typename R>
bool Noise<R>
::unrecord_callback_update(const uint32_t id)
{
	return this->callback_update.unrecord(id);
}

template<typename R>
void Noise<R>
::clear_callbacks_update()
{
	this->callback_update.clear();
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Noise<float>;
template class aff3ct::tools::Noise<double>;
// ==================================================================================== explicit template instantiation
