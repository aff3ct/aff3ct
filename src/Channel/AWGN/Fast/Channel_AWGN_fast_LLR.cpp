#include "Channel_AWGN_fast_LLR.hpp"

#include <algorithm>
#include <cassert>

template <typename R>
Channel_AWGN_fast_LLR<R>
::Channel_AWGN_fast_LLR(const R& sigma, const int seed, const R scaling_factor)
: sigma(sigma),
  scaling_factor(scaling_factor),
  rd(),
  rd_engine(this->rd()),
  dist(std::numeric_limits<R>::min(),1)
{
	rd_engine.seed(seed);

	std::cout << "min = " << std::numeric_limits<R>::min() << std::endl;
}

template <typename R>
Channel_AWGN_fast_LLR<R>
::~Channel_AWGN_fast_LLR()
{
}

// box muller method in the polar form
template <typename R>
void Channel_AWGN_fast_LLR<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size() % 2                       == 0         );
	assert(X_N.size()                           == Y_N.size());
	assert(scaling_factor                       != 0         );
	assert(sigma                                != 0         );
	assert(X_N.size() % (2 * mipp::nElReg<R>()) == 0         );

	const auto loop_size = (int)Y_N.size();
	for (auto i = 0; i < loop_size; i++)
	{
		Y_N[i] = this->dist(this->rd_engine);
		// if (Y_N[i] == 0)
		// 	std::cout << "PROUTTTT" << std::endl;
	}

	const mipp::Reg<R> sf       = this->scaling_factor;
	const mipp::Reg<R> sigma    = this->sigma;
	const mipp::Reg<R> twopi    = 2.0 * 3.14159265358979323846;
	const mipp::Reg<R> one      = 1.0;
	const mipp::Reg<R> minustwo = -2.0;

	for (auto i = 0; i < loop_size; i += mipp::nElReg<R>() * 2) 
	{
		// const auto u1 = one - mipp::Reg<R>(&Y_N[i                    ]); // [0, 1) -> (0, 1]
		const auto u1 = mipp::Reg<R>(&Y_N[i                    ]);
		const auto u2 = mipp::Reg<R>(&Y_N[i + mipp::nElReg<R>()]);

		const auto radius = mipp::sqrt(minustwo * mipp::log(u1)) * sigma;
		const auto theta  = twopi * u2;

		mipp::Reg<R> sintheta, costheta;
		mipp::sincos(theta, sintheta, costheta);

		// fmadd(a, b, c) = a * b + c
		auto awgn1 = mipp::fmadd(radius, costheta, mipp::Reg<R>(&X_N[i                    ])) * sf;
		auto awgn2 = mipp::fmadd(radius, sintheta, mipp::Reg<R>(&X_N[i + mipp::nElReg<R>()])) * sf;

		awgn1.store(&Y_N[i                    ]);
		awgn2.store(&Y_N[i + mipp::nElReg<R>()]);
	}

	// auto twopi = (R)(2.0 * 3.14159265358979323846);
	// for (auto i = 0; i < loop_size; i += 2)
	// {
	// 	// auto x  = (R)sqrt(-2.0 * log((R)rand() / (R)2147483647));
	// 	// auto y  = (R)rand() / (R)2147483647;
	// 	const auto u1 = Y_N[i +0];
	// 	const auto u2 = Y_N[i +1];

	// 	const auto radius = (R)std::sqrt((R)-2.0 * std::log(u1)) * sigma;
	// 	const auto theta  = twopi * u2;

	// 	const auto sintheta = std::sin(theta);
	// 	const auto costheta = std::cos(theta);

	// 	Y_N[i +0] = (X_N[i +0] + radius * sintheta) * scaling_factor;
	// 	Y_N[i +1] = (X_N[i +1] + radius * costheta) * scaling_factor;
	// }

	// for (auto i = 0; i < loop_size; i++)
	// 	if (std::isnan(Y_N[i]))
	// 		std::cout << "NaN" << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_AWGN_fast_LLR<R_32>;
template class Channel_AWGN_fast_LLR<R_64>;
#else
template class Channel_AWGN_fast_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation