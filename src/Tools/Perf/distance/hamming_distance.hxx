#ifndef HAMMING_DISTANCE_HXX_
#define HAMMING_DISTANCE_HXX_

#include "hamming_distance.h"
#include "distance.h"
#include "Boolean_diff.h"

namespace aff3ct
{
namespace tools
{
template <typename B>
size_t hamming_distance_seq(const B *in1, const B *in2, const unsigned size)
{
	return distance_seq<B,Boolean_diff<B,false>>(in1, in2, size);
}

template <typename B>
size_t hamming_distance(const B *in1, const B *in2, const unsigned size)
{
	return distance<B,Boolean_diff<B,false>>(in1, in2, size);
}

template <typename B>
size_t hamming_distance_seq(const B *in, const unsigned size)
{
	return distance_seq<B,Boolean_diff<B,false>>(in, size);
}

template <typename B>
size_t hamming_distance(const B *in, const unsigned size)
{
	return distance<B,Boolean_diff<B,false>>(in, size);
}




template <typename B>
size_t hamming_distance_unk_seq(const B *in1, const B *in2, const unsigned size)
{
	return distance_seq<B,Boolean_diff<B,true>>(in1, in2, size);
}

template <typename B>
size_t hamming_distance_unk(const B *in1, const B *in2, const unsigned size)
{
	return distance<B,Boolean_diff<B,true>>(in1, in2, size);
}

template <typename B>
size_t hamming_distance_unk_seq(const B *in, const unsigned size)
{
	return distance_seq<B,Boolean_diff<B,true>>(in, size);
}

template <typename B>
size_t hamming_distance_unk(const B *in, const unsigned size)
{
	return distance<B,Boolean_diff<B,true>>(in, size);
}

}
}

#endif