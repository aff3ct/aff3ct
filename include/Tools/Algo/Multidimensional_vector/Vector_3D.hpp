/*!
 * \file
 * \brief Class tools::Vector_3D.
 */
#ifndef VECTOR_3D_HPP_
#define VECTOR_3D_HPP_

#include <cstddef>
#include <vector>

namespace aff3ct
{
namespace tools
{
template <typename T>
class Vector_3D
{
private:
    const size_t d1, d2, d3, d23;
    std::vector<T> data;

public:
    explicit Vector_3D(size_t d1=0, size_t d2=0, size_t d3=0, T const & t=T());

    T & operator()(size_t i, size_t j, size_t k);

    T const & operator()(size_t i, size_t j, size_t k) const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Multidimensional_vector/Vector_3D.hxx"
#endif

#endif // VECTOR_3D_HPP_