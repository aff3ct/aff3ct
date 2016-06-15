/*
Copyright (c) 1998, Regents of the University of California
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
* Neither the name of the University of California, Berkeley nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * mipp.h
 *
 *  Created on: 5 oct. 2014
 *      Author: Adrien Cassagne
 */

#ifndef MY_INTRINSICS_PLUS_PLUS_H_
#define MY_INTRINSICS_PLUS_PLUS_H_

#ifndef NO_INTRINSICS
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include <arm_neon.h>
#elif defined(__SSE__) || defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
// header for special functions: log, exp, sin, cos
#if !defined(__INTEL_COMPILER) && !defined(__ICL) && !defined(__ICC)
#if defined(__AVX__)
#include "math/avx_mathfun.h"
#elif defined(__SSE__)
#include "math/sse_mathfun.h"
#endif
#endif
#include <immintrin.h>
#ifdef __SSE__
#include <xmmintrin.h>
#endif
#ifdef __SSE2__
#include <emmintrin.h>
#endif
#ifdef __SSE3__
#include <pmmintrin.h>
#endif
#ifdef __SSSE3__
#include <tmmintrin.h>
#endif
#ifdef __SSE4_1__
#include <smmintrin.h>
#endif
#endif
#endif

#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <cstddef>
#include <cassert>
#include <string>
#include <vector>
#include <cmath>
#include <map>

namespace mipp // My Intrinsics Plus Plus => mipp
{
// ------------------------------------------------------------------------------------------ myIntrinsics vector sizes
// --------------------------------------------------------------------------------------------------------------------
#ifndef NO_INTRINSICS
// ------------------------------------------------------------------------------------------------------- ARM NEON-128
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
	#define REQUIRED_ALIGNMENT 16
	constexpr int RequiredAlignment = REQUIRED_ALIGNMENT;
	constexpr int RegisterSizeBit = 128;

	using reg = float32x4_t;

	const std::string IntructionsType = "ARM NEONv1-128";

// -------------------------------------------------------------------------------------------------------- X86 AVX-512
#elif defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
	#define REQUIRED_ALIGNMENT 64
	constexpr int RequiredAlignment = REQUIRED_ALIGNMENT;
	constexpr int RegisterSizeBit = 512;

	using reg = __m512;

	const std::string IntructionsType = "x86 AVX-512";

// -------------------------------------------------------------------------------------------------------- X86 AVX-256
#elif defined(__AVX__)
	#define REQUIRED_ALIGNMENT 32
	constexpr int RequiredAlignment = REQUIRED_ALIGNMENT;
	constexpr int RegisterSizeBit = 256;

	using reg = __m256;

#ifdef __AVX2__
	const std::string IntructionsType = "x86 AVX2-256";
#else
	const std::string IntructionsType = "x86 AVX1-256";
#endif

// -------------------------------------------------------------------------------------------------------- X86 SSE-128
#elif defined(__SSE__)
	#define REQUIRED_ALIGNMENT 16
	constexpr int RequiredAlignment = REQUIRED_ALIGNMENT;
	constexpr int RegisterSizeBit = 128;

	using reg = __m128;

#ifdef __SSE4_2__
	const std::string IntructionsType = "x86 SSE4.2-128";
#elif defined(__SSE4_1__)
	const std::string IntructionsType = "x86 SSE4.1-128";
#elif defined(__SSSE3__)
	const std::string IntructionsType = "x86 SSSE3-128";
#elif defined(__SSE3__)
	const std::string IntructionsType = "x86 SSE3-128";
#elif defined(__SSE2__)
	const std::string IntructionsType = "x86 SSE2-128";
#else
	const std::string IntructionsType = "x86 SSE1-128";
#endif

// ------------------------------------------------------------------------------------------------------ NO_INTRINSICS
#else
	#define NO_INTRINSICS
	#define REQUIRED_ALIGNMENT 1
	constexpr int RequiredAlignment = REQUIRED_ALIGNMENT;
	constexpr int RegisterSizeBit = 0;

	using reg = int;

	const std::string IntructionsType = "NO INTRINSICS";
#endif

// ------------------------------------------------------------------------------------------------------ NO_INTRINSICS
#else
	#define REQUIRED_ALIGNMENT 1
	constexpr int RequiredAlignment = REQUIRED_ALIGNMENT;
	constexpr int RegisterSizeBit = 0;

	using reg = int;

	const std::string IntructionsType = "NO INTRINSICS";

#endif

typedef struct regx2 { reg val[2]; } regx2;

template <typename T>
constexpr int nElmtsPerRegister()
{
#ifdef NO_INTRINSICS
	return 1;
#else
	return RegisterSizeBit / (8 * sizeof(T));
#endif
}

template <typename T>
constexpr int nElReg() { return mipp::nElmtsPerRegister<T>(); }

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------- memory allocator
template <typename T>
T* malloc(int nData)
{
	T* ptr = nullptr;

#if defined(__SSE2__) || defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
	ptr = (T*)_mm_malloc(nData * sizeof(T), mipp::RequiredAlignment);
#else
	ptr = new T[nData];
#endif

	return ptr;
}

template <typename T>
void free(T* ptr)
{
#if defined(__SSE2__) || defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
	_mm_free(ptr);
#else
	delete[] ptr;
#endif
}

template <class T>
struct AlignedAllocator
{
	typedef T value_type;
	AlignedAllocator() { }
	template <class C> AlignedAllocator(const AlignedAllocator<C>& other) { }
	T* allocate(std::size_t n) { return mipp::malloc<T>((int)n); }
	void deallocate(T* p, std::size_t n) { mipp::free<T>(p); }
};

// returns true if and only if storage allocated from ma1 can be deallocated from ma2, and vice versa.
// always returns true for stateless allocators.
template <class C1, class C2>
bool operator==(const AlignedAllocator<C1>& ma1, const AlignedAllocator<C2>& ma2) { return true; }

template <class C1, class C2>
bool operator!=(const AlignedAllocator<C1>& ma1, const AlignedAllocator<C2>& ma2) { return !(ma1 == ma2); }

// override vector type
template<class T> using vector = std::vector<T, AlignedAllocator<T>>;

// --------------------------------------------------------------------------------------------------- memory allocator
// --------------------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------- myIntrinsics prototypes
// --------------------------------------------------------------------------------------------------------------------

template <typename T>
static void errorMessage(std::string instr)
{
	// define type names
	std::unordered_map<std::type_index,std::string> type_names;
	type_names[typeid(char)]                   = "char";
	type_names[typeid(unsigned char)]          = "unsigned char";
	type_names[typeid(signed char)]            = "signed char";
	type_names[typeid(short)]                  = "short";
	type_names[typeid(short int)]              = "short int";
	type_names[typeid(unsigned short)]         = "unsigned short";
	type_names[typeid(unsigned short int)]     = "unsigned short int";
	type_names[typeid(signed short)]           = "signed short";
	type_names[typeid(signed short int)]       = "signed short int";
	type_names[typeid(int)]                    = "int";
	type_names[typeid(unsigned)]               = "unsigned";
	type_names[typeid(unsigned int)]           = "unsigned int";
	type_names[typeid(signed int)]             = "signed int";
	type_names[typeid(long)]                   = "long";
	type_names[typeid(long int)]               = "long int";
	type_names[typeid(unsigned long)]          = "unsigned long";
	type_names[typeid(unsigned long int)]      = "unsigned long int";
	type_names[typeid(signed long)]            = "signed long";
	type_names[typeid(signed long int)]        = "signed long int";
	type_names[typeid(long long)]              = "long long";
	type_names[typeid(long long int)]          = "long long int";
	type_names[typeid(unsigned long long)]     = "unsigned long long";
	type_names[typeid(unsigned long long int)] = "unsigned long long int";
	type_names[typeid(signed long long)]       = "signed long long";
	type_names[typeid(signed long long int)]   = "signed long long int";
	type_names[typeid(float)]                  = "float";
	type_names[typeid(double)]                 = "double";

	if (RegisterSizeBit == 0)
		std::cerr << "Undefined type of instructions, try to add -mfpu=neon, -msse4.2, -mavx, -march=native... "
		          << "at the compile time." << std::endl;
	std::cerr << "mipp::" << instr << "<" << type_names[typeid(T)] << "> (" << IntructionsType << ") is undefined! "
	          << "Program halting..." << std::endl;
}

template <typename T1, typename T2>
static void errorMessage(std::string instr)
{
	// define type names
	std::unordered_map<std::type_index,std::string> type_names;
	type_names[typeid(char)]                   = "char";
	type_names[typeid(unsigned char)]          = "unsigned char";
	type_names[typeid(signed char)]            = "signed char";
	type_names[typeid(short)]                  = "short";
	type_names[typeid(short int)]              = "short int";
	type_names[typeid(unsigned short)]         = "unsigned short";
	type_names[typeid(unsigned short int)]     = "unsigned short int";
	type_names[typeid(signed short)]           = "signed short";
	type_names[typeid(signed short int)]       = "signed short int";
	type_names[typeid(int)]                    = "int";
	type_names[typeid(unsigned)]               = "unsigned";
	type_names[typeid(unsigned int)]           = "unsigned int";
	type_names[typeid(signed int)]             = "signed int";
	type_names[typeid(long)]                   = "long";
	type_names[typeid(long int)]               = "long int";
	type_names[typeid(unsigned long)]          = "unsigned long";
	type_names[typeid(unsigned long int)]      = "unsigned long int";
	type_names[typeid(signed long)]            = "signed long";
	type_names[typeid(signed long int)]        = "signed long int";
	type_names[typeid(long long)]              = "long long";
	type_names[typeid(long long int)]          = "long long int";
	type_names[typeid(unsigned long long)]     = "unsigned long long";
	type_names[typeid(unsigned long long int)] = "unsigned long long int";
	type_names[typeid(signed long long)]       = "signed long long";
	type_names[typeid(signed long long int)]   = "signed long long int";
	type_names[typeid(float)]                  = "float";
	type_names[typeid(double)]                 = "double";

	if (RegisterSizeBit == 0)
		std::cerr << "Undefined type of instructions, try to add -mfpu=neon, -msse4.2, -mavx, -march=native... "
		          << "at the compile time." << std::endl;
	std::cerr << "mipp::" << instr << "<" << type_names[typeid(T1)] << "," << type_names[typeid(T2)] << "> (" 
	          << IntructionsType << ") is undefined! Program halting..." << std::endl;
}

template <typename T> inline reg   load         (const T*)                        { errorMessage<T>("load");          exit(-1); }
template <typename T> inline reg   loadu        (const T*)                        { errorMessage<T>("loadu");         exit(-1); }
template <typename T> inline void  store        (T*, const reg)                   { errorMessage<T>("store");         exit(-1); }
template <typename T> inline void  storeu       (T*, const reg)                   { errorMessage<T>("storeu");        exit(-1); }
template <typename T> inline reg   set          (const T[nElReg<T>()])            { errorMessage<T>("set");           exit(-1); }
template <typename T> inline reg   set1         (const T)                         { errorMessage<T>("set1");          exit(-1); }
template <typename T> inline reg   set0         ()                                { errorMessage<T>("set0");          exit(-1); }
template <typename T> inline reg   cmask        (const int[nElReg<T>()])          { errorMessage<T>("cmask");         exit(-1); }
template <typename T> inline reg   cmask2       (const int[nElReg<T>()/2])        { errorMessage<T>("cmask2");        exit(-1); }
template <typename T> inline reg   shuff        (const reg, const reg)            { errorMessage<T>("shuff");         exit(-1); }
template <typename T> inline reg   shuff2       (const reg, const reg)            { errorMessage<T>("shuff2");        exit(-1); }
template <typename T> inline reg   interleavelo (const reg, const reg)            { errorMessage<T>("interleavelo");  exit(-1); }
template <typename T> inline reg   interleavehi (const reg, const reg)            { errorMessage<T>("interleavehi");  exit(-1); }
template <typename T> inline reg   interleavelo2(const reg, const reg)            { errorMessage<T>("interleavelo2"); exit(-1); }
template <typename T> inline reg   interleavehi2(const reg, const reg)            { errorMessage<T>("interleavehi2"); exit(-1); }
template <typename T> inline regx2 interleave   (const reg, const reg)            { errorMessage<T>("interleave");    exit(-1); }
template <typename T> inline regx2 interleave2  (const reg, const reg)            { errorMessage<T>("interleave2");   exit(-1); }
template <typename T> inline reg   interleave   (const reg)                       { errorMessage<T>("interleave");    exit(-1); }
template <typename T> inline regx2 interleavex2 (const reg, const reg)            { errorMessage<T>("interleavex2");  exit(-1); }
template <typename T> inline reg   interleavex4 (const reg)                       { errorMessage<T>("interleavex4");  exit(-1); }
template <typename T> inline reg   interleavex16(const reg)                       { errorMessage<T>("interleavex16"); exit(-1); }
template <typename T> inline void  transpose    (      reg[nElReg<T>()])          { errorMessage<T>("transpose");     exit(-1); }
template <typename T> inline void  transpose8x8 (      reg[8])                    { errorMessage<T>("transpose8x8");  exit(-1); }
template <typename T> inline void  transpose2   (      reg[nElReg<T>()/2])        { errorMessage<T>("transpose2");    exit(-1); }
template <typename T> inline void  transpose28x8(      reg[8])                    { errorMessage<T>("transpose28x8"); exit(-1); }
template <typename T> inline reg   andb         (const reg, const reg)            { errorMessage<T>("andb");          exit(-1); }
template <typename T> inline reg   andnb        (const reg, const reg)            { errorMessage<T>("andnb");         exit(-1); }
template <typename T> inline reg   notb         (const reg)                       { errorMessage<T>("notb");          exit(-1); }
template <typename T> inline reg   orb          (const reg, const reg)            { errorMessage<T>("orb");           exit(-1); }
template <typename T> inline reg   xorb         (const reg, const reg)            { errorMessage<T>("xorb");          exit(-1); }
template <typename T> inline reg   lshift       (const reg, const int)            { errorMessage<T>("lshift");        exit(-1); }
template <typename T> inline reg   rshift       (const reg, const int)            { errorMessage<T>("rshift");        exit(-1); }
template <typename T> inline reg   cmpeq        (const reg, const reg)            { errorMessage<T>("cmpeq");         exit(-1); }
template <typename T> inline reg   cmpneq       (const reg, const reg)            { errorMessage<T>("cmpneq");        exit(-1); }
template <typename T> inline reg   cmplt        (const reg, const reg)            { errorMessage<T>("cmplt");         exit(-1); }
template <typename T> inline reg   cmple        (const reg, const reg)            { errorMessage<T>("cmple");         exit(-1); }
template <typename T> inline reg   cmpgt        (const reg, const reg)            { errorMessage<T>("cmpgt");         exit(-1); }
template <typename T> inline reg   cmpge        (const reg, const reg)            { errorMessage<T>("cmpge");         exit(-1); }
template <typename T> inline reg   add          (const reg, const reg)            { errorMessage<T>("add");           exit(-1); }
template <typename T> inline reg   sub          (const reg, const reg)            { errorMessage<T>("sub");           exit(-1); }
template <typename T> inline reg   mul          (const reg, const reg)            { errorMessage<T>("mul");           exit(-1); }
template <typename T> inline reg   div          (const reg, const reg)            { errorMessage<T>("div");           exit(-1); }
template <typename T> inline reg   min          (const reg, const reg)            { errorMessage<T>("min");           exit(-1); }
template <typename T> inline reg   max          (const reg, const reg)            { errorMessage<T>("max");           exit(-1); }
template <typename T> inline reg   sign         (const reg)                       { errorMessage<T>("sign");          exit(-1); }
template <typename T> inline reg   sign         (const reg, const reg)            { errorMessage<T>("sign");          exit(-1); }
template <typename T> inline reg   neg          (const reg, const reg)            { errorMessage<T>("neg");           exit(-1); }
template <typename T> inline reg   abs          (const reg)                       { errorMessage<T>("abs");           exit(-1); }
template <typename T> inline reg   sqrt         (const reg)                       { errorMessage<T>("sqrt");          exit(-1); }
template <typename T> inline reg   rsqrt        (const reg)                       { errorMessage<T>("rsqrt");         exit(-1); }
template <typename T> inline reg   exp          (const reg)                       { errorMessage<T>("exp");           exit(-1); }
template <typename T> inline reg   fmadd        (const reg, const reg, const reg) { errorMessage<T>("fmadd");         exit(-1); }
template <typename T> inline reg   fnmadd       (const reg, const reg, const reg) { errorMessage<T>("fnmadd");        exit(-1); }
template <typename T> inline reg   fmsub        (const reg, const reg, const reg) { errorMessage<T>("fmsub");         exit(-1); }
template <typename T> inline reg   rot          (const reg)                       { errorMessage<T>("rot");           exit(-1); }
template <typename T> inline reg   rotr         (const reg)                       { errorMessage<T>("rotr");          exit(-1); }
template <typename T> inline reg   div2         (const reg)                       { errorMessage<T>("div2");          exit(-1); }
template <typename T> inline reg   div4         (const reg)                       { errorMessage<T>("div4");          exit(-1); }
template <typename T> inline reg   sat          (const reg, T, T)                 { errorMessage<T>("sat");           exit(-1); }
template <typename T> inline reg   round        (const reg)                       { errorMessage<T>("round");         exit(-1); }

template <typename T1, typename T2> 
inline reg cvt(const reg) {
	errorMessage<T1,T2>("cvt");  
	exit(-1); 
}

template <typename T1, typename T2>
inline reg pack(const reg, const reg) {
	errorMessage<T1,T2>("pack");  
	exit(-1); 
}

// --------------------------------------------------------------------------------------- myIntrinsics implementations
// --------------------------------------------------------------------------------------------------------------------

template <typename T>
void dump(const mipp::reg r, std::ostream &stream = std::cout, const int elmtWidth = 6)
{
	T dumpArray[mipp::nElReg<T>()];
	mipp::storeu<T>(dumpArray, r);

	stream << "[";
	if (typeid(char) == typeid(T) || typeid(signed char) == typeid(T))
		for (auto i = 0; i < mipp::nElReg<T>(); i++)
			stream << std::setw(elmtWidth) << (int)dumpArray[i] << ((i < mipp::nElReg<T>()-1) ? ", " : "");
	else if (typeid(unsigned char) == typeid(T))
		for (auto i = 0; i < mipp::nElReg<T>(); i++)
			stream << std::setw(elmtWidth) << (unsigned)dumpArray[i] << ((i < mipp::nElReg<T>()-1) ? ", " : "");
	else
		for (auto i = 0; i < mipp::nElReg<T>(); i++)
			stream << std::setw(elmtWidth) << dumpArray[i] << ((i < mipp::nElReg<T>()-1) ? ", " : "");

	stream << "]";
}

// ---------------------------------------------------------------------------------------------------------- reduction

template <typename T>
class Reg;

template <typename T>
using red_op = reg (*)(const reg, const reg);

template <typename T>
using Red_op = Reg<T> (*)(const Reg<T>, const Reg<T>);

template <typename T>
using ld_op = reg (*)(const T*);

template <typename T, red_op<T> OP>
struct _reduction
{
	static reg apply(const reg) {
		errorMessage<T>("_reduction::apply");
		exit(-1); 
	}
};

template <typename T, Red_op<T> OP>
struct _Reduction
{
	static Reg<T> apply(const Reg<T> r) {
#ifndef NO_INTRINSICS
		errorMessage<T>("_Reduction::apply");
		exit(-1);
#else
		return r;
#endif
	}
};

template <typename T, red_op<T> OP>
struct reduction
{
	static reg apply(const reg r) 
	{
		return _reduction<T,OP>::apply(r);
	}

	template <ld_op<T> LD = mipp::load<T>>
	static T apply(const mipp::vector<T> &data)
	{
		return reduction<T,OP>::template apply<LD>(data.data(), data.size());
	}

	template <ld_op<T> LD = mipp::loadu<T>>
	static T apply(const std::vector<T> &data)
	{
		return reduction<T,OP>::template apply<LD>(data.data(), data.size());
	}

	template <ld_op<T> LD = mipp::loadu<T>>
	static T apply(const T *data, const int dataSize) 
	{
		assert(dataSize > 0);
		assert(dataSize % mipp::nElReg<T>() == 0);

		auto rRed = LD(&data[0]);
		for (auto i = mipp::nElReg<T>(); i < dataSize; i += mipp::nElReg<T>())
			rRed = OP(rRed, LD(&data[i]));
		rRed = reduction<T,OP>::apply(rRed);

		T tRed[mipp::nElReg<T>()];
		mipp::store<T>(tRed, rRed);

		return tRed[0];
	}
};

template <typename T, Red_op<T> OP>
struct Reduction
{
	static Reg<T> apply(const Reg<T> r) 
	{
		return _Reduction<T,OP>::apply(r);
	}

	template <ld_op<T> LD = mipp::load<T>>
	static T apply(const mipp::vector<T> &data) 
	{
		return Reduction<T,OP>::template apply<LD>(data.data(), data.size());
	}

	template <ld_op<T> LD = mipp::loadu<T>>
	static T apply(const std::vector<T> &data)
	{
		return Reduction<T,OP>::template apply<LD>(data.data(), data.size());
	}

	template <ld_op<T> LD = mipp::loadu<T>>
	static T apply(const T *data, const int dataSize) 
	{
		assert(dataSize > 0);
		assert(dataSize % mipp::nElReg<T>() == 0);

#ifndef NO_INTRINSICS
		auto rRed = Reg<T>(LD(&data[0]));
#else
		auto rRed = Reg<T>(data[0]);
#endif
		for (auto i = mipp::nElReg<T>(); i < dataSize; i += mipp::nElReg<T>())
#ifndef NO_INTRINSICS
			rRed = OP(rRed, Reg<T>(LD(&data[i])));
#else
			rRed = OP(rRed, Reg<T>(data[i]));
#endif
		rRed = Reduction<T,OP>::apply(rRed);

		T tRed[mipp::nElReg<T>()];
		rRed.store(tRed);

		return tRed[0];
	}
};

// ------------------------------------------------------------------------- special reduction functions implementation

template <typename T> inline reg sum (const reg v) { return reduction<T,mipp::add<T>>::apply(v); }
template <typename T> inline reg hadd(const reg v) { return reduction<T,mipp::add<T>>::apply(v); }
template <typename T> inline reg hsub(const reg v) { return reduction<T,mipp::sub<T>>::apply(v); }
template <typename T> inline reg hmul(const reg v) { return reduction<T,mipp::mul<T>>::apply(v); }
template <typename T> inline reg hdiv(const reg v) { return reduction<T,mipp::div<T>>::apply(v); }
template <typename T> inline reg hmin(const reg v) { return reduction<T,mipp::min<T>>::apply(v); }
template <typename T> inline reg hmax(const reg v) { return reduction<T,mipp::max<T>>::apply(v); }

// ------------------------------------------------------------------------------------------------- wrapper to objects

template <typename T>
class Regx2;

#ifndef NO_INTRINSICS

template <typename T> inline Reg<T> add(const Reg<T> v1, const Reg<T> v2);
template <typename T> inline Reg<T> sub(const Reg<T> v1, const Reg<T> v2);
template <typename T> inline Reg<T> mul(const Reg<T> v1, const Reg<T> v2);
template <typename T> inline Reg<T> div(const Reg<T> v1, const Reg<T> v2);
template <typename T> inline Reg<T> min(const Reg<T> v1, const Reg<T> v2);
template <typename T> inline Reg<T> max(const Reg<T> v1, const Reg<T> v2);

template <typename T>
class Reg
{
public:
	reg r;

	Reg(                              )                           {}
	Reg(const reg r                   ) : r(r)                    {}
	Reg(const T   val                 ) : r(mipp::set1 <T>(val))  {}
	Reg(const T  *data                ) : r(mipp::load <T>(data)) {}
	Reg(const T  *data, bool unaligned) : r(mipp::loadu<T>(data)) {}

	~Reg() {}

	static inline Reg<T> cmask (const int mask[nElReg<T>()  ]) { return mipp::cmask <T>(mask); }
	static inline Reg<T> cmask2(const int mask[nElReg<T>()/2]) { return mipp::cmask2<T>(mask); }

	static inline void transpose(Reg<T> regs[nElReg<T>()])
	{
		reg rs[nElReg<T>()];
		for (auto i = 0; i < nElReg<T>(); i++) rs[i] = regs[i].r;
		mipp::transpose<T>(rs);
		for (auto i = 0; i < nElReg<T>(); i++) regs[i].r = rs[i];
	}

	static inline void transpose8x8(Reg<T> regs[8])
	{
		reg rs[8];
		for (auto i = 0; i < 8; i++) rs[i] = regs[i].r;
		mipp::transpose8x8<T>(rs);
		for (auto i = 0; i < 8; i++) regs[i].r = rs[i];
	}

	static inline void transpose2(Reg<T> regs[nElReg<T>()/2])
	{
		reg rs[nElReg<T>()/2];
		for (auto i = 0; i < nElReg<T>()/2; i++) rs[i] = regs[i].r;
		mipp::transpose2<T>(rs);
		for (auto i = 0; i < nElReg<T>()/2; i++) regs[i].r = rs[i];
	}

	static inline void transpose28x8(Reg<T> regs[8])
	{
		reg rs[8];
		for (auto i = 0; i < 8; i++) rs[i] = regs[i].r;
		mipp::transpose28x8<T>(rs);
		for (auto i = 0; i < 8; i++) regs[i].r = rs[i];
	}

	inline void     set0         (               )                        { r = mipp::set0<T>();                          }
	inline void     set1         (const T val    )                        { r = mipp::set1<T>(val);                       }
	inline void     load         (const T* data  )                        { r = mipp::load<T>(data);                      }
	inline void     loadu        (const T* data  )                        { r = mipp::loadu<T>(data);                     }
	inline void     store        (T* data        )                  const { mipp::store<T>(data, r);                      }
	inline void     storeu       (T* data        )                  const { mipp::storeu<T>(data, r);                     }

	inline Reg<T>   shuff        (const Reg<T> v_shu)               const { return mipp::shuff        <T>(r, v_shu.r);    }
	inline Reg<T>   shuff2       (const Reg<T> v_shu)               const { return mipp::shuff2       <T>(r, v_shu.r);    }
	inline Reg<T>   interleavelo (const Reg<T> v)                   const { return mipp::interleavelo <T>(r, v.r);        }
	inline Reg<T>   interleavehi (const Reg<T> v)                   const { return mipp::interleavehi <T>(r, v.r);        }
	inline Reg<T>   interleavelo2(const Reg<T> v)                   const { return mipp::interleavelo2<T>(r, v.r);        }
	inline Reg<T>   interleavehi2(const Reg<T> v)                   const { return mipp::interleavehi2<T>(r, v.r);        }
	inline Regx2<T> interleave   (const Reg<T> v)                   const { return mipp::interleave   <T>(r, v.r);        }
	inline Regx2<T> interleave2  (const Reg<T> v)                   const { return mipp::interleave2  <T>(r, v.r);        }
	inline Reg<T>   interleave   ()                                 const { return mipp::interleave   <T>(r);             }
	inline Regx2<T> interleavex2 (const Reg<T> v)                   const { return mipp::interleavex2 <T>(r, v.r);        }
	inline Reg<T>   interleavex4 ()                                 const { return mipp::interleavex4 <T>(r);             }
	inline Reg<T>   interleavex16()                                 const { return mipp::interleavex16<T>(r);             }
	inline Reg<T>   andb         (const Reg<T> v)                   const { return mipp::andb         <T>(r, v.r);        }
	inline Reg<T>   andnb        (const Reg<T> v)                   const { return mipp::andnb        <T>(r, v.r);        }
	inline Reg<T>   notb         ()                                 const { return mipp::notb         <T>(r);             }
	inline Reg<T>   orb          (const Reg<T> v)                   const { return mipp::orb          <T>(r, v.r);        }
	inline Reg<T>   xorb         (const Reg<T> v)                   const { return mipp::xorb         <T>(r, v.r);        }
	inline Reg<T>   lshift       (const int n)                      const { return mipp::lshift       <T>(r, n);          }
	inline Reg<T>   rshift       (const int n)                      const { return mipp::rshift       <T>(r, n);          }
	inline Reg<T>   cmpeq        (const Reg<T> v)                   const { return mipp::cmpeq        <T>(r, v.r);        }
	inline Reg<T>   cmpneq       (const Reg<T> v)                   const { return mipp::cmpneq       <T>(r, v.r);        }
	inline Reg<T>   cmplt        (const Reg<T> v)                   const { return mipp::cmplt        <T>(r, v.r);        }
	inline Reg<T>   cmple        (const Reg<T> v)                   const { return mipp::cmple        <T>(r, v.r);        }
	inline Reg<T>   cmpgt        (const Reg<T> v)                   const { return mipp::cmpgt        <T>(r, v.r);        }
	inline Reg<T>   cmpge        (const Reg<T> v)                   const { return mipp::cmpge        <T>(r, v.r);        }
	inline Reg<T>   add          (const Reg<T> v)                   const { return mipp::add          <T>(r, v.r);        }
	inline Reg<T>   sub          (const Reg<T> v)                   const { return mipp::sub          <T>(r, v.r);        }
	inline Reg<T>   mul          (const Reg<T> v)                   const { return mipp::mul          <T>(r, v.r);        }
	inline Reg<T>   div          (const Reg<T> v)                   const { return mipp::div          <T>(r, v.r);        }
	inline Reg<T>   min          (const Reg<T> v)                   const { return mipp::min          <T>(r, v.r);        }
	inline Reg<T>   max          (const Reg<T> v)                   const { return mipp::max          <T>(r, v.r);        }
	inline Reg<T>   sign         ()                                 const { return mipp::sign         <T>(r);             }
	inline Reg<T>   sign         (const Reg<T> v)                   const { return mipp::sign         <T>(r, v.r);        }
	inline Reg<T>   neg          (const Reg<T> v)                   const { return mipp::neg          <T>(r, v.r);        }
	inline Reg<T>   abs          ()                                 const { return mipp::abs          <T>(r);             }
	inline Reg<T>   sqrt         ()                                 const { return mipp::sqrt         <T>(r);             }
	inline Reg<T>   rsqrt        ()                                 const { return mipp::rsqrt        <T>(r);             }
	inline Reg<T>   exp          ()                                 const { return mipp::exp          <T>(r);             }
	inline Reg<T>   fmadd        (const Reg<T> v1, const Reg<T> v2) const { return mipp::fmadd        <T>(r, v1.r, v2.r); }
	inline Reg<T>   fnmadd       (const Reg<T> v1, const Reg<T> v2) const { return mipp::fnmadd       <T>(r, v1.r, v2.r); }
	inline Reg<T>   fmsub        (const Reg<T> v1, const Reg<T> v2) const { return mipp::fmsub        <T>(r, v1.r, v2.r); }
	inline Reg<T>   rot          ()                                 const { return mipp::rot          <T>(r);             }
	inline Reg<T>   rotr         ()                                 const { return mipp::rotr         <T>(r);             }
	inline Reg<T>   div2         ()                                 const { return mipp::div2         <T>(r);             }
	inline Reg<T>   div4         ()                                 const { return mipp::div4         <T>(r);             }
	inline Reg<T>   sat          (T min, T max)                     const { return mipp::sat          <T>(r, min, max);   }
	inline Reg<T>   round        ()                                 const { return mipp::round        <T>(r);             }

	template <typename T2> inline Reg<T2> cvt ()               const { return mipp::cvt<T,T2>(r);       }
	template <typename T2> inline Reg<T2> pack(const Reg<T> v) const { return mipp::pack<T,T2>(r, v.r); }

	inline Reg<T>  operator~  (               )       { auto r_new = mipp::notb<T>(r);           return r_new; }

	inline Reg<T>& operator+= (const Reg<T> &v)       { r   = mipp::add<T>(r, v.r);              return *this; }
	inline Reg<T>& operator+= (const T*   data)       { r   = mipp::add<T>(r, Reg<T>(data).r);   return *this; }
	inline Reg<T>  operator+  (      Reg<T>  v) const { v.r = mipp::add<T>(r, v.r);              return v;     }
	inline Reg<T>  operator+  (      T*   data) const { auto v = Reg<T>(data); v = (*this) + v;  return v;     }

	inline Reg<T>& operator-= (const Reg<T> &v)       { r   = mipp::sub<T>(r, v.r);              return *this; }
	inline Reg<T>& operator-= (const T*   data)       { r   = mipp::sub<T>(r, Reg<T>(data).r);   return *this; }
	inline Reg<T>  operator-  (      Reg<T>  v) const { v.r = mipp::sub<T>(r, v.r);              return v;     }
	inline Reg<T>  operator-  (      T*   data) const { auto v = Reg<T>(data); v = (*this) - v;  return v;     }

	inline Reg<T>& operator*= (const Reg<T> &v)       { r   = mipp::mul<T>(r, v.r);              return *this; }
	inline Reg<T>& operator*= (const T*   data)       { r   = mipp::mul<T>(r, Reg<T>(data).r);   return *this; }
	inline Reg<T>  operator*  (      Reg<T>  v) const { v.r = mipp::mul<T>(r, v.r);              return v;     }
	inline Reg<T>  operator*  (      T*   data) const { auto v = Reg<T>(data); v = (*this) * v;  return v;     }

	inline Reg<T>& operator/= (const Reg<T> &v)       { r   = mipp::div<T>(r, v.r);              return *this; }
	inline Reg<T>& operator/= (const T*   data)       { r   = mipp::div<T>(r, Reg<T>(data).r);   return *this; }
	inline Reg<T>  operator/  (      Reg<T>  v) const { v.r = mipp::div<T>(r, v.r);              return v;     }
	inline Reg<T>  operator/  (      T*   data) const { auto v = Reg<T>(data); v = (*this) / v;  return v;     }

	inline Reg<T>& operator^= (const Reg<T> &v)       { r   = mipp::xorb<T>(r, v.r);             return *this; }
	inline Reg<T>& operator^= (const T*   data)       { r   = mipp::xorb<T>(r, Reg<T>(data).r);  return *this; }
	inline Reg<T>  operator^  (      Reg<T>  v) const { v.r = mipp::xorb<T>(r, v.r);             return v;     }
	inline Reg<T>  operator^  (      T*   data) const { auto v = Reg<T>(data); v = (*this) ^ v;  return v;     }

	inline Reg<T>& operator|= (const Reg<T> &v)       { r   = mipp::orb<T>(r, v.r);              return *this; }
	inline Reg<T>& operator|= (const T*   data)       { r   = mipp::orb<T>(r, Reg<T>(data).r);   return *this; }
	inline Reg<T>  operator|  (      Reg<T>  v) const { v.r = mipp::orb<T>(r, v.r);              return v;     }
	inline Reg<T>  operator|  (      T*   data) const { auto v = Reg<T>(data); v = (*this) | v;  return v;     }

	inline Reg<T>& operator&= (const Reg<T> &v)       { r   = mipp::andb<T>(r, v.r);             return *this; }
	inline Reg<T>& operator&= (const T*   data)       { r   = mipp::andb<T>(r, Reg<T>(data).r);  return *this; }
	inline Reg<T>  operator&  (      Reg<T>  v) const { v.r = mipp::andb<T>(r, v.r);             return v;     }
	inline Reg<T>  operator&  (      T*   data) const { auto v = Reg<T>(data); v = (*this) & v;  return v;     }

	inline Reg<T>& operator<<=(const int n    )       { r = mipp::lshift<T>(r, n);               return *this; }
	inline Reg<T>  operator<< (const int n    ) const { Reg v = *this; v <<= n;                  return v;     }

	inline Reg<T>& operator>>=(const int n    )       { r = mipp::rshift<T>(r, n);               return *this; }
	inline Reg<T>  operator>> (const int n    ) const { Reg v = *this; v >>= n;                  return v;     }

	inline Reg<T>  operator== (      Reg<T>  v) const { v = mipp::cmpeq<T>(r, v.r);              return v;     }
	inline Reg<T>  operator== (      T*   data) const { auto v = Reg<T>(data); v = (*this) == v; return v;     }

	inline Reg<T>  operator!= (      Reg<T>  v) const { v = mipp::cmpneq<T>(r, v.r);             return v;     }
	inline Reg<T>  operator!= (      T*   data) const { auto v = Reg<T>(data); v = (*this) != v; return v;     }

	inline Reg<T>  operator<  (      Reg<T>  v) const { v = mipp::cmplt<T>(r, v.r);              return v;     }
	inline Reg<T>  operator<  (      T*   data) const { auto v = Reg<T>(data); v = (*this) <  v; return v;     }

	inline Reg<T>  operator<= (      Reg<T>  v) const { v = mipp::cmple<T>(r, v.r);              return v;     }
	inline Reg<T>  operator<= (      T*   data) const { auto v = Reg<T>(data); v = (*this) <= v; return v;     }

	inline Reg<T>  operator>  (      Reg<T>  v) const { v = mipp::cmpgt<T>(r, v.r);              return v;     }
	inline Reg<T>  operator>  (      T*   data) const { auto v = Reg<T>(data); v = (*this) >  v; return v;     }

	inline Reg<T>  operator>= (      Reg<T>  v) const { v = mipp::cmpge<T>(r, v.r);              return v;     }
	inline Reg<T>  operator>= (      T*   data) const { auto v = Reg<T>(data); v = (*this) >= v; return v;     }

	// ------------------------------------------------------------------------------------------------------ reduction
	inline Reg<T> sum () const { return Reduction<T,mipp::add>::apply(*this); }
	inline Reg<T> hadd() const { return Reduction<T,mipp::add>::apply(*this); }
	inline Reg<T> hsub() const { return Reduction<T,mipp::sub>::apply(*this); }
	inline Reg<T> hmul() const { return Reduction<T,mipp::mul>::apply(*this); }
	inline Reg<T> hdiv() const { return Reduction<T,mipp::div>::apply(*this); }
	inline Reg<T> hmin() const { return Reduction<T,mipp::min>::apply(*this); }
	inline Reg<T> hmax() const { return Reduction<T,mipp::max>::apply(*this); }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Reg<T>& r)
{
	dump<T>(r.r, os); return os;
}

template <typename T>
class Regx2 
{
public:
	Reg<T> val[2];

	Regx2(regx2 r2) : val{Reg<T>(r2.val[0]), Reg<T>(r2.val[1])} {}

	~Regx2() {}
};

#else /* ifdef NO_INTRINSICS */

template <typename T>
class Reg
{
public:
	T r;

	Reg(                             )              {}
	Reg(const T  val                 ) : r(val)     {}
	Reg(const T *data                ) : r(data[0]) {}
	Reg(const T *data, bool unaligned) : r(data[0]) {}

	~Reg() {}

	static inline Reg<T> cmask (const int mask[nElReg<T>()  ]) { return Reg<T>((T)0); }
	static inline Reg<T> cmask2(const int mask[nElReg<T>()/2]) { return Reg<T>((T)0); }

	static inline void transpose(Reg<T> regs[nElReg<T>()])
	{
		// nothing to do (regs is an array of 1x1...)
	}

	static inline void transpose8x8(Reg<T> regs[8])
	{
		std::cerr << "\"transpose8x8\" static method is a non-sense in sequential mode, exiting." << std::endl;
		exit(EXIT_FAILURE);
	}

	static inline void transpose2(Reg<T> regs[nElReg<T>()/2])
	{
		std::cerr << "\"transpose2\" static method is a non-sense in sequential mode, exiting." << std::endl;
		exit(EXIT_FAILURE);
	}

	static inline void transpose28x8(Reg<T> regs[8])
	{
		std::cerr << "\"transpose28x8\" static method is a non-sense in sequential mode, exiting." << std::endl;
		exit(EXIT_FAILURE);
	}

	inline void     set0         (               )                        { r = 0;                                    }
	inline void     set1         (const T val    )                        { r = val;                                  }
	inline void     load         (const T* data  )                        { r = data[0];                              }
	inline void     loadu        (const T* data  )                        { r = data[0];                              }
	inline void     store        (T* data        )                  const { data[0] = r;                              }
	inline void     storeu       (T* data        )                  const { data[0] = r;                              }

	inline Reg<T>   shuff        (const Reg<T> v_shu)               const { return *this;                             }
	inline Reg<T>   shuff2       (const Reg<T> v_shu)               const { return *this;                             }
	inline Reg<T>   interleavelo (const Reg<T> v)                   const { return *this;                             }
	inline Reg<T>   interleavehi (const Reg<T> v)                   const { return *this;                             }
	inline Reg<T>   interleavelo2(const Reg<T> v)                   const { return *this;                             }
	inline Reg<T>   interleavehi2(const Reg<T> v)                   const { return *this;                             }
	inline Regx2<T> interleave   (const Reg<T> v)                   const { return Regx2<T>(*this, v);                }
	inline Regx2<T> interleave2  (const Reg<T> v)                   const { return Regx2<T>(*this, v);                }
	inline Reg<T>   interleave   ()                                 const { return *this;                             }
	inline Regx2<T> interleavex2 (const Reg<T> v)                   const { return Regx2<T>(*this, v);                }
	inline Reg<T>   interleavex4 ()                                 const { return *this;                             }
	inline Reg<T>   interleavex16()                                 const { return *this;                             }
	inline Reg<T>   andb         (const Reg<T> v)                   const { return   r  &  v.r;                       }
	inline Reg<T>   andnb        (const Reg<T> v)                   const { return andb(~r);                          }
	inline Reg<T>   notb         ()                                 const { return  ~r;                               }
	inline Reg<T>   orb          (const Reg<T> v)                   const { return   r  |  v.r;                       }
	inline Reg<T>   xorb         (const Reg<T> v)                   const { return   r  ^  v.r;                       }
	inline Reg<T>   lshift       (const int n)                      const { return   r  << n;                         }
	inline Reg<T>   rshift       (const int n)                      const { return   r  >> n ;                        }
	inline Reg<T>   cmpeq        (const Reg<T> v)                   const { return   r  == v.r;                       }
	inline Reg<T>   cmpneq       (const Reg<T> v)                   const { return   r  != v.r;                       }
	inline Reg<T>   cmplt        (const Reg<T> v)                   const { return   r  <  v.r;                       }
	inline Reg<T>   cmple        (const Reg<T> v)                   const { return   r  <= v.r;                       }
	inline Reg<T>   cmpgt        (const Reg<T> v)                   const { return   r  >  v.r;                       }
	inline Reg<T>   cmpge        (const Reg<T> v)                   const { return   r  >= v.r;                       }
	inline Reg<T>   add          (const Reg<T> v)                   const { return   r  +  v.r;                       }
	inline Reg<T>   sub          (const Reg<T> v)                   const { return   r  -  v.r;                       }
	inline Reg<T>   mul          (const Reg<T> v)                   const { return   r  *  v.r;                       }
	inline Reg<T>   div          (const Reg<T> v)                   const { return   r  /  v.r;                       }
	inline Reg<T>   min          (const Reg<T> v)                   const { return std::min<T>(r, v.r);               }
	inline Reg<T>   max          (const Reg<T> v)                   const { return std::max<T>(r, v.r);               }
	// (1 = positive, -1 = negative, 0 = 0)
	inline Reg<T>   sign         ()                                 const { return (T(0) < r) - (r < T(0));           }
	inline Reg<T>   sign         (const Reg<T> v)                   const { return sign(Reg<T>(r ^ v.r));             }
	inline Reg<T>   neg          (const Reg<T> v)                   const { return v.r >= 0 ? Reg<T>(r) : Reg<T>(-r); }
	inline Reg<T>   abs          ()                                 const { return std::abs<T>(r);                    }
	inline Reg<T>   sqrt         ()                                 const { return std::sqrt<T>(r);                   }
	inline Reg<T>   rsqrt        ()                                 const { return 1 / std::sqrt<T>(r);               }
	inline Reg<T>   exp          ()                                 const { return std::exp<T>(r);                    }
	inline Reg<T>   fmadd        (const Reg<T> v1, const Reg<T> v2) const { return   r * v1.r + v2.r;                 }
	inline Reg<T>   fnmadd       (const Reg<T> v1, const Reg<T> v2) const { return -(r * v1.r + v2.r);                }
	inline Reg<T>   fmsub        (const Reg<T> v1, const Reg<T> v2) const { return   r * v1.r - v2.r;                 }
	inline Reg<T>   rot          ()                                 const { return r;                                 }
	inline Reg<T>   rotr         ()                                 const { return r;                                 }
	inline Reg<T>   div2         ()                                 const { return r * (T)0.50;                       }
	inline Reg<T>   div4         ()                                 const { return r * (T)0.25;                       }
	inline Reg<T>   sat          (T min, T max)                     const { return std::min(std::max(r, min), max);   }
	inline Reg<T>   round        ()                                 const { return std::round(r);                     }

	template <typename T2> inline Reg<T2> cvt ()               const { return (T2)r; }
	template <typename T2> inline Reg<T2> pack(const Reg<T> v) const
	{
		std::cerr << "\"pack\" method is a non-sense in sequential mode, exiting." << std::endl;
		exit(EXIT_FAILURE);
	}

	inline Reg<T>  operator~  (               ) const {                                          return ~r;   }

	inline Reg<T>& operator+= (const Reg<T> &v)       { r   = r + v.r;                           return *this; }
	inline Reg<T>& operator+= (const T*   data)       { r   = r + data[0];                       return *this; }
	inline Reg<T>  operator+  (      Reg<T>  v) const { v.r = r + v.r;                           return v;     }
	inline Reg<T>  operator+  (      T*   data) const { auto v = Reg<T>(data); v = (*this) + v;  return v;     }

	inline Reg<T>& operator-= (const Reg<T> &v)       { r   = r - v.r;                           return *this; }
	inline Reg<T>& operator-= (const T*   data)       { r   = r - data[0];                       return *this; }
	inline Reg<T>  operator-  (      Reg<T>  v) const { v.r = r - v.r;                           return v;     }
	inline Reg<T>  operator-  (      T*   data) const { auto v = Reg<T>(data); v = (*this) - v;  return v;     }

	inline Reg<T>& operator*= (const Reg<T> &v)       { r   = r * v.r;                           return *this; }
	inline Reg<T>& operator*= (const T*   data)       { r   = r * data[0];                       return *this; }
	inline Reg<T>  operator*  (      Reg<T>  v) const { v.r = r * v.r;                           return v;     }
	inline Reg<T>  operator*  (      T*   data) const { auto v = Reg<T>(data); v = (*this) * v;  return v;     }

	inline Reg<T>& operator/= (const Reg<T> &v)       { r   = r / v.r;                           return *this; }
	inline Reg<T>& operator/= (const T*   data)       { r   = r / data[0];                       return *this; }
	inline Reg<T>  operator/  (      Reg<T>  v) const { v.r = r / v.r;                           return v;     }
	inline Reg<T>  operator/  (      T*   data) const { auto v = Reg<T>(data); v = (*this) / v;  return v;     }

	inline Reg<T>& operator^= (const Reg<T> &v)       { r   = r ^ v.r;                           return *this; }
	inline Reg<T>& operator^= (const T*   data)       { r   = r ^ data[0];                       return *this; }
	inline Reg<T>  operator^  (      Reg<T>  v) const { v.r = r ^ v.r;                           return v;     }
	inline Reg<T>  operator^  (      T*   data) const { auto v = Reg<T>(data); v = (*this) ^ v;  return v;     }

	inline Reg<T>& operator|= (const Reg<T> &v)       { r   = r | v.r;                           return *this; }
	inline Reg<T>& operator|= (const T*   data)       { r   = r | data[0];                       return *this; }
	inline Reg<T>  operator|  (      Reg<T>  v) const { v.r = r | v.r;                           return v;     }
	inline Reg<T>  operator|  (      T*   data) const { auto v = Reg<T>(data); v = (*this) | v;  return v;     }

	inline Reg<T>& operator&= (const Reg<T> &v)       { r   = r & v.r;                           return *this; }
	inline Reg<T>& operator&= (const T*   data)       { r   = r & data[0];                       return *this; }
	inline Reg<T>  operator&  (      Reg<T>  v) const { v.r = r & v.r;                           return v;     }
	inline Reg<T>  operator&  (      T*   data) const { auto v = Reg<T>(data); v = (*this) & v;  return v;     }

	inline Reg<T>& operator<<=(const int n    )       { r = r << n;                              return *this; }
	inline Reg<T>  operator<< (const int n    ) const { Reg v = *this; v <<= n;                  return v;     }

	inline Reg<T>& operator>>=(const int n    )       { r = r >> n;                              return *this; }
	inline Reg<T>  operator>> (const int n    ) const { Reg v = *this; v >>= n;                  return v;     }

	inline Reg<T>  operator== (      Reg<T>  v) const { v = r == v.r;                            return v;     }
	inline Reg<T>  operator== (      T*   data) const { auto v = Reg<T>(data); v = (*this) == v; return v;     }

	inline Reg<T>  operator!= (      Reg<T>  v) const { v = r != v.r;                            return v;     }
	inline Reg<T>  operator!= (      T*   data) const { auto v = Reg<T>(data); v = (*this) != v; return v;     }

	inline Reg<T>  operator<  (      Reg<T>  v) const { v = r < v.r;                             return v;     }
	inline Reg<T>  operator<  (      T*   data) const { auto v = Reg<T>(data); v = (*this) <  v; return v;     }

	inline Reg<T>  operator<= (      Reg<T>  v) const { v = r <= v.r;                            return v;     }
	inline Reg<T>  operator<= (      T*   data) const { auto v = Reg<T>(data); v = (*this) <= v; return v;     }

	inline Reg<T>  operator>  (      Reg<T>  v) const { v = r > v.r;                             return v;     }
	inline Reg<T>  operator>  (      T*   data) const { auto v = Reg<T>(data); v = (*this) >  v; return v;     }

	inline Reg<T>  operator>= (      Reg<T>  v) const { v = r >= v.r;                            return v;     }
	inline Reg<T>  operator>= (      T*   data) const { auto v = Reg<T>(data); v = (*this) >= v; return v;     }

	// ------------------------------------------------------------------------------------------------------ reduction
	inline Reg<T> sum () const { return *this; }
	inline Reg<T> hadd() const { return *this; }
	inline Reg<T> hsub() const { return *this; }
	inline Reg<T> hmul() const { return *this; }
	inline Reg<T> hdiv() const { return *this; }
	inline Reg<T> hmin() const { return *this; }
	inline Reg<T> hmax() const { return *this; }
};

template <> Reg<int>         Reg<int        >::div2() const { return r >> 1; }
template <> Reg<short>       Reg<short      >::div2() const { return r >> 1; }
template <> Reg<signed char> Reg<signed char>::div2() const { return r >> 1; }

template <> Reg<int>         Reg<int        >::div4() const { return r >> 2; }
template <> Reg<short>       Reg<short      >::div4() const { return r >> 2; }
template <> Reg<signed char> Reg<signed char>::div4() const { return r >> 2; }

template <typename T>
std::ostream& operator<<(std::ostream& os, const Reg<T>& r)
{
	os << +r.r;
	return os;
}

template <typename T>
class Regx2 
{
public:
	Reg<T> val[2];

	Regx2()                                     {}
	Regx2(Reg<T> r1, Reg<T> r2) : val({r1, r2}) {}

	~Regx2() {}
};

#endif /* NO_INTRINSICS */

template <typename T> inline Reg<T>   shuff        (const Reg<T> v1, const Reg<T> v2)                  { return v1.shuff(v2);         }
template <typename T> inline Reg<T>   shuff2       (const Reg<T> v1, const Reg<T> v2)                  { return v1.shuff2(v2);        }
template <typename T> inline Reg<T>   interleavelo (const Reg<T> v1, const Reg<T> v2)                  { return v1.interleavelo(v2);  }
template <typename T> inline Reg<T>   interleavehi (const Reg<T> v1, const Reg<T> v2)                  { return v1.interleavehi(v2);  }
template <typename T> inline Reg<T>   interleavelo2(const Reg<T> v1, const Reg<T> v2)                  { return v1.interleavelo2(v2); }
template <typename T> inline Reg<T>   interleavehi2(const Reg<T> v1, const Reg<T> v2)                  { return v1.interleavehi2(v2); }
template <typename T> inline Regx2<T> interleave   (const Reg<T> v1, const Reg<T> v2)                  { return v1.interleave(v2);    }
template <typename T> inline Regx2<T> interleave2  (const Reg<T> v1, const Reg<T> v2)                  { return v1.interleave2(v2);   }
template <typename T> inline Reg<T>   interleave   (const Reg<T> v)                                    { return v.interleave();       }
template <typename T> inline Regx2<T> interleavex2 (const Reg<T> v1, const Reg<T> v2)                  { return v1.interleavex2(v2);  }
template <typename T> inline Reg<T>   interleavex4 (const Reg<T> v)                                    { return v.interleavex4();     }
template <typename T> inline Reg<T>   interleavex16(const Reg<T> v)                                    { return v.interleavex16();    }
template <typename T> inline Reg<T>   andb         (const Reg<T> v1, const Reg<T>v2)                   { return v1.andb(v2);          }
template <typename T> inline Reg<T>   andnb        (const Reg<T> v1, const Reg<T>v2)                   { return v1.andnb(v2);         }
template <typename T> inline Reg<T>   notb         (const Reg<T> v)                                    { return v.notb();             }
template <typename T> inline Reg<T>   orb          (const Reg<T> v1, const Reg<T> v2)                  { return v1.orb(v2);           }
template <typename T> inline Reg<T>   xorb         (const Reg<T> v1, const Reg<T> v2)                  { return v1.xorb(v2);          }
template <typename T> inline Reg<T>   lshift       (const Reg<T> v,  const int n)                      { return v.lshift(n);          }
template <typename T> inline Reg<T>   rshift       (const Reg<T> v,  const int n)                      { return v.rshift(n);          }
template <typename T> inline Reg<T>   cmpeq        (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmpeq(v2);         }
template <typename T> inline Reg<T>   cmpneq       (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmpneq(v2);        }
template <typename T> inline Reg<T>   cmplt        (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmplt(v2);         }
template <typename T> inline Reg<T>   cmple        (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmple(v2);         }
template <typename T> inline Reg<T>   cmpgt        (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmpgt(v2);         }
template <typename T> inline Reg<T>   cmpge        (const Reg<T> v1, const Reg<T> v2)                  { return v1.cmpge(v2);         }
template <typename T> inline Reg<T>   add          (const Reg<T> v1, const Reg<T> v2)                  { return v1.add(v2);           }
template <typename T> inline Reg<T>   sub          (const Reg<T> v1, const Reg<T> v2)                  { return v1.sub(v2);           }
template <typename T> inline Reg<T>   mul          (const Reg<T> v1, const Reg<T> v2)                  { return v1.mul(v2);           }
template <typename T> inline Reg<T>   div          (const Reg<T> v1, const Reg<T> v2)                  { return v1.div(v2);           }
template <typename T> inline Reg<T>   min          (const Reg<T> v1, const Reg<T> v2)                  { return v1.min(v2);           }
template <typename T> inline Reg<T>   max          (const Reg<T> v1, const Reg<T> v2)                  { return v1.max(v2);           }
template <typename T> inline Reg<T>   sign         (const Reg<T> v)                                    { return v.sign();             }
template <typename T> inline Reg<T>   sign         (const Reg<T> v1, const Reg<T> v2)                  { return v1.sign(v2);          }
template <typename T> inline Reg<T>   neg          (const Reg<T> v1, const Reg<T> v2)                  { return v1.andb(v2);          }
template <typename T> inline Reg<T>   abs          (const Reg<T> v)                                    { return v.abs();              }
template <typename T> inline Reg<T>   sqrt         (const Reg<T> v)                                    { return v.sqrt();             }
template <typename T> inline Reg<T>   rsqrt        (const Reg<T> v)                                    { return v.rsqrt();            }
template <typename T> inline Reg<T>   exp          (const Reg<T> v)                                    { return v.exp();              }
template <typename T> inline Reg<T>   fmadd        (const Reg<T> v1, const Reg<T> v2, const Reg<T> v3) { return v1.fmadd(v2, v3);     }
template <typename T> inline Reg<T>   fnmadd       (const Reg<T> v1, const Reg<T> v2, const Reg<T> v3) { return v1.fnmadd(v2, v3);    }
template <typename T> inline Reg<T>   fmsub        (const Reg<T> v1, const Reg<T> v2, const Reg<T> v3) { return v1.fmsub(v2, v3);     }
template <typename T> inline Reg<T>   rot          (const Reg<T> v)                                    { return v.rot();              }
template <typename T> inline Reg<T>   rotr         (const Reg<T> v)                                    { return v.rotr();             }
template <typename T> inline Reg<T>   div2         (const Reg<T> v)                                    { return v.div2();             }
template <typename T> inline Reg<T>   div4         (const Reg<T> v)                                    { return v.div4();             }
template <typename T> inline Reg<T>   sat          (const Reg<T> v, T min, T max)                      { return v.sat(min, max);      }
template <typename T> inline Reg<T>   round        (const Reg<T> v)                                    { return v.round();            }
template <typename T> inline Reg<T>   sum          (const Reg<T> v)                                    { return v.sum();              }
template <typename T> inline Reg<T>   hadd         (const Reg<T> v)                                    { return v.hadd();             }
template <typename T> inline Reg<T>   hmin         (const Reg<T> v)                                    { return v.hmin();             }
template <typename T> inline Reg<T>   hmax         (const Reg<T> v)                                    { return v.hmax();             }

template <typename T1, typename T2> 
inline Reg<T2> cvt(const Reg<T1> v) {
	return v.template cvt<T2>();
}

template <typename T1, typename T2>
inline Reg<T2> pack(const Reg<T1> v1, const Reg<T1> v2) {
	return v1.template pack<T2>(v2);
}

#ifndef NO_INTRINSICS
// ------------------------------------------------------------------------------------------------------- ARM NEON-128
// --------------------------------------------------------------------------------------------------------------------
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include "mipp_impl_NEON.hxx"
// -------------------------------------------------------------------------------------------------------- X86 AVX-512
// --------------------------------------------------------------------------------------------------------------------
#elif defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
#include "mipp_impl_AVX512.hxx"
// -------------------------------------------------------------------------------------------------------- X86 AVX-256
// --------------------------------------------------------------------------------------------------------------------
#elif defined(__AVX__)
#include "mipp_impl_AVX.hxx"
// -------------------------------------------------------------------------------------------------------- X86 SSE-128
// --------------------------------------------------------------------------------------------------------------------
#elif defined(__SSE__)
#include "mipp_impl_SSE.hxx"
#endif
#endif

}

#endif /* MY_INTRINSICS_PLUS_PLUS_H_ */
