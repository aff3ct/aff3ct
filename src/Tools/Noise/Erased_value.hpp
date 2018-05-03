#ifndef ERASED_VALUE_HPP
#define ERASED_VALUE_HPP

#include <type_traits>
#include <limits>

namespace aff3ct
{
namespace tools
{

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, T>::type erased_value_symbol()
{
	return std::numeric_limits<T>::max();
}

template<class T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type erased_value_symbol()
{
	return std::numeric_limits<T>::infinity();
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, T>::type erased_value_llr()
{
	return 0;
}

template<class T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type erased_value_llr()
{
	return (T)0.000001;
}

template<typename Q = float>
struct Erased_value
{
	static constexpr Q symbol = erased_value_symbol<Q>();
	static constexpr Q llr    = erased_value_llr<Q>();
};

}
}
#endif //ERASED_VALUE_HPP
