/*!
 * \file
 * \brief Class tools::Vector_2D.
 */
#ifndef VECTOR_2D_HPP_
#define VECTOR_2D_HPP_

#include <cstddef>
#include <vector>

namespace aff3ct
{
namespace tools
{
template <typename T>
class Vector_2D
{
private:
    const size_t d1, d2;
    std::vector<T> data;

public:
    explicit Vector_2D(size_t d1=0, size_t d2=0, T const & t=T());

    T & operator()(size_t i, size_t j);

    T const & operator()(size_t i, size_t j) const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Multidimensional_vector/Vector_2D.hxx"
#endif

#endif // VECTOR_2D_HPP_