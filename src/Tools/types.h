#ifndef TYPES_H_
#define TYPES_H_

#include <cstdint>

// type for the bits
//using B_8  = signed char;
//using B_16 = short;
//using B_32 = int;
//using B_64 = long long;
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

#if defined(PREC_8_BIT)
	using B  = B_8;
	using R  = R_8;
	using Q  = Q_8;
#elif defined(PREC_16_BIT)
	using B  = B_16;
	using R  = R_16;
	using Q  = Q_16;
#elif defined(PREC_64_BIT)
	using B  = B_64;
	using R  = R_64;
	using Q  = Q_64;
#else // PREC_32_BIT
	using B  = B_32;
	using R  = R_32;
	using Q  = Q_32;
	#ifndef PREC_32_BIT
	#define PREC_32_BIT
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
