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
::Puncturer_turbo_DB(const int &K, const int &N)
: Puncturer<B,Q>(K, N, K * 3)
{
	const std::string name = "Puncturer_turbo_DB";
	this->set_name(name);

	float rate = (float)K / (float)N;

	switch (int(rate * 100)) 
	{
		case 100 * 1 / 3:
			coderate = "1/3";
			break;
		case 100 * 2 / 5:
			coderate = "2/5";
			break;
		case 100 * 1 / 2:
			coderate = "1/2";
			break;
		case 100 * 2 / 3:
			coderate = "2/3";
			break;
		case 100 * 3 / 4:
			coderate = "3/4";
			break;
		case 100 * 4 / 5:
			coderate = "4/5";
			break;
		case 100 * 5 / 6:
			coderate = "5/6";
			break;
		case 100 * 6 / 7:
			coderate = "6/7";
			break;
		case 100 * 7 / 8:
			coderate = "7/8";
			break;
		default:
			std::stringstream message;
			message << "Unsupported code rate 'R' = 'K' / 'N' ('R' = " << rate << ", 'K' = " << K << ", 'N' = "
					<< N << "), supported code rates are 1/3, 2/5, 1/2, 2/3, 3/4, 4/5, 5/6, 6/7 and 7/8.";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename B, typename Q>
Puncturer_turbo_DB<B,Q>* Puncturer_turbo_DB<B,Q>
::clone() const
{
	auto m = new Puncturer_turbo_DB(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename Q>
void Puncturer_turbo_DB<B,Q>
::_puncture(const B *X_N1, B *X_N2, const size_t frame_id) const
{
	if (coderate == "1/3")	// Pattern: [1] [1]
	{
		std::copy(X_N1, X_N1 + this->N_cw, X_N2);
	}
	else if (coderate == "2/5") // Pattern: [11] [10]
	{
		std::copy(X_N1, X_N1 + 2 * this->K, X_N2);
		auto j = 2 * this->K;
		for(auto i = 0; i < this->K; i += 4)
		{
			X_N2[j++] = X_N1[2 * this->K + i    ];
			X_N2[j++] = X_N1[2 * this->K + i + 1];
		}
	}
	else if (coderate == "1/2")	// Pattern: [1] [0]
	{
		std::copy(X_N1, X_N1 + 2 * this->K, X_N2);
	}
	else if (coderate == "2/3")	// Pattern: [10] [0]
	{
		std::copy(X_N1, X_N1 + this->K, X_N2);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 4)
		{
			X_N2[j++] = X_N1[this->K + i    ];
			X_N2[j++] = X_N1[this->K + i + 1];
		}
	}
	else if (coderate == "3/4")	// Pattern: [101000] [0]
	{
		std::copy(X_N1, X_N1 + this->K, X_N2);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 12)
		{
			X_N2[j++] = X_N1[this->K + i    ];
			X_N2[j++] = X_N1[this->K + i + 1];
			X_N2[j++] = X_N1[this->K + i + 4];
			X_N2[j++] = X_N1[this->K + i + 5];
		}
	}
	else if (coderate == "4/5")	// Pattern: [1000] [0]
	{
		std::copy(X_N1, X_N1 + this->K, X_N2);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 8)
		{
			X_N2[j++] = X_N1[this->K + i    ];
			X_N2[j++] = X_N1[this->K + i + 1];
		}
	}
	else if (coderate == "5/6") // Pattern: [10001000100010000000] [0]
	{
		std::copy(X_N1, X_N1 + this->K, X_N2);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 40)
		{
			X_N2[j++] = X_N1[this->K + i     ];
			X_N2[j++] = X_N1[this->K + i + 1 ];
			X_N2[j++] = X_N1[this->K + i + 8 ];
			X_N2[j++] = X_N1[this->K + i + 9 ];
			X_N2[j++] = X_N1[this->K + i + 16];
			X_N2[j++] = X_N1[this->K + i + 17];
			X_N2[j++] = X_N1[this->K + i + 24];
			X_N2[j++] = X_N1[this->K + i + 25];
		}
	}
	else if (coderate == "6/7") // Pattern: [100010000000] [0]
	{
		std::copy(X_N1, X_N1 + this->K, X_N2);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 24)
		{
			X_N2[j++] = X_N1[this->K + i    ];
			X_N2[j++] = X_N1[this->K + i + 1];
			X_N2[j++] = X_N1[this->K + i + 8];
			X_N2[j++] = X_N1[this->K + i + 9];
		}
	}
	else if (coderate == "7/8") // Pattern: [1000100000001000000010000000] [0]
	{
		std::copy(X_N1, X_N1 + this->K, X_N2);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 56)
		{
			X_N2[j++] = X_N1[this->K + i     ];
			X_N2[j++] = X_N1[this->K + i + 1 ];
			X_N2[j++] = X_N1[this->K + i + 8 ];
			X_N2[j++] = X_N1[this->K + i + 9 ];
			X_N2[j++] = X_N1[this->K + i + 24];
			X_N2[j++] = X_N1[this->K + i + 25];
			X_N2[j++] = X_N1[this->K + i + 40];
			X_N2[j++] = X_N1[this->K + i + 41];
		}
	}
}

template <typename B, typename Q>
void Puncturer_turbo_DB<B,Q>
::_depuncture(const Q *Y_N1, Q *Y_N2, const size_t frame_id) const
{
	if (coderate == "1/3")	// Pattern: [1] [1]
	{
		std::copy(Y_N1, Y_N1 + this->N_cw, Y_N2);
	}
	else if (coderate == "2/5") // Pattern: [11] [10]
	{
		std::copy(Y_N1, Y_N1 + 2 * this->K, Y_N2);
		std::fill(Y_N2 + 2 * this->K, Y_N2 + this->N_cw, (Q)0);
		auto j = 2 * this->K;
		for(auto i = 0; i < this->K; i += 4)
		{
			Y_N2[2 * this->K + i    ] = Y_N1[j++];
			Y_N2[2 * this->K + i + 1] = Y_N1[j++];
		}
	}
	else if (coderate == "1/2")	// Pattern: [1] [0]
	{
		std::copy(Y_N1, Y_N1 + 2 * this->K, Y_N2);
		std::fill(Y_N2 + 2 * this->K, Y_N2 + this->N_cw, (Q)0);
	}
	else if (coderate == "2/3")	// Pattern: [10] [0]
	{
		std::copy(Y_N1, Y_N1 + this->K, Y_N2);
		std::fill(Y_N2 + this->K, Y_N2 + this->N_cw, (Q)0);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 4)
		{
			Y_N2[this->K + i    ] = Y_N1[j++];
			Y_N2[this->K + i + 1] = Y_N1[j++];
		}
	}
	else if (coderate == "3/4")	// Pattern: [101000] [0]
	{
		std::copy(Y_N1, Y_N1 + this->K, Y_N2);
		std::fill(Y_N2 + this->K, Y_N2 + this->N_cw, (Q)0);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 12)
		{
			Y_N2[this->K + i    ] = Y_N1[j++];
			Y_N2[this->K + i + 1] = Y_N1[j++];
			Y_N2[this->K + i + 4] = Y_N1[j++];
			Y_N2[this->K + i + 5] = Y_N1[j++];
		}
	}
	else if (coderate == "4/5")	// Pattern: [1000] [0]
	{
		std::copy(Y_N1, Y_N1 + this->K, Y_N2);
		std::fill(Y_N2 + this->K, Y_N2 + this->N_cw, (Q)0);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 8)
		{
			Y_N2[this->K + i    ] = Y_N1[j++];
			Y_N2[this->K + i + 1] = Y_N1[j++];
		}
	}
	else if (coderate == "5/6") // Pattern: [10001000100010000000] [0]
	{
		std::copy(Y_N1, Y_N1 + this->K, Y_N2);
		std::fill(Y_N2 + this->K, Y_N2 + this->N_cw, (Q)0);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 40)
		{
			Y_N2[this->K + i     ] = Y_N1[j++];
			Y_N2[this->K + i + 1 ] = Y_N1[j++];
			Y_N2[this->K + i + 8 ] = Y_N1[j++];
			Y_N2[this->K + i + 9 ] = Y_N1[j++];
			Y_N2[this->K + i + 16] = Y_N1[j++];
			Y_N2[this->K + i + 17] = Y_N1[j++];
			Y_N2[this->K + i + 24] = Y_N1[j++];
			Y_N2[this->K + i + 25] = Y_N1[j++];
		}
	}
	else if (coderate == "6/7") // Pattern: [100010000000] [0]
	{
		std::copy(Y_N1, Y_N1 + this->K, Y_N2);
		std::fill(Y_N2 + this->K, Y_N2 + this->N_cw, (Q)0);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 24)
		{
			Y_N2[this->K + i    ] = Y_N1[j++];
			Y_N2[this->K + i + 1] = Y_N1[j++];
			Y_N2[this->K + i + 8] = Y_N1[j++];
			Y_N2[this->K + i + 9] = Y_N1[j++];
		}
	}
	else if (coderate == "7/8") // Pattern: [1000100000001000000010000000] [0]
	{
		std::copy(Y_N1, Y_N1 + this->K, Y_N2);
		std::fill(Y_N2 + this->K, Y_N2 + this->N_cw, (Q)0);
		auto j = this->K;
		for(auto i = 0; i < this->K; i += 56)
		{
			Y_N2[this->K + i     ] = Y_N1[j++];
			Y_N2[this->K + i + 1 ] = Y_N1[j++];
			Y_N2[this->K + i + 8 ] = Y_N1[j++];
			Y_N2[this->K + i + 9 ] = Y_N1[j++];
			Y_N2[this->K + i + 24] = Y_N1[j++];
			Y_N2[this->K + i + 25] = Y_N1[j++];
			Y_N2[this->K + i + 40] = Y_N1[j++];
			Y_N2[this->K + i + 41] = Y_N1[j++];
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
