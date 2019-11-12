#include "Tools/Algo/Callback/Callback.hpp"

namespace aff3ct
{
namespace tools
{

template <class... Args>
uint32_t Callback<Args...>
::record(std::function<void(Args...)> callback)
{
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
	this->callbacks.clear();
}

template <class... Args>
void Callback<Args...>
::notify(Args ... args) const
{
	for (auto& c : this->callbacks)
		c.first(args...);
}

}
}
