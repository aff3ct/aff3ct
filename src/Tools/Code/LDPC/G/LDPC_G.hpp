#ifndef LDPC_G_HPP_
#define LDPC_G_HPP_

#include <fstream>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace tools
{
struct LDPC_G
{
public:
	template <typename B>
	static void build_H(const int k, const int n, const std::vector<std::vector<unsigned int>>& positions,
	                    std::vector<mipp::vector<B>>& H);
	template <typename B>
	static void triangularization_H(std::vector<mipp::vector<B>>& H, mipp::vector<int>& swapped);
	template <typename B>
	static void identity_H(std::vector<mipp::vector<B>>& H);
	template <typename B>
	static void transformation_H_to_G(std::vector<mipp::vector<B>>& H, mipp::vector<B>& G, mipp::vector<int>& swapped);
};
}
}

#include "LDPC_G.hxx"

#endif /*G_TOOLS_HPP_ */
