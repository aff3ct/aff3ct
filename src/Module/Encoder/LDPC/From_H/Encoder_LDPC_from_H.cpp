#include <cassert>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/matrix.h"

#include "Encoder_LDPC_from_H.hpp"

using namespace aff3ct;
using namespace module;

template <typename B>
Encoder_LDPC_from_H<B>
::Encoder_LDPC_from_H(const int K, const int N, const tools::AList_reader &alist_H, const int n_frames,
                      const std::string name)
: Encoder_LDPC<B>(K, N, n_frames, name)
{
	std::vector<mipp::vector<B>> H;
	build_H(alist_H.get_n_CN(), alist_H.get_n_VN(), alist_H.get_CN_to_VN(), H);
	triangularization_H(H, swapped);
	assert(H[0].size() == N);
	assert(H[0].size()-H.size() == K);
	identity_H(H);
	transformation_H_to_G(H, G);
	tools::real_transpose(K, N, G, this->tG);
}

template <typename B>
Encoder_LDPC_from_H<B>
::~Encoder_LDPC_from_H()
{
}

template <typename B>
void Encoder_LDPC_from_H<B>
::get_info_bits_pos(mipp::vector<B>& info_bits_pos)
{
	assert(this->K <= (int)info_bits_pos.size());
	mipp::vector<B> tmp(this->N);
	B tmp2;
	std::iota(tmp.begin(), tmp.begin() + this->N, 0);
	for(unsigned int l = 1; l <= (swapped.size() / 2); l++ )
	{
		tmp2 = tmp[swapped[l*2-1]];
		tmp[swapped[l*2-1]] = tmp[swapped[(l-1)*2]];
		tmp[swapped[(l-1)*2]] = tmp2;
	}
	std::copy(tmp.begin() + this->N - this->K, tmp.end(), info_bits_pos.begin());
}

template <typename B>
void Encoder_LDPC_from_H<B>
::get_G(mipp::vector<B>& G)
{
	assert(this->G.size() == G.size());
	std::copy(this->G.begin(), this->G.end(), G.begin());
}

template <typename B>
void Encoder_LDPC_from_H<B>
::build_H(const int k, const int n, const std::vector<std::vector<unsigned int>>& positions,
	      std::vector<mipp::vector<B>>& H)
{
	for(int i = 0; i < k; i++)
	{
		H.push_back(mipp::vector<B>(n,0));
	}
	for(unsigned int i = 0; i < positions.size(); i++)
	{
		for(unsigned int j = 0; j < positions[i].size(); j++)
		{
			H[i][positions[i][j]] = 1;
		}
	}
}

template <typename B>
void Encoder_LDPC_from_H<B>
::triangularization_H(std::vector<mipp::vector<B>>& H, mipp::vector<int>& swapped)
{
	unsigned int n(H[0].size());
	unsigned int k(H.size());
	int i(0);
	bool fund(false);
	mipp::vector<B> tmp(n,0);
	mipp::vector<B> tmp2;
	while( i < k-1 )
	{
		if( H[i][i] )
		{
			for(int j = (i+1); j < k; j++)
			{
 				if( H[j][i] )
				{
					std::transform(H[j].begin(), H[j].end(), H[i].begin(), H[j].begin(), std::not_equal_to<B>());
				}
			}
		}
		else
		{
			for(int j = (i+1); j < k; j++) // find an other row which is good
			{
				if( H[j][i] )
				{
					tmp = H[i];
					H[i] = H[j];
					H[j] = tmp;
					i--;
					fund = true;
					break;
				}
			}
			if( !fund ) // if does not fund
			{
				for(int j = (i+1); j<n; j++) // find an other column which is good
				{
					if( H[i][j] )
					{
						swapped.push_back(i);
						swapped.push_back(j);
						tmp2.clear();
						for(int l = 0; l < k; l++)
						{
							tmp2.push_back(H[l][i]);
						}
						for(int l = 0; l < k; l++)
						{
							H[l][i] = H[l][j];
						}
						for(int l = 0; l < k; l++)
						{
							H[l][j] = tmp2[l];
						}
						i--;
						fund = true;
						break;
					}
				}
 			}
			if( !fund ) // if does not fund again this mean that the row is the null vector
			{
				H.erase( H.begin() + i );
				i--;
				k--;
			}
			fund = false;
		}
		i++;
	}
}

template <typename B>
void Encoder_LDPC_from_H<B>
::identity_H(std::vector<mipp::vector<B>>& H)
{
	unsigned int k(H.size());
	for(int i = k-1 ; i > 0; i--)
	{
		for(int j = (i-1); j > -1; j--)
		{
			if( H[j][i] )
			{
				std::transform (H[j].begin(), H[j].end(), H[i].begin(), H[j].begin(), std::not_equal_to<B>());
			}
		}
	}
}

template <typename B>
void Encoder_LDPC_from_H<B>
::transformation_H_to_G(std::vector<mipp::vector<B>>& H, mipp::vector<B>& G)
{
	unsigned int n(H[0].size());
	unsigned int k(H.size());
	for(int i = 0; i < k; i++) // Kill of the Identity in H
	{
		H[i].erase( H[i].begin(), H[i].begin() + k );
	}
	for(int i = k; i < n; i++) // Add identity at the end
	{
		H.push_back(mipp::vector<B>(n-k,0));
		H[i][i-k] = 1;
	}
	// Re-organization: column of G
	mipp::vector<B> tmp(n-k,0);
	for(int l = (swapped.size() / 2); l > 0; l-- )
	{
		tmp = H[swapped[l*2-1]];
		H[swapped[l*2-1]] = H[swapped[(l-1)*2]];
		H[swapped[(l-1)*2]] = tmp;
	}
	// Write G matrix in G vector
	for(int j = 0; j < n-k; j++)
	{
		for(int i = 0; i < n; i++)
		{
			G.push_back(H[i][j]);
		}
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_LDPC_from_H<B_8>;
template class Encoder_LDPC_from_H<B_16>;
template class Encoder_LDPC_from_H<B_32>;
template class Encoder_LDPC_from_H<B_64>;
#else
template class Encoder_LDPC_from_H<B>;
#endif
// ==================================================================================== explicit template instantiation
