#ifndef FB_EXTRACT_HPP
#define FB_EXTRACT_HPP

#include <string>
#include <vector>
#include <utility>
#include <stdexcept>
#include <algorithm>

#include "Tools/Code/Polar/Patterns/Pattern_polar_i.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B, int N_FRAMES = 1>
inline void fb_extract(const std::vector<std::pair<unsigned char, int>> &leaves_patterns, const B *V_N, B *V_K)
{
	static_assert(N_FRAMES > 0, "aff3ct::tools::fb_extract: N_FRAMES has to be bigger than 0.");

	auto off_s = 0;
	auto sk_idx = 0;

	const auto loop_size = (int)leaves_patterns.size();
	for (auto l = 0; l < loop_size; l++)
	{
		const auto node_type = (tools::polar_node_t)leaves_patterns[l].first;
		const auto n_elmts = leaves_patterns[l].second;
		switch (node_type)
		{
			case tools::RATE_0:
				break;
			case tools::RATE_1:
				std::copy(V_N +  off_s            * N_FRAMES,
				          V_N + (off_s + n_elmts) * N_FRAMES,
				          V_K + sk_idx);

				sk_idx += n_elmts * N_FRAMES;
				break;
			case tools::REP:
				std::copy(V_N + (off_s + n_elmts -1) * N_FRAMES,
				          V_N + (off_s + n_elmts +0) * N_FRAMES,
				          V_K + sk_idx);

				sk_idx += N_FRAMES;
				break;
			case tools::SPC:
				std::copy(V_N + (off_s + 1      ) * N_FRAMES,
				          V_N + (off_s + n_elmts) * N_FRAMES,
				          V_K + sk_idx);

				sk_idx += (n_elmts -1) * N_FRAMES;
				break;
			default:
				throw std::runtime_error("aff3ct::tools::fb_extract: unknown polar node type.");
				break;
		}
		off_s += n_elmts;
	}
}
}
}

#endif /* FB_EXTRACT_HPP */
