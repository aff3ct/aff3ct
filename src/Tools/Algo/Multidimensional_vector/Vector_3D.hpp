#ifndef VECTOR_3D_HPP_
#define VECTOR_3D_HPP_

#include <vector>

namespace aff3ct
{
namespace tools
{

template <typename T>
class Vector_3D
{
public:
    explicit Vector_3D(size_t d1=0, size_t d2=0, size_t d3=0, T const & t=T()) :
        d1(d1), d2(d2), d3(d3), d23(d2*d3), data(d1*d23, t)
    {}

    T & operator()(size_t i, size_t j, size_t k)
    {
        return data[i*d23 + j*d3 + k];
    }

    T const & operator()(size_t i, size_t j, size_t k) const
    {
        return data[i*d23 + j*d3 + k];
    }

private:
    const size_t d1, d2, d3;
    const size_t d23;
    std::vector<T> data;
};

}
}

#endif // VECTOR_3D_HPP_