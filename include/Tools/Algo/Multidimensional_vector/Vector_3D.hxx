#include "Tools/Algo/Multidimensional_vector/Vector_3D.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
Vector_3D<T>
::Vector_3D(size_t d1, size_t d2, size_t d3, T const & t)
: d1(d1), d2(d2), d3(d3), d23(d2*d3), data(d1*d23, t)
{
}

template <typename T>
T & Vector_3D<T>
::operator()(size_t i, size_t j, size_t k)
{
    return data[i*d23 + j*d3 + k];
}

template <typename T>
T const & Vector_3D<T>
::operator()(size_t i, size_t j, size_t k) const
{
    return data[i*d23 + j*d3 + k];
}
}
}
