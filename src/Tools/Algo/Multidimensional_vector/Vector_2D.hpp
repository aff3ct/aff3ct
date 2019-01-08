#ifndef VECTOR_2D_HPP_
#define VECTOR_2D_HPP_

#include <vector>

namespace aff3ct
{
namespace tools
{

template <typename T>
class Vector_2D
{
public:
    explicit Vector_2D(size_t d1=0, size_t d2=0, T const & t=T()) :
        d1(d1), d2(d2), data(d1*d2, t)
    {}

    T & operator()(size_t i, size_t j)
    {
        return data[i*d2 + j];
    }

    T const & operator()(size_t i, size_t j) const
    {
        return data[i*d2 + j];
    }

private:
    const size_t d1, d2;
    std::vector<T> data;
};

}
}

#endif // VECTOR_2D_HPP_