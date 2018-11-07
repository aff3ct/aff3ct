#ifndef VECTOR_4D_HPP_
#define VECTOR_4D_HPP_

#include <vector>

namespace aff3ct
{
namespace tools
{

template <typename T>
class Vector_4D
{
public:
    explicit Vector_4D(size_t d1=0, size_t d2=0, size_t d3=0, size_t d4=0,  T const & t=T()) :
        d1(d1), d2(d2), d3(d3), d4(d4), d34(d3*d4), d234(d2*d34), data(d1*d234, t)
    {}

    T & operator()(size_t i, size_t j, size_t k, size_t l)
    {
        return data[i*d234 + j*d34 + k*d4 + l];
    }

    T const & operator()(size_t i, size_t j, size_t k, size_t l) const
    {
        return data[i*d234 + j*d34 + k*d4 + l];
    }

private:
    const size_t d1, d2, d3, d4;
    const size_t d34, d234;
    std::vector<T> data;
};

}
}

#endif // VECTOR_4D_HPP_