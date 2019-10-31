#include <algorithm>
#include <utility>

#include "Tools/auto_cloned_unique_ptr.hpp"

namespace aff3ct
{
namespace tools
{
template <typename D>
constexpr auto_cloned_unique_ptr<D>
::auto_cloned_unique_ptr() noexcept
{
}

template <typename D>
constexpr auto_cloned_unique_ptr<D>
::auto_cloned_unique_ptr(std::nullptr_t) noexcept
: up(nullptr)
{
}

template <typename D>
auto_cloned_unique_ptr<D>
::auto_cloned_unique_ptr(pointer p) noexcept
: up(p)
{
}

template <typename D>
auto_cloned_unique_ptr<D>
::auto_cloned_unique_ptr(pointer p, typename std::conditional<std::is_reference<D>::value,D,const D&> del) noexcept
: up(p, del)
{
}

template <typename D>
auto_cloned_unique_ptr<D>
::auto_cloned_unique_ptr(pointer p, typename std::remove_reference<D>::type&& del) noexcept
: up(p, del)
{
}

template <typename D>
auto_cloned_unique_ptr<D>
::auto_cloned_unique_ptr(std::unique_ptr<D>&& _up)
: up(std::move(_up))
{
}

template <typename D>
auto_cloned_unique_ptr<D>
::auto_cloned_unique_ptr(auto_cloned_unique_ptr<D>&& other)
: up(std::move(other))
{
}

template <typename D>
auto_cloned_unique_ptr<D>
::auto_cloned_unique_ptr(auto_cloned_unique_ptr<D> const& other)
{
	if (other != nullptr)
		up.reset(dynamic_cast<pointer>(other->clone()));
}

template <typename D>
auto_cloned_unique_ptr<D>& auto_cloned_unique_ptr<D>
::operator=(auto_cloned_unique_ptr<D> const& other)
{
	if (other != nullptr)
		up = other.up->clone();
	else
		up.reset();
	return *this;
}


template <typename D>
auto_cloned_unique_ptr<D>& auto_cloned_unique_ptr<D>
::operator=(auto_cloned_unique_ptr<D>&& other)
{
	up = std::move(other.up);
	return *this;
}

template <typename D>
auto_cloned_unique_ptr<D>& auto_cloned_unique_ptr<D>
::operator=(pointer p)
{
	up.reset(p);
	return *this;
}

template <typename D>
const D& auto_cloned_unique_ptr<D>
::operator *() const
{
	return *up;
}

template <typename D>
const typename std::unique_ptr<D>::pointer auto_cloned_unique_ptr<D>
::operator->() const
{
	return up.operator->();
}

template <typename D>
const typename std::unique_ptr<D>::pointer auto_cloned_unique_ptr<D>
::get() const
{
	return up.get();
}

template <typename D>
void auto_cloned_unique_ptr<D>
::reset(pointer __p) noexcept
{
	return up.reset(__p);
}

template <typename D>
bool operator==(const auto_cloned_unique_ptr<D>& x, const auto_cloned_unique_ptr<D>& y)
{
	return x.up == y.up;
}

template <typename D>
bool operator==(const auto_cloned_unique_ptr<D>& x, std::nullptr_t) noexcept
{
	return x.up == nullptr;
}

template <typename D>
bool operator!=(const auto_cloned_unique_ptr<D>& x, const auto_cloned_unique_ptr<D>& y)
{
	return x.up != y.up;
}

template <typename D>
bool operator!=(const auto_cloned_unique_ptr<D>& x, std::nullptr_t) noexcept
{
	return x.up != nullptr;
}

template<typename _Tp,	typename _Up, typename _Ep>
bool operator==(std::nullptr_t, const auto_cloned_unique_ptr<_Up>& y)
{
	return y == nullptr;
}

template<typename _Tp,	typename _Up, typename _Ep>
bool operator!=(std::nullptr_t, const auto_cloned_unique_ptr<_Up>& y)
{
	return y != nullptr;
}
}
}
