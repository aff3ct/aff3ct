#include "Tools/Algo/Multidimensional_vector/Vector_2D.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
Vector_2D<T>
::Vector_2D(size_t d1, size_t d2, T const & t)
: d1(d1), d2(d2), data(d1*d2, t)
{
}

template <typename T>
T& Vector_2D<T>
::operator()(size_t i, size_t j)
{
    return data[i*d2 + j];
}

template <typename T>
T const& Vector_2D<T>
::operator()(size_t i, size_t j) const
{
    return data[i*d2 + j];
}
}
}
