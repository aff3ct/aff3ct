#ifdef MIPP_NO_INTRINSICS

#ifndef MIPP_SCALAR_OP_H_
#define MIPP_SCALAR_OP_H_

template <typename T>
T andb_s(const T val1, const T val2);

template <typename T>
T xorb_s(const T val1, const T val2);

template <typename T>
T div2_s(const T val);

template <typename T>
T div4_s(const T val);

#endif /* MIPP_SCALAR_OP_H_ */

#endif /* MIPP_NO_INTRINSICS */
