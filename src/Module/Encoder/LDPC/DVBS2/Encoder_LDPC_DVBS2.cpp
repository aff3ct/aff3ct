#include <iostream>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/LDPC/DVBS2/Encoder_LDPC_DVBS2.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC_DVBS2<B>
::Encoder_LDPC_DVBS2(const tools::dvbs2_values& dvbs2)
: Encoder_LDPC<B>(dvbs2.K, dvbs2.N), dvbs2(dvbs2)
{
	const std::string name = "Encoder_LDPC_DVBS2";
	this->set_name(name);
}

template <typename B>
Encoder_LDPC_DVBS2<B>* Encoder_LDPC_DVBS2<B>
::clone() const
{
	auto m = new Encoder_LDPC_DVBS2(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_LDPC_DVBS2<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	std::copy(U_K,           U_K + this->K, X_N);
	std::fill(X_N + this->K, X_N + this->N, 0  );

	B* Px = X_N + this->K;
	const int *p = dvbs2.EncValues.data();
	int xPos = 0;

	for (int y = 0; y < dvbs2.N_LINES; y++)
	{
		int nbPos = (*p++);

		for (int l = 0; l < dvbs2.M; l++)
		{
			int bit = (int)U_K[xPos];
			if (bit == 1)
			{
				for (int q = 0; q < nbPos; q++)
				{
					// int position  = (p[q] + (xPos % dvbs2.M) * dvbs2.Q) % dvbs2.NmK;
					int position  = (p[q] + l * dvbs2.Q) % dvbs2.NmK;
					Px[position] ^= bit;
				}
			}
			xPos++;
		}
		p += nbPos;
	}

	for (int i = 1; i < dvbs2.NmK; i++)
		Px[i] = Px[i] ^ Px[i-1];
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_LDPC_DVBS2<B_8>;
template class aff3ct::module::Encoder_LDPC_DVBS2<B_16>;
template class aff3ct::module::Encoder_LDPC_DVBS2<B_32>;
template class aff3ct::module::Encoder_LDPC_DVBS2<B_64>;
#else
template class aff3ct::module::Encoder_LDPC_DVBS2<B>;
#endif
// ==================================================================================== explicit template instantiation
