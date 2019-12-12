/*!
 * \file
 * \brief Class tools::Vector_4D.
 */
#ifndef VECTOR_4D_HPP_
#define VECTOR_4D_HPP_

#include <cstddef>
#include <vector>

namespace aff3ct
{
namespace tools
{
template <typename T>
class Vector_4D
{
private:
    const size_t d1, d2, d3, d4, d34, d234;
    std::vector<T> data;

public:
    explicit Vector_4D(size_t d1=0, size_t d2=0, size_t d3=0, size_t d4=0,  T const & t=T());

    T & operator()(size_t i, size_t j, size_t k, size_t l);

    T const & operator()(size_t i, size_t j, size_t k, size_t l) const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Multidimensional_vector/Vector_4D.hxx"
#endif

#endif // VECTOR_4D_HPP_