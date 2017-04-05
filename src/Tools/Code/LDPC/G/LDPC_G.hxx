#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "LDPC_G.hpp"

using namespace aff3ct::tools;

namespace aff3ct
{
namespace tools
{
template <typename B>
void LDPC_G
::build_H(const unsigned k, const unsigned n, const std::vector<std::vector<unsigned>>& positions,
          std::vector<mipp::vector<B>>& H)
{
	H.resize(k, mipp::vector<B>(n, 0));
	for (unsigned i = 0; i < positions.size(); i++)
		for (unsigned j = 0; j < positions[i].size(); j++)
			H[i][positions[i][j]] = 1;
}

template <typename B>
void LDPC_G
::triangularization_H(std::vector<mipp::vector<B>>& H, mipp::vector<unsigned>& swapped)
{
	unsigned n = H[0].size();
	unsigned k = H.size();
	unsigned i = 0;
	bool fund = false;

	mipp::vector<B> tmp(n,0);
	mipp::vector<B> tmp2;

	while (i < k)
	{
		if (H[i][i])
		{
			for (unsigned j = i +1; j < k; j++)
 				if( H[j][i] )
					std::transform(H[j].begin(), H[j].end(), H[i].begin(), H[j].begin(), std::not_equal_to<B>());
		}
		else
		{
			for (unsigned j = i +1; j < k; j++) // find an other row which is good
				if (H[j][i])
				{
					tmp = H[i];
					H[i] = H[j];
					H[j] = tmp;
					i--;
					fund = true;
					break;
				}

			if (!fund) // if does not fund
				for (unsigned j = i +1; j < n; j++) // find an other column which is good
					if (H[i][j])
					{
						swapped.push_back(i);
						swapped.push_back(j);

						tmp2.clear();

						for (unsigned l = 0; l < k; l++) tmp2.push_back(H[l][i]);
						for (unsigned l = 0; l < k; l++) H[l][i] = H[l][j];
						for (unsigned l = 0; l < k; l++) H[l][j] = tmp2[l];

						i--;
						fund = true;
						break;
					}

			if (!fund) // if does not fund again this mean that the row is the null vector
			{
				H.erase(H.begin() +i);
				i--;
				k--;
			}
			fund = false;
		}
		i++;
	}
}

template <typename B>
void LDPC_G
::identity_H(std::vector<mipp::vector<B>>& H)
{
	unsigned k = H.size();
	for (unsigned i = k - 1 ; i > 0; i--)
		for (unsigned j = i; j > 0; j--)
			if (H[j-1][i])
				std::transform (H[j-1].begin(), H[j-1].end(), H[i].begin(), H[j-1].begin(), std::not_equal_to<B>());

}

template <typename B>
void LDPC_G
::transformation_H_to_G(std::vector<mipp::vector<B>>& H, mipp::vector<B>& G, mipp::vector<unsigned>& swapped)
{
	unsigned n = H[0].size();
	unsigned k = H.size();

	for (unsigned i = 0; i < k; i++) // Kill of the Identity in H
		H[i].erase( H[i].begin(), H[i].begin() + k );

	H.resize(n, mipp::vector<B>(n-k,0));
	for (unsigned i = k; i < n; i++) // Add identity at the end
		H[i][i-k] = 1;

	// Re-organization: column of G
	mipp::vector<B> tmp(n - k, 0);
	for (unsigned l = (swapped.size() / 2); l > 0; l--)
	{
		tmp = H[swapped[l*2-1]];
		H[swapped[l*2-1]] = H[swapped[(l-1)*2]];
		H[swapped[(l-1)*2]] = tmp;
	}

	// Write G matrix in G vector
	G.resize(n*(n-k));
	unsigned long long compter = 0;
	for (unsigned j = 0; j < n - k; j++)
	{
		std::cout << j << " ";
		for (unsigned i = 0; i < n; i++)
			G[compter++] = H[i][j];
	}
}

}
}
