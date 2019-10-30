/*!
 * \file
 * \brief Class tools::auto_cloned_unique_ptr.
 */
#ifndef AUTO_CLONED_UNIQUE_PTR_HPP__
#define AUTO_CLONED_UNIQUE_PTR_HPP__

#include <memory>
#include <cstddef>
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
	using uniptr = std::unique_ptr<D>;
	using pointer = typename uniptr::pointer;

private:
	uniptr up;

public:
	// default (1)
	constexpr auto_cloned_unique_ptr() noexcept;
	// from null pointer (2)
	constexpr auto_cloned_unique_ptr(std::nullptr_t) noexcept;
	// from pointer (3)
	explicit auto_cloned_unique_ptr(pointer p) noexcept;
	// from pointer + lvalue deleter (4)
	auto_cloned_unique_ptr(pointer p, typename std::conditional<std::is_reference<D>::value,D,const D&> del) noexcept;
	// from pointer + rvalue deleter (5)
	auto_cloned_unique_ptr(pointer p, typename std::remove_reference<D>::type&& del) noexcept;
	// move constructor
	auto_cloned_unique_ptr(std::unique_ptr<D>&& _up);
	// move constructor
	auto_cloned_unique_ptr(auto_cloned_unique_ptr<D>&& other);
	// copy constructor
	auto_cloned_unique_ptr(auto_cloned_unique_ptr<D> const& other);

	// copy assignment
	inline auto_cloned_unique_ptr<D>& operator=(auto_cloned_unique_ptr<D> const& other);
	// move assigment
	inline auto_cloned_unique_ptr<D>& operator=(auto_cloned_unique_ptr<D>&& other);

	inline auto_cloned_unique_ptr<D>& operator=(pointer p);

	inline const D&      operator *() const;
	inline const pointer operator->() const;
	inline const pointer get()        const;
	inline void reset(pointer __p = pointer()) noexcept;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
protected:
	template <typename T>
	friend bool operator==(const auto_cloned_unique_ptr<T>& x, const auto_cloned_unique_ptr<T>& y);
	template <typename T>
	friend bool operator==(const auto_cloned_unique_ptr<T>& x, std::nullptr_t) noexcept;
	template <typename T>
	friend bool operator!=(const auto_cloned_unique_ptr<T>& x, const auto_cloned_unique_ptr<T>& y);
	template <typename T>
	friend bool operator!=(const auto_cloned_unique_ptr<T>& x, std::nullptr_t) noexcept;
#endif
};

template <typename D>
using ac_unique_ptr = auto_cloned_unique_ptr<D>;
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/auto_cloned_unique_ptr.hxx"
#endif

#endif // AUTO_CLONED_UNIQUE_PTR_HPP__