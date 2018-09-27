#ifndef AUTO_CLONED_UNIQUE_PTR_HPP__
#define AUTO_CLONED_UNIQUE_PTR_HPP__

#include <memory>
#include <type_traits>

namespace aff3ct
{
namespace tools
{

// source : https://stackoverflow.com/q/23726228/7219905
template <typename D>
class auto_cloned_unique_ptr
{
public:
	using uniptr = std::unique_ptr<D>; // = D*
	using pointer = typename uniptr::pointer;

private:
    uniptr up;

public:

	// default (1)
	constexpr auto_cloned_unique_ptr() noexcept {}
	// from null pointer (2)
	constexpr auto_cloned_unique_ptr(std::nullptr_t) noexcept : up(nullptr) {}
	// from pointer (3)
	explicit auto_cloned_unique_ptr(pointer p) noexcept : up(p) {}
	// from pointer + lvalue deleter (4)
	auto_cloned_unique_ptr(pointer p, typename std::conditional<std::is_reference<D>::value,D,const D&> del) noexcept : up(p, del) {}
	// from pointer + rvalue deleter (5)
	auto_cloned_unique_ptr(pointer p, typename std::remove_reference<D>::type&& del) noexcept : up(p, del) {}
	// move constructor
	auto_cloned_unique_ptr(std::unique_ptr<D>&& _up) : up(std::move(_up)) {}
	// move constructor
	auto_cloned_unique_ptr(auto_cloned_unique_ptr<D>&& other) : up(std::move(other)) {}
	// copy constructor
	auto_cloned_unique_ptr(auto_cloned_unique_ptr<D> const& other)
	{
		if (other != nullptr)
			up.reset(dynamic_cast<pointer>(other->clone()));
	}

	// copy assignment
	inline auto_cloned_unique_ptr<D>& operator=(auto_cloned_unique_ptr<D> const& other)
	{
		if (other != nullptr)
			up = other.up->clone();
		else
			up.reset();
		return *this;
	}

	// move assigment
	inline auto_cloned_unique_ptr<D>& operator=(auto_cloned_unique_ptr<D>&& other)
	{
		up = std::move(other.up);
		return *this;
	}

	inline auto_cloned_unique_ptr<D>& operator=(pointer p)
	{
		up.reset(p);
		return *this;
	}


	inline const D&      operator *() const {return *up;}
	inline const pointer operator->() const {return up.operator->();}
	inline const pointer get()        const {return up.get();}
	inline void reset(pointer __p = pointer()) noexcept {return up.reset(__p);}

	protected:
	template <typename T>
	friend bool operator==(const auto_cloned_unique_ptr<T>& x, const auto_cloned_unique_ptr<T>& y);
	template <typename T>
	friend bool operator==(const auto_cloned_unique_ptr<T>& x, std::nullptr_t) noexcept;
	template <typename T>
	friend bool operator!=(const auto_cloned_unique_ptr<T>& x, const auto_cloned_unique_ptr<T>& y);
	template <typename T>
	friend bool operator!=(const auto_cloned_unique_ptr<T>& x, std::nullptr_t) noexcept;
};

template <typename D>
inline bool operator==(const auto_cloned_unique_ptr<D>& x, const auto_cloned_unique_ptr<D>& y)
{ return x.up == y.up; }

template <typename D>
inline bool operator==(const auto_cloned_unique_ptr<D>& x, std::nullptr_t) noexcept
{ return x.up == nullptr; }

template <typename D>
inline bool operator!=(const auto_cloned_unique_ptr<D>& x, const auto_cloned_unique_ptr<D>& y)
{ return x.up != y.up; }

template <typename D>
inline bool operator!=(const auto_cloned_unique_ptr<D>& x, std::nullptr_t) noexcept
{ return x.up != nullptr; }

template<typename _Tp,	typename _Up, typename _Ep>
inline bool operator==(std::nullptr_t, const auto_cloned_unique_ptr<_Up>& y)
{ return y == nullptr; }

template<typename _Tp,	typename _Up, typename _Ep>
inline bool operator!=(std::nullptr_t, const auto_cloned_unique_ptr<_Up>& y)
{ return y != nullptr; }

}
}

#endif // AUTO_CLONED_UNIQUE_PTR_HPP__