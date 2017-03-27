#ifndef G_TOOLS_HPP_
#define G_TOOLS_HPP_

#include <fstream>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace tools
{
struct G_tools
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

#include "G_tools.hxx"

#endif /*G_TOOLS_HPP_ */
