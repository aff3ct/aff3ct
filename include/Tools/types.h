/*!
 * \file
 * \brief Description of the main templatized types.
 */
#ifndef TYPES_H_
#define TYPES_H_

#include <cstdint>
#include <cstddef>

// type for the bits
using B_8  = int8_t;
using B_16 = int16_t;
using B_32 = int32_t;
using B_64 = int64_t;

// type for the real numbers (before quantization)
using R_8  = float;
using R_16 = float;
using R_32 = float;
using R_64 = double;

// type for the real numbers (after  quantization)
using Q_8  = signed char;
using Q_16 = short;
using Q_32 = float;
using Q_64 = double;

#if defined(AFF3CT_8BIT_PREC)
	using B  = B_8;
	using R  = R_8;
	using Q  = Q_8;
#elif defined(AFF3CT_16BIT_PREC)
	using B  = B_16;
	using R  = R_16;
	using Q  = Q_16;
#elif defined(AFF3CT_64BIT_PREC)
	using B  = B_64;
	using R  = R_64;
	using Q  = Q_64;
#else // AFF3CT_32BIT_PREC
	using B  = B_32;
	using R  = R_32;
	using Q  = Q_32;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	#ifndef AFF3CT_32BIT_PREC
	#define AFF3CT_32BIT_PREC
	#endif
#endif
#endif

namespace aff3ct
{
namespace tools
{

template<size_t precision> // the wanted precision 8, 16, 32, 64
struct types_from_prec
{
};

template<>
struct types_from_prec<8>
{
	using B = B_8;
	using R = R_8;
	using Q = Q_8;
};

template<>
struct types_from_prec<16>
{
	using B = B_16;
	using R = R_16;
	using Q = Q_16;
};

template<>
struct types_from_prec<32>
{
	using B = B_32;
	using R = R_32;
	using Q = Q_32;
};

template<>
struct types_from_prec<64>
{
	using B = B_64;
	using R = R_64;
	using Q = Q_64;
};

template<typename T> // give a type T and get the B R Q of the same size
struct matching_types : types_from_prec<sizeof(T) * 8>
{
};

}
}

#endif /* TYPES_H_ */
