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
using namespace std;

template <typename B>
Encoder_LDPC_from_H<B>
::Encoder_LDPC_from_H(const int K, const int N, const tools::AList_reader &alist_H, const int n_frames,
                      const std::string name)
: Encoder_LDPC<B>(K, N, n_frames, name)
{
// I) take H
    // Take some information about H
    int n(alist_H.get_n_VN());
    int k(alist_H.get_n_CN());
    auto positions = alist_H.get_CN_to_VN();
    // Initialization of H
    vector<vector<bool> > H;
    for(int i = 0; i < k; i++)
    {
        H.push_back(vector<bool>(n,false));
    }
    // Complete the matrix H
    for(unsigned int i = 0; i < positions.size(); i++)
    {
        for(unsigned int j = 0; j < positions[i].size(); j++)
        {
            H[i][positions[i][j]] = true;
        }
    }

// II) Gauss pivot
    // Initialization of variables
    vector<int> swapped;
    int i(-1);
    bool fund(false);
    vector<bool> tmp1(n,false);
    vector<bool> tmp2;
    // Create a triangularization in H(1:k,1:k)
    while( i < k-1 )
    {
        i++;
        if( H[i][i] )
        {
            for(int j = (i+1); j < k; j++)
            {
                if( H[j][i] )
                {
                    transform (H[j].begin(), H[j].end(), H[i].begin(), H[j].begin(), not_equal_to<bool>());
                }
            }
        }
        else
        {
            for(int j = (i+1); j < k; j++) // find an other row which is good
            {
                if( H[j][i] )
                {
                    tmp1 = H[i];
                    H[i] = H[j];
                    H[j] = tmp1;
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
    }
    // assert of variable K abnd N give by the user
    assert(n == N);
    assert(n-k == K);
    // Create Identity in H(1:k,1:k)
    for(int i = k-1 ; i > 0; i--)
    {
        for(int j = (i-1); j > -1; j--)
        {
            if( H[j][i] )
            {
                transform (H[j].begin(), H[j].end(), H[i].begin(), H[j].begin(), not_equal_to<bool>());
            }
        }
    }
    // Creation of G with previous information
    for(int i = 0; i < k; i++) // Kill of the Identity in H
    {
        H[i].erase( H[i].begin(), H[i].begin() + k );
    }
    for(int i = k; i < n; i++) // Add identity at the end
    {
        H.push_back(vector<bool>(n-k,false));
        H[i][i-k] = true;
    }
    // Re-organization: column of G
    vector<bool> tmp3(n-k,false);
    for(int l = (swapped.size() / 2); l > 0; l-- )
    {
        tmp3 = H[swapped[l*2-1]];
        H[swapped[l*2-1]] = H[swapped[(l-1)*2]];
        H[swapped[(l-1)*2]] = tmp3;
    }
// III) Write G matrix in Gt vector
    for(int i = 0; i < N; i++)
    {
      for(int j = 0; j < K; j++)
      {
        this->tG[ K*i + j ] = H[i][j];
      }
    }
//    for(unsigned int i = 0; i< tG.size(); i++)
//    {
//      cout << tG[i];
//    }
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
	std::iota(info_bits_pos.begin(), info_bits_pos.begin() + this->K, 0);
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
