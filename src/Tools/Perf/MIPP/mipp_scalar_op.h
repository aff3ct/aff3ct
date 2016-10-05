#ifndef MIPP_SCALAR_OP_H_
#define MIPP_SCALAR_OP_H_

namespace mipp_scop // My Intrinsics Plus Plus Scalar Operations
{
	template <typename T>
	T andb(const T val1, const T val2);

	template <typename T>
	T xorb(const T val1, const T val2);

	template <typename T>
	T div2(const T val);

	template <typename T>
	T div4(const T val);

	template <typename T>
	T rshift(const T val, const int n);

	template <typename T>
	T lshift(const T val, const int n);
}

#endif /* MIPP_SCALAR_OP_H_ */
