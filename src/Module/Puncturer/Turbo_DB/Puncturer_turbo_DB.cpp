#include <cmath>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"
#include "Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Puncturer_turbo_DB<B,Q>
::Puncturer_turbo_DB(const int &K, const int &N, const int n_frames)
: Puncturer<B,Q>(K, N, K * 3, n_frames)
{
	const std::string name = "Puncturer_turbo_DB";
	this->set_name(name);

	float rate = (float)K / (float)N;

	     if (std::round(100.0 * rate) == std::round(100 * 2.0 / 5.0)) coderate = "2/5";
	else if (std::round(100.0 * rate) == std::round(100 * 1.0 / 2.0)) coderate = "1/2";
	else if (std::round(100.0 * rate) == std::round(100 * 2.0 / 3.0)) coderate = "2/3";
	else if (std::round(100.0 * rate) == std::round(100 * 4.0 / 5.0)) coderate = "4/5";
	else
	{
		std::stringstream message;
		message << "Unsupported code rate 'R' = 'K' / 'N' ('R' = " << rate << ", 'K' = " << K << ", 'N' = "
		        << N << "), supported code rates are 2/5, 1/2, 2/3 and 4/5.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename B, typename Q>
Puncturer_turbo_DB<B,Q>* Puncturer_turbo_DB<B,Q>
::clone() const
{
	auto m = new Puncturer_turbo_DB<B,Q>(*this); // soft copy constructor
	m->deep_copy(*this); // hard copy
	return m;
}

template <typename B, typename Q>
void Puncturer_turbo_DB<B,Q>
::_puncture(const B *X_N1, B *X_N2, const int frame_id) const
{
	if (coderate == "1/2")// all the AB, all the Y1Y2
	{
		std::copy(X_N1, X_N1 + 2 * this->K, X_N2);
	}
	else if (coderate == "2/5")// all the AB, all the Y1Y2 , one W1W2 over two
	{
		std::copy(X_N1, X_N1 + 2 * this->K, X_N2);
		auto j =  2 * this->K;
		for(auto i = 0; i < this->K; i+=4)
		{
			X_N2[j++] = X_N1[2 * this->K + i    ];
			X_N2[j++] = X_N1[2 * this->K + i + 1];
		}
	}
	else if (coderate == "2/3") // all the AB, one Y1Y2 over two
	{
		std::copy(X_N1, X_N1 + this->K, X_N2);
		auto j =  this->K;
		for(auto i = 0; i < this->K; i+=4)
		{
			X_N2[j++] = X_N1[this->K + i    ];
			X_N2[j++] = X_N1[this->K + i + 1];
		}
	}
	else if (coderate == "4/5") // all the AB, one Y1Y2 over four
	{
		std::copy(X_N1, X_N1 + this->K, X_N2);
		auto j =  this->K;
		for(auto i = 0; i < this->K; i+=8)
		{
			X_N2[j++] = X_N1[this->K + i    ];
			X_N2[j++] = X_N1[this->K + i + 1];
		}
	}
}

template <typename B, typename Q>
void Puncturer_turbo_DB<B,Q>
::_depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const
{
	if (coderate == "1/2") // all the AB, all the Y1Y2
	{
		std::copy(Y_N1              , Y_N1 + 2 * this->K, Y_N2);
		std::fill(Y_N2 + 2 * this->K, Y_N2 + 3 * this->K, (Q)0);
	}
	else if (coderate == "2/5")// all the AB, all the Y1Y2 , one W1W2 over two
	{
		std::copy(Y_N1              , Y_N1 + 2 * this->K, Y_N2);
		std::fill(Y_N2 + 2 * this->K, Y_N2 + 3 * this->K, (Q)0);
		auto j =  2 * this->K;
		for(auto i = 0; i < this->K; i+=4)
		{
			Y_N2[2 * this->K + i    ] = Y_N1[j++];
			Y_N2[2 * this->K + i + 1] = Y_N1[j++];
		}
	}
	else if (coderate == "2/3") // all the AB, one Y1Y2 over two
	{
		std::copy(Y_N1          , Y_N1 +     this->K, Y_N2);
		std::fill(Y_N2 + this->K, Y_N2 + 3 * this->K, (Q)0);
		auto j =  this->K;
		for(auto i = 0; i < this->K; i+=4)
		{
			Y_N2[this->K + i    ] = Y_N1[j++];
			Y_N2[this->K + i + 1] = Y_N1[j++];
		}
	}
	else if (coderate == "4/5") // all the AB, one Y1Y2 over four
	{
		std::copy(Y_N1          , Y_N1 +     this->K, Y_N2);
		std::fill(Y_N2 + this->K, Y_N2 + 3 * this->K, (Q)0);
		auto j =  this->K;
		for(auto i = 0; i < this->K; i+=8)
		{
			Y_N2[this->K + i    ] = Y_N1[j++];
			Y_N2[this->K + i + 1] = Y_N1[j++];
		}
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Puncturer_turbo_DB<B_8,Q_8>;
template class aff3ct::module::Puncturer_turbo_DB<B_16,Q_16>;
template class aff3ct::module::Puncturer_turbo_DB<B_32,Q_32>;
template class aff3ct::module::Puncturer_turbo_DB<B_64,Q_64>;
#else
template class aff3ct::module::Puncturer_turbo_DB<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
