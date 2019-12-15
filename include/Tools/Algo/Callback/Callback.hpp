/*!
 * \file
 * \brief Class tools::Callback.
 */
#ifndef CALLBACK_HPP__
#define CALLBACK_HPP__

#include <mutex>
#include <vector>
#include <cstdint>
#include <utility>
#include <functional>

namespace aff3ct
{
namespace tools
{
template <class... Args>
class Callback
{
protected:
	std::vector<std::pair<std::function<void(Args...)>, uint32_t>> callbacks;
	mutable std::mutex mtx;

public:
	Callback() = default;
	Callback(const Callback &c);
	virtual ~Callback() = default;

	uint32_t record(std::function<void(Args...)> callback);
	bool unrecord(const uint32_t id);
	void clear();
	void notify(Args ... args);
	size_t size() const;
};
}
}

#include "Tools/Algo/Callback/Callback.hxx"

#endif // CALLBACK_HPP__