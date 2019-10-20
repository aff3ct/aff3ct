#include "Tools/Algo/Multidimensional_vector/Vector_4D.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
Vector_4D<T>
::Vector_4D(size_t d1, size_t d2, size_t d3, size_t d4,  T const & t)
: d1(d1), d2(d2), d3(d3), d4(d4), d34(d3*d4), d234(d2*d34), data(d1*d234, t)
{
}

template <typename T>
T & Vector_4D<T>
::operator()(size_t i, size_t j, size_t k, size_t l)
{
    return data[i*d234 + j*d34 + k*d4 + l];
}

template <typename T>
T const & Vector_4D<T>
::operator()(size_t i, size_t j, size_t k, size_t l) const
{
    return data[i*d234 + j*d34 + k*d4 + l];
}
}
}
