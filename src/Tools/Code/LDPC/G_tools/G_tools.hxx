#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "G_tools.hpp"

using namespace aff3ct::tools;

namespace aff3ct
{
namespace tools
{
template <typename B>
void G_tools
::build_H(const int k, const int n, const std::vector<std::vector<unsigned int>>& positions,
	      std::vector<mipp::vector<B>>& H)
{
	for (int i = 0; i < k; i++)
		H.push_back(mipp::vector<B>(n,0));

	for (unsigned int i = 0; i < positions.size(); i++)
		for(unsigned int j = 0; j < positions[i].size(); j++)
			H[i][positions[i][j]] = 1;
}

template <typename B>
void G_tools
::triangularization_H(std::vector<mipp::vector<B>>& H, mipp::vector<int>& swapped)
{
	unsigned n = H[0].size();
	unsigned k = H.size();
	unsigned i = 0;
	bool fund = false;

	mipp::vector<B> tmp(n,0);
	mipp::vector<B> tmp2;

	while ( i < k-1 )
	{
		if ( H[i][i] )
		{
			for (int j = (i+1); j < k; j++)
 				if( H[j][i] )
					std::transform(H[j].begin(), H[j].end(), H[i].begin(), H[j].begin(), std::not_equal_to<B>());
		}
		else
		{
			for (int j = (i+1); j < k; j++) // find an other row which is good
				if ( H[j][i] )
				{
					tmp = H[i];
					H[i] = H[j];
					H[j] = tmp;
					i--;
					fund = true;
					break;
				}

			if ( !fund ) // if does not fund
				for (int j = (i+1); j<n; j++) // find an other column which is good
					if ( H[i][j] )
					{
						swapped.push_back(i);
						swapped.push_back(j);

						tmp2.clear();

						for (int l = 0; l < k; l++) tmp2.push_back(H[l][i]);
						for (int l = 0; l < k; l++) H[l][i] = H[l][j];
						for (int l = 0; l < k; l++) H[l][j] = tmp2[l];

						i--;
						fund = true;
						break;
					}

			if ( !fund ) // if does not fund again this mean that the row is the null vector
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
void G_tools
::identity_H(std::vector<mipp::vector<B>>& H)
{
	auto k = (int)H.size();
	for (auto i = k-1 ; i > 0; i--)
		for (auto j = (i-1); j > -1; j--)
			if ( H[j][i] )
				std::transform (H[j].begin(), H[j].end(), H[i].begin(), H[j].begin(), std::not_equal_to<B>());

}

template <typename B>
void G_tools
::transformation_H_to_G(std::vector<mipp::vector<B>>& H, mipp::vector<B>& G, mipp::vector<int>& swapped)
{
	unsigned n = H[0].size();
	unsigned k = H.size();

	for (unsigned i = 0; i < k; i++) // Kill of the Identity in H
		H[i].erase( H[i].begin(), H[i].begin() + k );

	for (unsigned i = k; i < n; i++) // Add identity at the end
	{
		H.push_back(mipp::vector<B>(n-k,0));
		H[i][i-k] = 1;
	}

	// Re-organization: column of G
	mipp::vector<B> tmp(n-k,0);
	for (unsigned l = (swapped.size() / 2); l > 0; l-- )
	{
		tmp = H[swapped[l*2-1]];
		H[swapped[l*2-1]] = H[swapped[(l-1)*2]];
		H[swapped[(l-1)*2]] = tmp;
	}

	// Write G matrix in G vector
	for (unsigned j = 0; j < n-k; j++)
		for (unsigned i = 0; i < n; i++)
			G.push_back(H[i][j]);
}

}
}