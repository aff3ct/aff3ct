#include "Tools/Algo/Callback/Callback.hpp"

namespace aff3ct
{
namespace tools
{

template <class... Args>
Callback<Args...>
::Callback(const Callback &c)
{
	std::lock_guard<std::mutex> lg(this->mtx);
	this->callbacks = c.callbacks;
}

template <class... Args>
uint32_t Callback<Args...>
::record(std::function<void(Args...)> callback)
{
	std::lock_guard<std::mutex> lg(this->mtx);;
	uint32_t id = 0;
	if (this->callbacks.size())
		id = this->callbacks[this->callbacks.size() -1].second +1;
	this->callbacks.push_back(std::make_pair(callback, id));
	return id;
}

template <class... Args>
bool Callback<Args...>
::unrecord(const uint32_t id)
{
	std::lock_guard<std::mutex> lg(this->mtx);
	auto it = this->callbacks.begin();
	while (it != this->callbacks.end())
	{
		if ((*it).second == id)
		{
			it = this->callbacks.erase(it);
			return true;
		}
		else
			++it;
	}
	return false;
}

template <class... Args>
void Callback<Args...>
::clear()
{
	std::lock_guard<std::mutex> lg(this->mtx);
	this->callbacks.clear();
}

template <class... Args>
void Callback<Args...>
::notify(Args ... args)
{
	std::lock_guard<std::mutex> lg(this->mtx);
	for (auto& c : this->callbacks)
		c.first(args...);
}

template <class... Args>
size_t Callback<Args...>
::size() const
{
	std::lock_guard<std::mutex> lg(this->mtx);
	return this->callbacks.size();
}

}
}
