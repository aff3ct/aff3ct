#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <map>

#include "Tools/Code/Polar/Pattern_parser_polar.hpp"
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Perf/Transpose/transpose_selector.h"
#include "Tools/Display/bash_tools.h"

#include "Tools/Code/Polar/Patterns/Pattern_SC_standard.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_SC_rate0.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_SC_rate1.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_SC_rep.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_SC_spc.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_SC_rate0_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_SC_rep_left.hpp"

#include "Decoder_polar_SCL_fast_sys.hpp"

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_fast_sys<B,R,API_polar>
::Decoder_polar_SCL_fast_sys(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits,
                             const int n_frames, const std::string name)
: Decoder<B,R>  (K, N, n_frames, API_polar::get_n_frames(), name),
  m             (std::log2(N)),
  L             (L),
  frozen_bits   (frozen_bits),
  polar_patterns(N,
                 frozen_bits,
                 {new Pattern_SC<pattern_SC_type::STANDARD   >(),
                  // new Pattern_SC<pattern_SC_type::RATE_0     >(),
                  // new Pattern_SC<pattern_SC_type::RATE_1     >(),
                  // new Pattern_SC<pattern_SC_type::RATE_0_LEFT>(),
                  // new Pattern_SC<pattern_SC_type::REP_LEFT   >(),
                  // new Pattern_SC<pattern_SC_type::REP        >(),
                  /*new Pattern_SC<pattern_SC_type::SPC        >()*/}, // perf. degradation with SPC nodes length > 4
                  new Pattern_SC<pattern_SC_type::RATE_0     >(),
                  new Pattern_SC<pattern_SC_type::RATE_1     >()),
  paths         (L),
  last_paths    (L),
  metrics       (L),
  y             (                   N + mipp::nElReg<R>()    ),
  l             (L, mipp::vector<R>(N + mipp::nElReg<R>()   )),
  s             (L, mipp::vector<B>(N + mipp::nElReg<B>(), 0)),
  metrics_vec   (3, std::vector<float>()),
  metrics_idx   (3, std::vector<int  >()),
  dup_count     (L, 0),
  llr_indexes   (),
  bit_flips     (4 * L),
  is_even       (L),
  best_path     (0),
  n_active_paths(1),
  n_array_ref   (L, std::vector<int> (m, 0   )),
  path_2_array  (L, std::vector<int> (m, 0   ))
{
	static_assert(API_polar::get_n_frames() == 1, "The inter-frame API_polar is not supported.");
	static_assert(sizeof(B) == sizeof(R), "Sizes of the bits and reals have to be identical.");

	metrics_vec[0].resize(L * 2);
	metrics_idx[0].resize(L * 2);

	metrics_vec[1].resize(L * 4);
	metrics_idx[1].resize(L * 4);

	metrics_vec[2].resize((L <= 2 ? 2 : 8) * L);
	metrics_idx[2].resize((L <= 2 ? 2 : 8) * L);

	for (auto j = 0 ; j < 3 ; j++)
		for (size_t i = 0 ; i < metrics_idx[j].size() ; i++)
			metrics_idx[j][i] = i;

	for (auto i = 0; i <= m; i ++)
	{
		llr_indexes.push_back(std::vector<int>(std::exp2(i)));
		for (size_t j = 0 ; j < llr_indexes[i].size() ; j++)
			llr_indexes[i][j] = j;
	}
}

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_fast_sys<B,R,API_polar>
::~Decoder_polar_SCL_fast_sys()
{
	polar_patterns.release_patterns();
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::load(const mipp::vector<R>& Y_N)
{
	std::fill(metrics.begin(), metrics.end(), std::numeric_limits<float>::min());
	for (auto i = 0; i < L; i++) paths[i] = i;
	n_active_paths = 1;

	for (auto i = 0; i < 3; i++)
		std::fill(metrics_vec[i].begin(), metrics_vec[i].end(), std::numeric_limits<float>::max());

	std::copy(Y_N.begin(), Y_N.end(), y.begin());

	// at the beginning, path 0 points to array 0
	std::fill(n_array_ref [0].begin(), n_array_ref [0].end(), 1);
	std::fill(path_2_array[0].begin(), path_2_array[0].end(), 0);

	for (auto i = 1; i < L; i++)
		std::fill(n_array_ref[i]  .begin(), n_array_ref[i]  .end(), 0   );
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::hard_decode()
{
	int first_node_id = 0, off_l = 0, off_s = 0;
	recursive_decode(off_l, off_s, m, first_node_id);
	select_best_path();
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::recursive_decode(const int off_l, const int off_s, const int rev_depth, int &node_id)
{
	const int n_elmts = 1 << rev_depth;
	const int n_elm_2 = n_elmts >> 1;
	const pattern_SC_type node_type = polar_patterns.get_node_type(node_id);

	const bool is_terminal_pattern = (node_type == pattern_SC_type::RATE_0) ||
	                                 (node_type == pattern_SC_type::RATE_1) ||
	                                 (node_type == pattern_SC_type::REP)    ||
	                                 (node_type == pattern_SC_type::SPC);

	// root node
	if (/*!is_terminal_pattern &&*/ rev_depth == m)
	{
		// f
		switch (node_type)
		{
			case STANDARD:
				API_polar::f(y.data(), y.data() + n_elm_2, l[0].data(), n_elm_2);
				break;
			case REP_LEFT:
				API_polar::f(y.data(), y.data() + n_elm_2, l[0].data(), n_elm_2);
				break;
			case RATE_0_LEFT:
				if(n_active_paths > 1)
					API_polar::f(y.data(), y.data() + n_elm_2, l[0].data(), n_elm_2);
				break;
			default:
				// TODO: we should not have to do this (for instance when RATE_0_LEFT node we can avoid to compute f...)
				// TODO: yes we should because contrary to SC, llrs are needed to update the path metric
				// TODO: the only case where we could avoid this is when there is only one path, then the metric doesn't need to be refreshed			
				API_polar::f(y.data(), y.data() + n_elm_2, l[0].data(), n_elm_2);
				break;
		}

		recursive_decode(off_l, off_s, rev_depth -1, ++node_id); // recursive call left

		// g
		// allocate arrays to paths
		for (auto i = 0; i < n_active_paths; i++)


		switch (node_type)
		{
			case STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
				{
					allocate_array(paths[i], rev_depth -1);
					API_polar::g (y.data(), y.data() + n_elm_2, s[paths[i]].data() + off_s,l[paths[i]].data(), n_elm_2);
				}
				break;
			case RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					allocate_array(paths[i], rev_depth -1);
					API_polar::g0(y.data(), y.data() + n_elm_2,                            l[paths[i]].data(), n_elm_2);
				}
				break;
			case REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					allocate_array(paths[i], rev_depth -1);
					API_polar::gr(y.data(), y.data() + n_elm_2, s[paths[i]].data() + off_s,l[paths[i]].data(), n_elm_2);
				}
				break;
			default:
				break;
		}

		recursive_decode(off_l, off_s + n_elm_2, rev_depth -1, ++node_id); // recursive call right

		// xor
		switch (node_type)
		{
			case STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2); break;
			case RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo0(s[paths[i]],        off_s + n_elm_2, off_s, n_elm_2); break;
			case REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2); break;
			default:
				break;
		}
	}
	else if (!is_terminal_pattern && rev_depth)
	{
		// f
		switch (node_type)
		{
			case STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
				{
					allocate_array(paths[i], rev_depth -1);
					API_polar::f(l[paths[i]], off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2);
				}
				break;
			case REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					allocate_array(paths[i], rev_depth -1);
					API_polar::f(l[paths[i]], off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2);
				}
				break;
			case RATE_0_LEFT:
				if(n_active_paths > 1)
					for (auto i = 0; i < n_active_paths; i++)
					{
						allocate_array(paths[i], rev_depth -1);
						API_polar::f(l[paths[i]], off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2);
					}
				break;
			default:
				// TODO: we should not have to do this (for instance when RATE0_LEFT node we can avoid to compute f...)
				// TODO: yes we should because contrary to SC, llrs are needed to update the path metric
				// TODO: the only case where we could avoid this is when there is only one path, then the metric doesn't need to be refreshed
				for (auto i = 0; i < n_active_paths; i++)
				{
					allocate_array(paths[i], rev_depth -1);
					API_polar::f(l[paths[i]], off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2);
				}
				break;
		}

		recursive_decode(off_l + n_elmts, off_s, rev_depth -1, ++node_id); // recursive call left

		// g
		switch (node_type)
		{
			case STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
				{
					allocate_array(paths[i], rev_depth -1);
					API_polar::g (s[paths[i]], l[paths[i]], off_l, off_l + n_elm_2, off_s, off_l + n_elmts, n_elm_2);
				}
				break;
			case RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					allocate_array(paths[i], rev_depth -1);
					API_polar::g0(             l[paths[i]], off_l, off_l + n_elm_2,        off_l + n_elmts, n_elm_2);
				}
				break;
			case REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					allocate_array(paths[i], rev_depth -1);
					API_polar::gr(s[paths[i]], l[paths[i]], off_l, off_l + n_elm_2, off_s, off_l + n_elmts, n_elm_2);
				}
				break;
			default:
				break;
		}

		recursive_decode(off_l + n_elmts, off_s + n_elm_2, rev_depth -1, ++node_id); // recursive call right

		// xor
		switch (node_type)
		{
			case STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2); break;
			case RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo0(s[paths[i]],        off_s + n_elm_2, off_s, n_elm_2); break;
			case REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2); break;
			default:
				break;
		}
	}
	else
	{
		// h
		switch (node_type)
		{
			case RATE_0: update_paths_r0 (           off_l, off_s, n_elmts); break;
			case REP:    update_paths_rep(           off_l, off_s, n_elmts); break;
			case RATE_1: update_paths_r1 (rev_depth, off_l, off_s, n_elmts); break;
			case SPC:    update_paths_spc(rev_depth, off_l, off_s, n_elmts); break;
			default:
				break;
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() >= (unsigned) this->K);

	auto k = 0;
	for (auto i = 0; i < this->N; i++)
		if (!frozen_bits[i])
			V_K[k++] = s[best_path][i] ? 1 : 0;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::store_fast(mipp::vector<B>& V) const
{
	assert(V.size() == (unsigned) this->N);
	std::copy(s[best_path].begin(), s[best_path].end() - mipp::nElReg<B>(), V.begin());
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::unpack(mipp::vector<B>& V_N) const
{
	assert(V_N.size() == frozen_bits.size());

	for (unsigned i = 0; i < V_N.size(); i++)
		V_N[i] = !frozen_bits[i] && V_N[i];
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_r0(const int off_l, const int off_s, const int n_elmts)
{
	for (auto i = 0; i < n_active_paths; i++)
	{
		auto metric = 0.f;
		const auto path = paths[i];
		for (auto j = 0; j < n_elmts; j++)
			metric -= std::min((float)l[path][off_l +j], 0.f);
		metrics[path] += metric;

		// TODO: Remove this fill when rate_0 left nodes are in the patterns
		std::fill(s[path].begin() + off_s, s[path].begin() + off_s + n_elmts, 0);
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_r1(const int r_d, const int off_l, const int off_s, const int n_elmts)
{
	if(r_d == 0)
	{
		update_paths_rep(off_l, off_s, n_elmts);
	}
	else
	{
		for (auto i = 0; i < n_active_paths; i++)
		{
			const auto path = paths[i];
			std::partial_sort(llr_indexes[r_d].begin(), llr_indexes[r_d].begin() + 2, llr_indexes[r_d].end(),
				[this, path, off_l](int x, int y) {
					return std::abs(l[path][off_l + x]) < std::abs(l[path][off_l + y]);
				});

			bit_flips[4 * path   ] = llr_indexes[r_d][0];
			bit_flips[4 * path +1] = llr_indexes[r_d][1];

			metrics_vec[1][4 * path   ] = metrics[path];
			metrics_vec[1][4 * path +1] = metrics[path] + std::abs(l[path][off_l + bit_flips[4 * path   ]]);
			metrics_vec[1][4 * path +2] = metrics[path] + std::abs(l[path][off_l + bit_flips[4 * path +1]]);
			metrics_vec[1][4 * path +3] = metrics[path] + std::abs(l[path][off_l + bit_flips[4 * path +1]])
			                                            + std::abs(l[path][off_l + bit_flips[4 * path   ]]);
		}
		for (auto i = n_active_paths; i < L; i++)
			for (auto j = 0 ; j < 4 ; j++)
				metrics_vec[1][4 * paths[i] +j] = std::numeric_limits<float>::max();

		std::partial_sort(metrics_idx[1].begin(), metrics_idx[1].begin() + L, metrics_idx[1].end(),
			[this](int x, int y) {
				return metrics_vec[1][x] < metrics_vec[1][y];
			});

		// TODO: disable this code
		// L first of the lists are the L best paths
		for (auto i = 0; i < L; i++)
			dup_count[metrics_idx[1][i] / 4]++;

	//	// TODO: enable this code
	//	// L first of the lists are the L best paths
	//	for (auto i = 0; i < n_active_paths; i++)
	//		dup_count[metrics_idx[1][paths[i]] / 4]++;

		// erase paths
		auto k = 0;
		auto n_active_paths_cpy = n_active_paths;
		for (auto i = 0; i < n_active_paths_cpy; i++)
			if (dup_count[paths[k]] == 0)
				delete_path(k);
			else
				k++;

		// duplicate paths
		n_active_paths_cpy = n_active_paths;
		for (auto i = 0; i < n_active_paths_cpy; i++)
		{
			const auto path = paths[i];
			if (dup_count[path])
			{
				API_polar::h(s[path], l[path], off_l, off_s, n_elmts);
				for (auto dup = 2; dup <= dup_count[path]; dup++)
					flip_bits_r1(path, duplicate_tree(path, off_l, off_s, n_elmts), dup, off_s, n_elmts);
				dup_count[path] = 0;
			}
		}
		std::fill(dup_count.begin(), dup_count.end(), 0); // TODO: remove this fill
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_rep(const int off_l, const int off_s, const int n_elmts)
{
	// metrics vec used to store values of hypothetic path metrics
	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path = paths[i];
		auto metric1 = metrics[path];
		auto metric2 = metrics[path];
		for (auto j = 0; j < n_elmts; j++)
		{
			metric1 -= std::min(l[path][off_l +j], (R)0);
			metric2 += std::max(l[path][off_l +j], (R)0);
		}
		metrics_vec[0][2 * path   ] = metric1;
		metrics_vec[0][2 * path +1] = metric2;
	}

	if (n_active_paths <= L / 2)
	{
		const auto n_active_paths_cpy = n_active_paths;
		for (auto i = 0; i < n_active_paths_cpy; i++)
			flip_bits_rep(paths[i], duplicate_tree(paths[i], off_l, off_s, n_elmts), off_s, n_elmts);
	}
	else // n_active_paths = L
	{
		// sort hypothetic metrics
		std::sort(metrics_idx[0].begin(), metrics_idx[0].end(),
			[this](int x, int y) {
				return metrics_vec[0][x] < metrics_vec[0][y];
			});

		for (auto i = 0; i < L; i++)
			dup_count[metrics_idx[0][i] / 2]++;

		// erase paths
		auto k = 0;
		for (auto i = 0; i < L; i++)
			if (dup_count[paths[k]] == 0)
				delete_path(k);
			else
				k++;

		for (auto path = 0; path < L; path++)
		{
			if (dup_count[path])
			{
				if (dup_count[path] == 1)
				{
					const auto comp = metrics_vec[0][2 * path] > metrics_vec[0][2 * path +1];
					std::fill(s[path].begin() + off_s, s[path].begin() + off_s + n_elmts, comp ? bit_init<B>() : 0);
					metrics[path] = metrics_vec[0][2*path + (comp ? 1 : 0)];
				}
				else
					flip_bits_rep(path, duplicate_tree(path, off_l, off_s, n_elmts), off_s, n_elmts);

				dup_count[path] = 0;
			}
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_spc(const int r_d, const int off_l, const int off_s, const int n_elmts)
{
	const auto n_candidates = (L <= 2 ? 2 : 8);

	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path = paths[i];

		std::partial_sort(llr_indexes[r_d].begin(), llr_indexes[r_d].begin() + (L <= 2 ? 2 : 4), llr_indexes[r_d].end(),
			[this, path, off_l](int x, int y){
				return std::abs(l[path][off_l + x]) < std::abs(l[path][off_l + y]);
			});

		for (auto i = 0; i < 4; i++)
			bit_flips[4 * path +i] = llr_indexes[r_d][i];

		is_even[path] = !API_polar::spc(s[path], l[path], off_l, off_s, n_elmts);

		metrics_vec[2][n_candidates * path +0] = metrics[path]       +
		                                         (1 - is_even[path]) *
		                                         std::abs(l[path][off_l + bit_flips[4 * path]]);

		metrics_vec[2][n_candidates * path +1] = metrics_vec[2][n_candidates * path]            +
		                                         is_even[path]                                  *
		                                         std::abs(l[path][off_l + bit_flips[4 * path]]) +
		                                         std::abs(l[path][off_l + bit_flips[4 * path +1]]);
		if (L > 2)
		{
			metrics_vec[2][n_candidates * path +2] = metrics_vec[2][n_candidates * path]               +
			                                         is_even[path]                                     *
			                                         std::abs(l[path][off_l + bit_flips[4 * path]])    +
			                                         std::abs(l[path][off_l + bit_flips[4 * path +2]]);

			metrics_vec[2][n_candidates * path +3] = metrics_vec[2][n_candidates * path]               +
			                                         is_even[path]                                     *
			                                         std::abs(l[path][off_l + bit_flips[4 * path]])    +
			                                         std::abs(l[path][off_l + bit_flips[4 * path +3]]);

			metrics_vec[2][n_candidates * path +4] = metrics_vec[2][n_candidates * path]               +
			                                         std::abs(l[path][off_l + bit_flips[4 * path +1]]) +
			                                         std::abs(l[path][off_l + bit_flips[4 * path +2]]);

			metrics_vec[2][n_candidates * path +5] = metrics_vec[2][n_candidates * path]               +
			                                         std::abs(l[path][off_l + bit_flips[4 * path +1]]) +
			                                         std::abs(l[path][off_l + bit_flips[4 * path +3]]);

			metrics_vec[2][n_candidates * path +6] = metrics_vec[2][n_candidates * path]               +
			                                         std::abs(l[path][off_l + bit_flips[4 * path +2]]) +
			                                         std::abs(l[path][off_l + bit_flips[4 * path +3]]);

			metrics_vec[2][n_candidates * path +7] = metrics_vec[2][n_candidates * path +1]            +
			                                         std::abs(l[path][off_l + bit_flips[4 * path +2]]) +
			                                         std::abs(l[path][off_l + bit_flips[4 * path +3]]);
		}
	}

	std::partial_sort(metrics_idx[2].begin(), metrics_idx[2].begin() + L, metrics_idx[2].end(),
		[this](int x, int y){
			return metrics_vec[2][x] < metrics_vec[2][y];
		});

	// L first of the lists are the L best paths
	for (auto i = 0; i < L; i++)
		dup_count[metrics_idx[2][i] / n_candidates]++;

	// erase paths
	auto k = 0;
	auto n_active_paths_cpy = n_active_paths;
	for (auto i = 0; i < n_active_paths_cpy; i++)
		if (dup_count[paths[k]] == 0)
			delete_path(k);
		else
			k++;

	// duplicate paths
	n_active_paths_cpy = n_active_paths;
	for (auto i = 0; i < n_active_paths_cpy; i++)
	{
		const auto path = paths[i];
		if (dup_count[path])
		{
			metrics[path] = metrics_vec[2][n_candidates * path];
			for (auto dup = 2; dup <= dup_count[path]; dup++)
				flip_bits_spc(path, duplicate_tree(path, off_l, off_s, n_elmts), dup, off_s, n_elmts);
			dup_count[path] = 0;
		}
	}
	std::fill(dup_count.begin(), dup_count.end(), 0); // TODO: remove this fill
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::flip_bits_r1(const int old_path, const int new_path, const int dup, const int off_s, const int n_elmts)
{
	constexpr B b = bit_init<B>();

	//TODO: use bit flips vector in std
	//TODO: the case where n_elmts == 1 doesn't exist when REP nodes and R1 nodes are in the patterns. To be removed in a final version ?
	if (n_elmts == 1)
	{
		s[new_path][off_s] = b; metrics[new_path] = metrics_vec[0][2 * old_path + 1];
		s[old_path][off_s] = 0; metrics[old_path] = metrics_vec[0][2 * old_path    ];
	}
	else
	{
		metrics[new_path] = metrics_vec[1][4 * old_path + dup -1];
		switch (dup)
		{
		case 2:
			s[new_path][off_s + bit_flips[4 * old_path   ]] = !s[old_path][off_s + bit_flips[4 * old_path   ]] ? b : 0;
			break;
		case 3:
			s[new_path][off_s + bit_flips[4 * old_path +1]] = !s[old_path][off_s + bit_flips[4 * old_path +1]] ? b : 0;
			break;
		case 4:
			s[new_path][off_s + bit_flips[4 * old_path   ]] = !s[old_path][off_s + bit_flips[4 * old_path   ]] ? b : 0;
			s[new_path][off_s + bit_flips[4 * old_path +1]] = !s[old_path][off_s + bit_flips[4 * old_path +1]] ? b : 0;
			break;
		default:
			std::cout << bold_red("(EE) Flip bits error on rate 1 node.") << std::endl;
			std::exit(-1);
			break;
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::flip_bits_rep(const int old_path, const int new_path, const int off_s, const int n_elmts)
{
	constexpr B b = bit_init<B>();

	std::fill(s[new_path].begin() + off_s, s[new_path].begin() + off_s + n_elmts, b);
	std::fill(s[old_path].begin() + off_s, s[old_path].begin() + off_s + n_elmts, 0);

	metrics[new_path] = metrics_vec[0][2 * old_path +1];
	metrics[old_path] = metrics_vec[0][2 * old_path   ];
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::flip_bits_spc(const int old_path, const int new_path, const int dup, const int off_s, const int n_elmts)
{
	constexpr B b = bit_init<B>();

	metrics[new_path] = metrics_vec[2][(L <= 2 ? 2 : 8) * old_path + dup -1];
	switch(dup)
	{
	case 2 :
		s[new_path][off_s + bit_flips[4 * old_path   ]] = !s[old_path][off_s + bit_flips[4 * old_path   ]] ? b : 0;
		s[new_path][off_s + bit_flips[4 * old_path +1]] = !s[old_path][off_s + bit_flips[4 * old_path +1]] ? b : 0;
		break;
	case 3 :
		s[new_path][off_s + bit_flips[4 * old_path   ]] = !s[old_path][off_s + bit_flips[4 * old_path   ]] ? b : 0;
		s[new_path][off_s + bit_flips[4 * old_path +2]] = !s[old_path][off_s + bit_flips[4 * old_path +2]] ? b : 0;
		break;
	case 4 :
		s[new_path][off_s + bit_flips[4 * old_path   ]] = !s[old_path][off_s + bit_flips[4 * old_path   ]] ? b : 0;
		s[new_path][off_s + bit_flips[4 * old_path +3]] = !s[old_path][off_s + bit_flips[4 * old_path +3]] ? b : 0;
		break;
	case 5 :
		s[new_path][off_s + bit_flips[4 * old_path +1]] = !s[old_path][off_s + bit_flips[4 * old_path +1]] ? b : 0;
		s[new_path][off_s + bit_flips[4 * old_path +2]] = !s[old_path][off_s + bit_flips[4 * old_path +2]] ? b : 0;
		break;
	case 6 :
		s[new_path][off_s + bit_flips[4 * old_path +1]] = !s[old_path][off_s + bit_flips[4 * old_path +1]] ? b : 0;
		s[new_path][off_s + bit_flips[4 * old_path +3]] = !s[old_path][off_s + bit_flips[4 * old_path +3]] ? b : 0;
		break;
	case 7 :
		s[new_path][off_s + bit_flips[4 * old_path +2]] = !s[old_path][off_s + bit_flips[4 * old_path +2]] ? b : 0;
		s[new_path][off_s + bit_flips[4 * old_path +3]] = !s[old_path][off_s + bit_flips[4 * old_path +3]] ? b : 0;
		break;
	case 8 :
		s[new_path][off_s + bit_flips[4 * old_path   ]] = !s[old_path][off_s + bit_flips[4 * old_path   ]] ? b : 0;
		s[new_path][off_s + bit_flips[4 * old_path +1]] = !s[old_path][off_s + bit_flips[4 * old_path +1]] ? b : 0;
		s[new_path][off_s + bit_flips[4 * old_path +2]] = !s[old_path][off_s + bit_flips[4 * old_path +2]] ? b : 0;
		s[new_path][off_s + bit_flips[4 * old_path +3]] = !s[old_path][off_s + bit_flips[4 * old_path +3]] ? b : 0;
		break;
	default:
		std::cout << bold_red("(EE) Flip bits error on SPC node.") << std::endl;
		std::exit(-1);
		break;
	}
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_fast_sys<B,R,API_polar>
::duplicate_tree(const int old_path, const int off_l, const int off_s, const int n_elmts)
{
	const auto new_path = paths[n_active_paths++];

	std::copy(path_2_array[old_path].begin(), path_2_array[old_path].end(), path_2_array[new_path].begin());

	for (auto i = 0; i < m; i++)
		n_array_ref[path_2_array[new_path][i]][i]++;

	std::copy(s[old_path].begin(), s[old_path].begin() + off_s + n_elmts, s[new_path].begin());
	std::copy(l[old_path].begin(), l[old_path].begin() + off_l          , l[new_path].begin());

	return new_path;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::delete_path(int path_id)
{
	for (auto i = 0; i < m; i++)
	{
		n_array_ref[path_2_array[paths[path_id]][i]][i]--;
		//TODO: remove, debug only
		assert(n_array_ref[paths[path_id]][i] >= 0);
	}

	const auto old_path = paths[path_id];
	paths[path_id] = paths[--n_active_paths];
	paths[n_active_paths] = old_path;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::select_best_path()
{
	best_path = -1;
	for (auto i = 0; i < n_active_paths; i++)
		if (best_path == -1 || metrics[paths[i]] < metrics[best_path])
			best_path = paths[i];

	if (best_path == -1)
		best_path = 0;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::allocate_array(const int path, const int r_d)
{
	// if more than 1 path points to the array
	if (n_array_ref[path_2_array[path][r_d]][r_d] > 1)
	{
		// allocate new array to given path, r_d
		auto i = 0;
		n_array_ref[path_2_array[path][r_d]][r_d]--;
		while(n_array_ref[i][r_d])
		{
			i++;
			assert(i < L);
		}
		path_2_array[path_2_array[path][r_d]][r_d] = i -1;
		n_array_ref[i -1][r_d]++;
	}
}
