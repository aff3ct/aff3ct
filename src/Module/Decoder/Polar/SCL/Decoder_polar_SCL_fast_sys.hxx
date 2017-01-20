#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <map>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Perf/Transpose/transpose_selector.h"
#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_spc.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_std.hpp"

#include "Tools/Code/Polar/Pattern_polar_parser.hpp"

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
                 {new Pattern_polar_std,
                  new Pattern_polar_r0,
                  new Pattern_polar_r1,
                  new Pattern_polar_r0_left,
                  new Pattern_polar_rep_left,
                  new Pattern_polar_rep,
                  new Pattern_polar_spc(2,2)}, // perf. degradation with SPC nodes length > 4
                 1,
                 2),
  paths         (L),
  last_paths    (L),
  metrics       (L),
  Y_N           (                   N + mipp::nElReg<R>() ),
  l             (L, mipp::vector<R>(N + mipp::nElReg<R>())),
  s             (L, mipp::vector<B>(N + mipp::nElReg<B>())),
  metrics_vec   (3, std::vector<float>()),
  metrics_idx   (3, std::vector<int  >()),
  dup_count     (L, 0),
  llr_indexes   (),
  bit_flips     (4 * L),
  is_even       (L),
  best_path     (0),
  n_active_paths(1),
  n_array_ref   (L, std::vector<int>(m)),
  path_2_array  (L, std::vector<int>(m))
{
	static_assert(API_polar::get_n_frames() == 1, "The inter-frame API_polar is not supported.");
	static_assert(sizeof(B) == sizeof(R), "Sizes of the bits and reals have to be identical.");

	assert(is_power_of_2(L));

	metrics_vec[0].resize(L * 2);
	metrics_idx[0].resize(L * 2);

	metrics_vec[1].resize(L * 4);
	metrics_idx[1].resize(L * 4);

	metrics_vec[2].resize((L <= 2 ? 4 : 8) * L);
	metrics_idx[2].resize((L <= 2 ? 4 : 8) * L);

	for (auto i = 0; i <= m; i++)
	{
		llr_indexes.push_back(std::vector<int>(std::exp2(i)));
		for (auto j = 0 ; j < (int)llr_indexes[i].size() ; j++)
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
::init_buffers()
{
	std::fill(metrics.begin(), metrics.begin() + L, std::numeric_limits<float>::min());
	for (auto i = 0; i < L; i++) paths[i] = i;
	n_active_paths = 1;

	// at the beginning, path 0 points to array 0
	std::fill(n_array_ref [0].begin(), n_array_ref [0].end(), 1);
	std::fill(path_2_array[0].begin(), path_2_array[0].end(), 0);

	for (auto i = 1; i < L; i++)
		std::fill(n_array_ref[i].begin(), n_array_ref[i].end(), 0);

	for (auto i = 0 ; i <               2  * L ; i++) metrics_idx[0][i] = i;
	for (auto i = 0 ; i <               4  * L ; i++) metrics_idx[1][i] = i;
	for (auto i = 0 ; i < (L <= 2 ? 4 : 8) * L ; i++) metrics_idx[2][i] = i;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::load(const mipp::vector<R>& Y_N)
{
	std::copy(Y_N.begin(), Y_N.begin() + this->N, this->Y_N.begin());
	init_buffers();
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
	const polar_node_t node_type = polar_patterns.get_node_type(node_id);

	const bool is_terminal_pattern = (node_type == polar_node_t::RATE_0) ||
	                                 (node_type == polar_node_t::RATE_1) ||
	                                 (node_type == polar_node_t::REP)    ||
	                                 (node_type == polar_node_t::SPC);

	// root node
	if (rev_depth == m)
	{
		// f
		switch (node_type)
		{
			case STANDARD:
				API_polar::f(Y_N.data(), Y_N.data() + n_elm_2, l[0].data(), n_elm_2);
				break;
			case REP_LEFT:
				API_polar::f(Y_N.data(), Y_N.data() + n_elm_2, l[0].data(), n_elm_2);
				break;
			default:
				break;
		}

		recursive_decode(off_l, off_s, rev_depth -1, ++node_id); // recursive call left

		// g
		switch (node_type)
		{
			case STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path  = paths[i];
					const auto child = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::g (Y_N.data(), Y_N.data() + n_elm_2, s[path].data() + off_s, child, n_elm_2);
				}
				break;
			case RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path  = paths[i];
					const auto child = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::g0(Y_N.data(), Y_N.data() + n_elm_2,                         child, n_elm_2);
				}
				break;
			case REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path  = paths[i];
					const auto child = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::gr(Y_N.data(), Y_N.data() + n_elm_2, s[path].data() + off_s, child, n_elm_2);
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
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2);
				break;
			case RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo0(s[paths[i]],        off_s + n_elm_2, off_s, n_elm_2);
				break;
			case REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2);
				break;
			default:
				break;
		}
	}
	else if (!is_terminal_pattern && rev_depth) // other node (not root or leaf)
	{
		// f
		switch (node_type)
		{
			case STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::f(parent + off_l, parent + off_l + n_elm_2, child + off_l + n_elmts, n_elm_2);
				}
				break;
			case REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::f(parent + off_l, parent + off_l + n_elm_2, child + off_l + n_elmts, n_elm_2);
				}
				break;
			case RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths && n_active_paths > 1; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::f(parent + off_l, parent + off_l + n_elm_2, child + off_l + n_elmts, n_elm_2);
				}
				break;
			default:
				break;
		}

		recursive_decode(off_l + n_elmts, off_s, rev_depth -1, ++node_id); // recursive call left

		// g
		switch (node_type)
		{
			case STANDARD:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::g (parent + off_l, parent + off_l + n_elm_2, s[path].data() + off_s, child + off_l + n_elmts, n_elm_2);
				}
				break;
			case RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::g0(parent + off_l, parent + off_l + n_elm_2,                         child + off_l + n_elmts, n_elm_2);
				}
				break;
			case REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
				{
					const auto path   = paths[i];
					const auto parent = l[path_2_array    [path][rev_depth   ]].data();
					const auto child  = l[up_ref_array_idx(path, rev_depth -1)].data();
					API_polar::gr(parent + off_l, parent + off_l + n_elm_2, s[path].data() + off_s, child + off_l + n_elmts, n_elm_2);
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
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2);
				break;
			case RATE_0_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo0(s[paths[i]],        off_s + n_elm_2, off_s, n_elm_2);
				break;
			case REP_LEFT:
				for (auto i = 0; i < n_active_paths; i++)
					API_polar::xo (s[paths[i]], off_s, off_s + n_elm_2, off_s, n_elm_2);
				break;
			default:
				break;
		}
	}
	else // leaf node
	{
		// h
		switch (node_type)
		{
			case RATE_0: update_paths_r0 (rev_depth, off_l, off_s, n_elmts); break;
			case REP:    update_paths_rep(rev_depth, off_l, off_s, n_elmts); break;
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
::update_paths_r0(const int r_d, const int off_l, const int off_s, const int n_elmts)
{
	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path  = paths[i];
		const auto array = path_2_array[path][r_d];

		auto metric = 0.f;
		for (auto j = 0; j < n_elmts; j++)
			metric -= std::min((float)l[array][off_l +j], 0.f);
		metrics[path] += metric; // add a penalty to the current path metric

		// TODO: Remove this fill when rate_0 left nodes are in the patterns
		std::fill(s[path].begin() + off_s, s[path].begin() + off_s + n_elmts, 0);
	}
}

template <typename B, typename R, class API_polar>
template <int REV_D, int N_ELMTS>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_r0(const int off_l, const int off_s)
{
	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path  = paths[i];
		const auto array = path_2_array[path][REV_D];

		auto metric = 0.f;
		for (auto j = 0; j < N_ELMTS; j++)
			metric -= std::min((float)l[array][off_l +j], 0.f);
		metrics[path] += metric; // add a penalty to the current path metric

		// TODO: Remove this fill when rate_0 left nodes are in the patterns
		std::fill(s[path].begin() + off_s, s[path].begin() + off_s + N_ELMTS, 0);
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_r1(const int r_d, const int off_l, const int off_s, const int n_elmts)
{
	if (r_d == 0)
		update_paths_rep(r_d, off_l, off_s, n_elmts);
	else
	{
		for (auto i = 0; i < n_active_paths; i++)
		{
			const auto path  = paths[i];
			const auto array = path_2_array[path][r_d];

			std::partial_sort(llr_indexes[r_d].begin(), llr_indexes[r_d].begin() + 2, llr_indexes[r_d].end(),
				[this, array, off_l](int x, int y) {
					return std::abs(l[array][off_l + x]) < std::abs(l[array][off_l + y]);
				});

			bit_flips[2 * path +0] = llr_indexes[r_d][0];
			bit_flips[2 * path +1] = llr_indexes[r_d][1];

			metrics_vec[1][4 * path +0] = metrics[path];
			metrics_vec[1][4 * path +1] = metrics[path] + std::abs(l[array][off_l + bit_flips[2 * path   ]]);
			metrics_vec[1][4 * path +2] = metrics[path] + std::abs(l[array][off_l + bit_flips[2 * path +1]]);
			metrics_vec[1][4 * path +3] = metrics[path] + std::abs(l[array][off_l + bit_flips[2 * path +1]])
			                                            + std::abs(l[array][off_l + bit_flips[2 * path   ]]);
		}
		for (auto i = n_active_paths; i < L; i++)
			for (auto j = 0; j < 4; j++)
				metrics_vec[1][4 * paths[i] +j] = std::numeric_limits<float>::max();

		std::partial_sort(metrics_idx[1].begin(), metrics_idx[1].begin() + L, metrics_idx[1].begin() + L * 4,
			[this](int x, int y) {
				return metrics_vec[1][x] < metrics_vec[1][y];
			});

		// L first of the lists are the L best paths
		for (auto i = 0; i < L; i++)
			dup_count[metrics_idx[1][i] / 4]++;

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
			const auto path  = paths[i];
			const auto array = path_2_array[path][r_d];
			if (dup_count[path])
			{
				API_polar::h(s[path], l[array], off_l, off_s, n_elmts);
				for (auto dup = 2; dup <= dup_count[path]; dup++)
					flip_bits_r1(path, duplicate_tree(path, off_l, off_s, n_elmts), dup, off_s, n_elmts);
				dup_count[path] = 0;
			}
		}
		std::fill(dup_count.begin(), dup_count.begin() + L, 0);
	}
}

template <typename B, typename R, class API_polar>
template <int REV_D, int N_ELMTS>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_r1(const int off_l, const int off_s)
{
	if (REV_D == 0)
		update_paths_rep<REV_D, N_ELMTS>(off_l, off_s);
	else
	{
		for (auto i = 0; i < n_active_paths; i++)
		{
			const auto path  = paths[i];
			const auto array = path_2_array[path][REV_D];

			std::partial_sort(llr_indexes[REV_D].begin(), llr_indexes[REV_D].begin() + 2, llr_indexes[REV_D].end(),
				[this, array, off_l](int x, int y) {
					return std::abs(l[array][off_l + x]) < std::abs(l[array][off_l + y]);
				});

			bit_flips[2 * path +0] = llr_indexes[REV_D][0];
			bit_flips[2 * path +1] = llr_indexes[REV_D][1];

			metrics_vec[1][4 * path +0] = metrics[path];
			metrics_vec[1][4 * path +1] = metrics[path] + std::abs(l[array][off_l + bit_flips[2 * path   ]]);
			metrics_vec[1][4 * path +2] = metrics[path] + std::abs(l[array][off_l + bit_flips[2 * path +1]]);
			metrics_vec[1][4 * path +3] = metrics[path] + std::abs(l[array][off_l + bit_flips[2 * path +1]])
			                                            + std::abs(l[array][off_l + bit_flips[2 * path   ]]);
		}
		for (auto i = n_active_paths; i < L; i++)
			for (auto j = 0; j < 4; j++)
				metrics_vec[1][4 * paths[i] +j] = std::numeric_limits<float>::max();

		std::partial_sort(metrics_idx[1].begin(), metrics_idx[1].begin() + L, metrics_idx[1].begin() + L * 4,
			[this](int x, int y) {
				return metrics_vec[1][x] < metrics_vec[1][y];
			});

		// L first of the lists are the L best paths
		for (auto i = 0; i < L; i++)
			dup_count[metrics_idx[1][i] / 4]++;

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
			const auto path  = paths[i];
			const auto array = path_2_array[path][REV_D];
			if (dup_count[path])
			{
				API_polar::template h<N_ELMTS>(s[path], l[array], off_l, off_s, N_ELMTS);
				for (auto dup = 2; dup <= dup_count[path]; dup++)
					flip_bits_r1(path, duplicate_tree(path, off_l, off_s, N_ELMTS), dup, off_s, N_ELMTS);
				dup_count[path] = 0;
			}
		}
		std::fill(dup_count.begin(), dup_count.begin() + L, 0);
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::flip_bits_r1(const int old_path, const int new_path, const int dup, const int off_s, const int n_elmts)
{
	constexpr B b = bit_init<B>();

	metrics[new_path] = metrics_vec[1][4 * old_path + dup -1];
	switch (dup)
	{
	case 2:
		s[new_path][off_s + bit_flips[2 * old_path +0]] = !s[old_path][off_s + bit_flips[2 * old_path +0]] ? b : 0;
		break;
	case 3:
		s[new_path][off_s + bit_flips[2 * old_path +1]] = !s[old_path][off_s + bit_flips[2 * old_path +1]] ? b : 0;
		break;
	case 4:
		s[new_path][off_s + bit_flips[2 * old_path +0]] = !s[old_path][off_s + bit_flips[2 * old_path +0]] ? b : 0;
		s[new_path][off_s + bit_flips[2 * old_path +1]] = !s[old_path][off_s + bit_flips[2 * old_path +1]] ? b : 0;
		break;
	default:
		std::cout << bold_red("(EE) Flip bits error on rate 1 node.") << std::endl;
		std::exit(-1);
		break;
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_rep(const int r_d, const int off_l, const int off_s, const int n_elmts)
{
	constexpr B b = bit_init<B>();

	// metrics vec used to store values of hypothetic path metrics
	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path  = paths[i];
		const auto array = path_2_array[path][r_d];

		auto metric1 = metrics[path];
		auto metric2 = metrics[path];
		for (auto j = 0; j < n_elmts; j++)
		{
			metric1 -= std::min(l[array][off_l +j], (R)0);
			metric2 += std::max(l[array][off_l +j], (R)0);
		}
		metrics_vec[0][2 * path +0] = metric1;
		metrics_vec[0][2 * path +1] = metric2;
	}

	if (n_active_paths <= L / 2)
	{
		const auto n_active_paths_cpy = n_active_paths;
		for (auto i = 0; i < n_active_paths_cpy; i++)
			flip_bits_rep(paths[i], duplicate_tree(paths[i], off_l, off_s, n_elmts), off_s, n_elmts);
	}
	else // n_active_paths == L
	{
		// sort hypothetic metrics
		std::sort(metrics_idx[0].begin(), metrics_idx[0].begin() + L * 2,
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
					const auto comp = metrics_vec[0][2 * path +0] > metrics_vec[0][2 * path +1];
					std::fill(s[path].begin() + off_s, s[path].begin() + off_s + n_elmts, comp ? b : 0);
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
template <int REV_D, int N_ELMTS>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_rep(const int off_l, const int off_s)
{
	constexpr B b = bit_init<B>();

	// metrics vec used to store values of hypothetic path metrics
	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path  = paths[i];
		const auto array = path_2_array[path][REV_D];

		auto metric1 = metrics[path];
		auto metric2 = metrics[path];
		for (auto j = 0; j < N_ELMTS; j++)
		{
			metric1 -= std::min(l[array][off_l +j], (R)0);
			metric2 += std::max(l[array][off_l +j], (R)0);
		}
		metrics_vec[0][2 * path +0] = metric1;
		metrics_vec[0][2 * path +1] = metric2;
	}

	if (n_active_paths <= L / 2)
	{
		const auto n_active_paths_cpy = n_active_paths;
		for (auto i = 0; i < n_active_paths_cpy; i++)
			flip_bits_rep(paths[i], duplicate_tree(paths[i], off_l, off_s, N_ELMTS), off_s, N_ELMTS);
	}
	else // n_active_paths == L
	{
		// sort hypothetic metrics
		std::sort(metrics_idx[0].begin(), metrics_idx[0].begin() + L * 2,
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
					const auto comp = metrics_vec[0][2 * path +0] > metrics_vec[0][2 * path +1];
					std::fill(s[path].begin() + off_s, s[path].begin() + off_s + N_ELMTS, comp ? b : 0);
					metrics[path] = metrics_vec[0][2*path + (comp ? 1 : 0)];
				}
				else
					flip_bits_rep(path, duplicate_tree(path, off_l, off_s, N_ELMTS), off_s, N_ELMTS);
				dup_count[path] = 0;
			}
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
	metrics[old_path] = metrics_vec[0][2 * old_path +0];
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_spc(const int r_d, const int off_l, const int off_s, const int n_elmts)
{
	const auto n_cands = L <= 2 ? 4 : 8; // number of candidates

	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path  = paths[i];
		const auto array = path_2_array[paths[i]][r_d];

		std::partial_sort(llr_indexes[r_d].begin(), llr_indexes[r_d].begin() +4, llr_indexes[r_d].end(),
			[this, array, off_l](int x, int y){
				return std::abs(l[array][off_l + x]) < std::abs(l[array][off_l + y]);
			});

		for (auto i = 0; i < 4; i++)
			bit_flips[4 * path +i] = llr_indexes[r_d][i];

//		is_even[path] = !API_polar::spc(s[path], l[array], off_l, off_s, n_elmts);
		auto prod = 1.f;
		for (auto i = 0; i < n_elmts; i++)
			prod *= l[array][off_l +i];
		is_even[path] = prod > 0;

		{
			metrics_vec[2][n_cands * path +0] = metrics[path] +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +0]]) * (!is_even[path]);

			metrics_vec[2][n_cands * path +1] = metrics_vec[2][n_cands * path +0]                                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +0]]) * is_even[path] +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +1]]);

			metrics_vec[2][n_cands * path +2] = metrics_vec[2][n_cands * path +0]                                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +0]]) * is_even[path] +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +2]]);

			metrics_vec[2][n_cands * path +3] = metrics_vec[2][n_cands * path +0]                                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +0]]) * is_even[path] +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +3]]);
		}
		if (L > 2)
		{
			metrics_vec[2][n_cands * path +4] = metrics_vec[2][n_cands * path +0]                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +1]]) +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +2]]);

			metrics_vec[2][n_cands * path +5] = metrics_vec[2][n_cands * path +0]                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +1]]) +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +3]]);

			metrics_vec[2][n_cands * path +6] = metrics_vec[2][n_cands * path +0]                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +2]]) +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +3]]);

			metrics_vec[2][n_cands * path +7] = metrics_vec[2][n_cands * path +1]                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +2]]) +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +3]]);
		}
	}
	for (auto i = n_active_paths; i < L; i++)
		for (auto j = 0; j < n_cands; j++)
			metrics_vec[2][n_cands * paths[i] +j] = std::numeric_limits<float>::max();

	std::partial_sort(metrics_idx[2].begin(), metrics_idx[2].begin() + L, metrics_idx[2].begin() + n_cands * L,
		[this](int x, int y){
			return metrics_vec[2][x] < metrics_vec[2][y];
		});

	// L first of the lists are the L best paths
	for (auto i = 0; i < L; i++)
		dup_count[metrics_idx[2][i] / n_cands]++;

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
			const auto array = path_2_array[path][r_d];
			API_polar::h(s[path], l[array], off_l, off_s, n_elmts);

			metrics[path] = metrics_vec[2][n_cands * path];
			for (auto dup = 2; dup <= dup_count[path]; dup++)
				flip_bits_spc(path, duplicate_tree(path, off_l, off_s, n_elmts), dup, off_s, n_elmts);

			if (!is_even[path])
				s[path][off_s + bit_flips[4 * path +0]] = s[path][off_s + bit_flips[4 * path +0]] ? 0 : bit_init<B>();

			dup_count[path] = 0;
		}
	}
	std::fill(dup_count.begin(), dup_count.begin() + L, 0);
}

template <typename B, typename R, class API_polar>
template <int REV_D, int N_ELMTS>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_spc(const int off_l, const int off_s)
{
	const auto n_cands = L <= 2 ? 4 : 8; // number of candidates

	for (auto i = 0; i < n_active_paths; i++)
	{
		const auto path  = paths[i];
		const auto array = path_2_array[paths[i]][REV_D];

		std::partial_sort(llr_indexes[REV_D].begin(), llr_indexes[REV_D].begin() +4, llr_indexes[REV_D].end(),
			[this, array, off_l](int x, int y){
				return std::abs(l[array][off_l + x]) < std::abs(l[array][off_l + y]);
			});

		for (auto i = 0; i < 4; i++)
			bit_flips[4 * path +i] = llr_indexes[REV_D][i];

//		is_even[path] = !API_polar::template spc<N_ELMTS>(s[path], l[array], off_l, off_s, N_ELMTS);
		auto prod = 1.f;
		for (auto i = 0; i < N_ELMTS; i++)
			prod *= l[array][off_l +i];
		is_even[path] = prod > 0;

		{
			metrics_vec[2][n_cands * path +0] = metrics[path] +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +0]]) * (!is_even[path]);

			metrics_vec[2][n_cands * path +1] = metrics_vec[2][n_cands * path +0]                                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +0]]) * is_even[path] +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +1]]);

			metrics_vec[2][n_cands * path +2] = metrics_vec[2][n_cands * path +0]                                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +0]]) * is_even[path] +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +2]]);

			metrics_vec[2][n_cands * path +3] = metrics_vec[2][n_cands * path +0]                                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +0]]) * is_even[path] +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +3]]);
		}
		if (L > 2)
		{
			metrics_vec[2][n_cands * path +4] = metrics_vec[2][n_cands * path +0]                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +1]]) +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +2]]);

			metrics_vec[2][n_cands * path +5] = metrics_vec[2][n_cands * path +0]                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +1]]) +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +3]]);

			metrics_vec[2][n_cands * path +6] = metrics_vec[2][n_cands * path +0]                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +2]]) +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +3]]);

			metrics_vec[2][n_cands * path +7] = metrics_vec[2][n_cands * path +1]                  +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +2]]) +
			                                    std::abs(l[array][off_l + bit_flips[4 * path +3]]);
		}
	}
	for (auto i = n_active_paths; i < L; i++)
		for (auto j = 0; j < n_cands; j++)
			metrics_vec[2][n_cands * paths[i] +j] = std::numeric_limits<float>::max();

	std::partial_sort(metrics_idx[2].begin(), metrics_idx[2].begin() + L, metrics_idx[2].begin() + n_cands * L,
		[this](int x, int y){
			return metrics_vec[2][x] < metrics_vec[2][y];
		});

	// L first of the lists are the L best paths
	for (auto i = 0; i < L; i++)
		dup_count[metrics_idx[2][i] / n_cands]++;

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
			const auto array = path_2_array[path][REV_D];
			API_polar::h<N_ELMTS>(s[path], l[array], off_l, off_s, N_ELMTS);

			metrics[path] = metrics_vec[2][n_cands * path];
			for (auto dup = 2; dup <= dup_count[path]; dup++)
				flip_bits_spc(path, duplicate_tree(path, off_l, off_s, N_ELMTS), dup, off_s, N_ELMTS);

			if (!is_even[path])
				s[path][off_s + bit_flips[4 * path +0]] = s[path][off_s + bit_flips[4 * path +0]] ? 0 : bit_init<B>();

			dup_count[path] = 0;
		}
	}
	std::fill(dup_count.begin(), dup_count.begin() + L, 0);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::flip_bits_spc(const int old_path, const int new_path, const int dup, const int off_s, const int n_elmts)
{
	constexpr B b = bit_init<B>();

	const auto n_cands = L <= 2 ? 4 : 8; // number of candidates

	metrics[new_path] = metrics_vec[2][n_cands * old_path + dup -1];

	switch(dup)
	{
	case 2 :
		if (is_even[old_path])
			s[new_path][off_s + bit_flips[4 * old_path +0]] = s[old_path][off_s + bit_flips[4 * old_path +0]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +1]] = s[old_path][off_s + bit_flips[4 * old_path +1]] ? 0 : b;
		break;
	case 3 :
		if (is_even[old_path])
			s[new_path][off_s + bit_flips[4 * old_path +0]] = s[old_path][off_s + bit_flips[4 * old_path +0]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +2]] = s[old_path][off_s + bit_flips[4 * old_path +2]] ? 0 : b;
		break;
	case 4 :
		if (is_even[old_path])
			s[new_path][off_s + bit_flips[4 * old_path +0]] = s[old_path][off_s + bit_flips[4 * old_path +0]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +3]] = s[old_path][off_s + bit_flips[4 * old_path +3]] ? 0 : b;
		break;
	case 5 :
		if (!is_even[old_path])
			s[new_path][off_s + bit_flips[4 * old_path +0]] = s[old_path][off_s + bit_flips[4 * old_path +0]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +1]] = s[old_path][off_s + bit_flips[4 * old_path +1]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +2]] = s[old_path][off_s + bit_flips[4 * old_path +2]] ? 0 : b;
		break;
	case 6 :
		if (!is_even[old_path])
			s[new_path][off_s + bit_flips[4 * old_path +0]] = s[old_path][off_s + bit_flips[4 * old_path +0]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +1]] = s[old_path][off_s + bit_flips[4 * old_path +1]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +3]] = s[old_path][off_s + bit_flips[4 * old_path +3]] ? 0 : b;
		break;
	case 7 :
		if (!is_even[old_path])
			s[new_path][off_s + bit_flips[4 * old_path +0]] = s[old_path][off_s + bit_flips[4 * old_path +0]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +2]] = s[old_path][off_s + bit_flips[4 * old_path +2]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +3]] = s[old_path][off_s + bit_flips[4 * old_path +3]] ? 0 : b;
		break;
	case 8 :
		if (is_even[old_path])
			s[new_path][off_s + bit_flips[4 * old_path +0]] = s[old_path][off_s + bit_flips[4 * old_path +0]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +1]] = s[old_path][off_s + bit_flips[4 * old_path +1]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +2]] = s[old_path][off_s + bit_flips[4 * old_path +2]] ? 0 : b;
		s[new_path][off_s + bit_flips[4 * old_path +3]] = s[old_path][off_s + bit_flips[4 * old_path +3]] ? 0 : b;
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

//	std::copy(path_2_array[old_path].begin(), path_2_array[old_path].end(), path_2_array[new_path].begin());
	const auto loop_size = (int)path_2_array[old_path].size();
	for (auto i = 0; i < loop_size; i++)
		path_2_array[new_path][i] = path_2_array[old_path][i];

	for (auto i = 0; i < m; i++)
		n_array_ref[path_2_array[new_path][i]][i]++;

//	std::copy(s[old_path].begin(), s[old_path].begin() + off_s + n_elmts, s[new_path].begin());
	for (auto i = 0; i < off_s + n_elmts; i++)
		s[new_path][i] = s[old_path][i]; // TODO: this line takes a big part of the decoding time!

	return new_path;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::delete_path(int path_id)
{
	const auto old_path = paths[path_id];
	for (auto i = 0; i < m; i++)
		n_array_ref[path_2_array[old_path][i]][i]--;

	paths[path_id] = paths[--n_active_paths];
	paths[n_active_paths] = old_path;
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_fast_sys<B,R,API_polar>
::select_best_path()
{
	best_path = -1;
	for (auto i = 0; i < n_active_paths; i++)
		if (best_path == -1 || metrics[paths[i]] < metrics[best_path])
			best_path = paths[i];

	if (best_path == -1)
		best_path = 0;

	return n_active_paths;
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_fast_sys<B,R,API_polar>
::up_ref_array_idx(const int path, const int r_d)
{
	auto old_array = path_2_array[path][r_d];

	// if more than 1 path points to the array
	if (n_array_ref[old_array][r_d] > 1)
	{
		// allocate new array to given path, r_d
		n_array_ref[old_array][r_d]--;

		auto new_array = 0;
		while (n_array_ref[new_array][r_d]) new_array++;

		path_2_array[path     ][r_d] = new_array;
		n_array_ref [new_array][r_d]++;

		return new_array;
	}

	return old_array;
}
