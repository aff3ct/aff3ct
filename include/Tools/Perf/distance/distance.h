#ifndef DISTANCE_H__
#define DISTANCE_H__

#include <cstddef>

namespace aff3ct
{
namespace tools
{
/*
 * \brief compute the distance between the arrays 'in1' and 'in2' of length 'size'
 * \param Fdiffcnt is called each time to compare in1 and in2
 * \return the distance, ie. 0 when arrays are equal
 */
template <typename B, class Fdiffcnt>
inline size_t distance_seq(const B *in1, const B *in2, const unsigned size);

/*
 * \brief compute the distance between the array 'in' of length 'size' and a value defined by Fdiffcnt
 * \param Fdiffcnt is called each time to compare in with its default value (usually 0)
 * \return the distance, ie. 0 when array 'in' satisfy Fdiffcnt
 */
template <typename B, class Fdiffcnt>
inline size_t distance_seq(const B *in, const unsigned size);


/*
 * \brief same than distance_seq but operations are MIPP optimized
 */
template <typename T, class Fdiffcnt>
inline size_t distance(const T *in1, const T *in2, const unsigned size);

/*
 * \brief same than distance_seq but operations are MIPP optimized
 */
template <typename T, class Fdiffcnt>
inline size_t distance(const T *in, const unsigned size);
}
}

#include "Tools/Perf/distance/distance.hxx"

#endif