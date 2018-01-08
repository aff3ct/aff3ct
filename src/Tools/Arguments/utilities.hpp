#ifndef ARGUMENT_UTILITIES_HPP
#define ARGUMENT_UTILITIES_HPP

#include <tuple>

namespace aff3ct
{
namespace tools
{

/******************************************************************************
               TUPLE UTILITIES
 ******************************************************************************/

template<int...> struct index_tuple{};

template<int I, typename IndexTuple, typename... Types>
struct make_indexes_impl;

template<int I, int... Indexes, typename T, typename ... Types>
struct make_indexes_impl<I, index_tuple<Indexes...>, T, Types...>
{
    typedef typename make_indexes_impl<I + 1, index_tuple<Indexes..., I>, Types...>::type type;
};

template<int I, int... Indexes>
struct make_indexes_impl<I, index_tuple<Indexes...> >
{
    typedef index_tuple<Indexes...> type;
};

template<typename ... Types>
struct make_indexes : make_indexes_impl<0, index_tuple<>, Types...>
{};


template<class Ret, class... Args, int... Indexes >
Ret apply_tuple_helper( Ret (*pf)(Args...), index_tuple< Indexes... >, std::tuple<Args...>&& tup)
{
    return pf( std::forward<Args>( std::get<Indexes>(tup))... );
}

template<class Ret, class ... Args>
Ret apply_tuple(Ret (*pf)(Args...), const std::tuple<Args...>&  tup)
{
    return apply_tuple_helper(pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
}

template<class Ret, class ... Args>
Ret apply_tuple(Ret (*pf)(Args...), std::tuple<Args...>&&  tup)
{
    return apply_tuple_helper(pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
}

}
}

#endif /* ARGUMENT_UTILITIES_HPP */