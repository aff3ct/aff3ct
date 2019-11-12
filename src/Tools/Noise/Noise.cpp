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
	this->notify_noise_changed();
}

template <typename R>
void Noise<R>
::set_value(const R value)
{
	this->value = value;
	this->check();
	this->notify_noise_changed();
}

template <typename R>
void Noise<R>
::notify_noise_changed()
{
	for (auto c : this->callbacks_changed)
		c.first();
}

template <typename R>
R Noise<R>
::get_value() const
{
	if (this->value == std::numeric_limits<R>::infinity())
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
::register_callback_changed(std::function<void()> callback)
{
	uint32_t id = 0;
	if (this->callbacks_changed.size())
		id = this->callbacks_changed[this->callbacks_changed.size() -1].second +1;
	this->callbacks_changed.push_back(std::make_pair(callback, id));
	return id;
}

template<typename R>
bool Noise<R>
::unregister_callback_changed(const uint32_t id)
{
	auto it = this->callbacks_changed.begin();
	while (it != this->callbacks_changed.end())
	{
		if ((*it).second == id)
		{
			it = this->callbacks_changed.erase(it);
			return true;
		}
		else
			++it;
	}
	return false;
}

template<typename R>
void Noise<R>
::clear_callbacks_changed()
{
	this->callbacks_changed.clear();
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Noise<float>;
template class aff3ct::tools::Noise<double>;
// ==================================================================================== explicit template instantiation
