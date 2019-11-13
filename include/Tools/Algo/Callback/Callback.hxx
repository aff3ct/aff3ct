#include "Tools/Algo/Callback/Callback.hpp"

namespace aff3ct
{
namespace tools
{

template <class... Args>
Callback<Args...>
::Callback(const Callback &c)
{
	c.mtx.lock();
	this->callbacks = c.callbacks;
	c.mtx.unlock();
}

template <class... Args>
uint32_t Callback<Args...>
::record(std::function<void(Args...)> callback)
{
	this->mtx.lock();
	uint32_t id = 0;
	if (this->callbacks.size())
		id = this->callbacks[this->callbacks.size() -1].second +1;
	this->callbacks.push_back(std::make_pair(callback, id));
	this->mtx.unlock();
	return id;
}

template <class... Args>
bool Callback<Args...>
::unrecord(const uint32_t id)
{
	this->mtx.lock();
	auto it = this->callbacks.begin();
	while (it != this->callbacks.end())
	{
		if ((*it).second == id)
		{
			it = this->callbacks.erase(it);
			this->mtx.unlock();
			return true;
		}
		else
			++it;
	}
	this->mtx.unlock();
	return false;
}

template <class... Args>
void Callback<Args...>
::clear()
{
	this->mtx.lock();
	this->callbacks.clear();
	this->mtx.unlock();
}

template <class... Args>
void Callback<Args...>
::notify(Args ... args)
{
	this->mtx.lock();
	for (auto& c : this->callbacks)
		c.first(args...);
	this->mtx.unlock();
}

}
}
