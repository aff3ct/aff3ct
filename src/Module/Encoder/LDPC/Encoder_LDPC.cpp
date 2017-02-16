#include <cassert>

#include "Tools/Display/bash_tools.h"

#include "Encoder_LDPC.hpp"

int EncValues[] = {
	13, 0, 2084, 1613, 1548, 1286, 1460, 3196, 4297, 2481, 3369, 3451, 4620, 2622,
	13, 1, 122, 1516, 3448, 2880, 1407, 1847, 3799, 3529, 373, 971, 4358, 3108,
	13, 2, 259, 3399, 929, 2650, 864, 3996, 3833, 107, 5287, 164, 3125, 2350,
	3, 3, 342, 3529,
	3, 4, 4198, 2147,
	3, 5, 1880, 4836,
	3, 6, 3864, 4910,
	3, 7, 243, 1542,
	3, 8, 3011, 1436,
	3, 9, 2167, 2512,
	3, 10, 4606, 1003,
	3, 11, 2835, 705,
	3, 12, 3426, 2365,
	3, 13, 3848, 2474,
	3, 14, 1360, 1743,
	3, 0, 163, 2536,
	3, 1, 2583, 1180,
	3, 2, 1542, 509,
	3, 3, 4418, 1005,
	3, 4, 5212, 5117,
	3, 5, 2155, 2922,
	3, 6, 347, 2696,
	3, 7, 226, 4296,
	3, 8, 1560, 487,
	3, 9, 3926, 1640,
	3, 10, 149, 2928,
	3, 11, 2364, 563,
	3, 12, 635, 688,
	3, 13, 231, 1684,
	3, 14, 1129, 3894
};

int N_LDPC   = 16200;
int K_LDPC   = 10800;
int NmK_LDPC = 5400;
int M_LDPC   = 360;
int Q_LDPC   = 15;
int N_LINES  = 30;


template <typename B>
Encoder_LDPC<B>
::Encoder_LDPC(const int K, const int N, const int n_frames, const std::string name)
: Encoder_sys<B>(K, N, n_frames, name), K(K), N(N)
{
//	assert( K == (K_LDPC) );
//	assert( N == (N_LDPC) );
}

template <typename B>
Encoder_LDPC<B>
::~Encoder_LDPC()
{
}

//template <typename B>
//void Encoder_LDPC<B>
//::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
//{
//	assert(U_K.size() <= X_N.size());
//	assert(this->K * this->n_frames == static_cast<int>(U_K.size()));
//	assert(this->N * this->n_frames == static_cast<int>(X_N.size()));
//
//	for (auto f = 0; f < this->n_frames; f++)
//	{
//		std::copy(U_K.begin() + (f+ 0) * this->K, U_K.begin() + (f +1) * this->K, X_N.begin() + f * this->N);
//
//		for (auto i = this->K; i < this->N; i++)
//			X_N[f * this->N + i] = (B)this->uniform_dist(this->rd_engine);
//	}
//}

template <typename B>
void Encoder_LDPC<B>
::encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par)
{
	std::cerr << "Unimplemented method" << std::endl;
}


// METHODE PERMETTANT D'ENCODER LES TRAINS DE BITS AU FORMAT DVB-S2 (r=9/10)

template <typename B>
void Encoder_LDPC<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	std::copy(U_K.begin(),   U_K.end(), X_N.begin());
	std::fill(X_N.begin()+K, X_N.end(), 0);

//	for(int y=0; y<N; y++)
//	{
//		X_N[y] = 0;
//	}
//
//	for(int y=0; y<K; y++)
//	{
//		int bit     = rand()%2;
//		U_K[y] = bit;
//		X_N[y] = bit;
//	}

	std::cout << "coucou" << std::endl;
	return;

	auto *Px  = &X_N[K_LDPC];
	auto *p   = EncValues;
	int xPos = 0;

	for(int y=0; y<N_LINES; y++)
	{
		int nbPos = (*p++);

		for(int l=0; l<M_LDPC; l++){

			int bit   = U_K[xPos];
			if( bit == 1 )
			{
				for(int q=0; q<nbPos; q++){
					int position = (p[q] + (xPos % 360) * Q_LDPC) % NmK_LDPC;
					Px[position] ^= bit;
				}
			}
			xPos += 1;
		}
		p += nbPos;
	}

	for(int i=1; i<NmK_LDPC; i++){
		Px[i] = Px[i] ^ Px[i-1];
	}
}


// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_LDPC<B_8>;
template class Encoder_LDPC<B_16>;
template class Encoder_LDPC<B_32>;
template class Encoder_LDPC<B_64>;
#else
template class Encoder_LDPC<B>;
#endif
// ==================================================================================== explicit template instantiation
