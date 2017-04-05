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
	static void build_H(const unsigned k, const unsigned n, const std::vector<std::vector<unsigned>>& positions,
	                    std::vector<mipp::vector<B>>& H);
	template <typename B>
	static void triangularization_H(std::vector<mipp::vector<B>>& H, mipp::vector<unsigned>& swapped);
	template <typename B>
	static void identity_H(std::vector<mipp::vector<B>>& H);
	template <typename B>
	static void transformation_H_to_G(std::vector<mipp::vector<B>>& H, mipp::vector<B>& G, mipp::vector<unsigned>& swapped);
};
}
}

#include "LDPC_G.hxx"

#endif /*G_TOOLS_HPP_ */
