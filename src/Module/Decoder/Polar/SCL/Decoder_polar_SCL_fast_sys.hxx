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

#include "../SC/Patterns/Pattern_SC_standard.hpp"
#include "../SC/Patterns/Pattern_SC_rate0.hpp"
#include "../SC/Patterns/Pattern_SC_rate1.hpp"
#include "../SC/Patterns/Pattern_SC_rep.hpp"
#include "../SC/Patterns/Pattern_SC_spc.hpp"
#include "../SC/Patterns/Pattern_SC_rate0_left.hpp"
#include "../SC/Patterns/Pattern_SC_rep_left.hpp"

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
                  new Pattern_SC<pattern_SC_type::RATE_0_LEFT>(),
                  new Pattern_SC<pattern_SC_type::RATE_0     >(),
                  new Pattern_SC<pattern_SC_type::RATE_1     >(),
                  new Pattern_SC<pattern_SC_type::REP_LEFT   >(),
                  new Pattern_SC<pattern_SC_type::REP        >()/*,
                  new Pattern_SC<pattern_SC_type::SPC        >()*/}, // perf. degradation with SPC nodes length > 4
                 2,
                 3),
  paths         (L),
  last_paths    (L),
  metrics       (L),
  l             (L, mipp::vector<R>(2 * N + mipp::nElReg<R>())),
  s             (L, mipp::vector<B>(N     + mipp::nElReg<B>())),
  metrics_vec   (3, std::vector<float>()),
  metrics_idx   (3, std::vector<int  >()),
  dup_count     (L, 0),
  llr_indexes   (),
  bit_flips     (4 * L),
  is_even       (L),
  depth2offl    (m +1),
  best_path     (0),
  n_active_paths(1),
  off_s         (0)
{
	static_assert(API_polar::get_n_frames() == 1, "The inter-frame API_polar is not supported.");
	static_assert(sizeof(B) == sizeof(R), "Sizes of the bits and reals have to be identical.");

	// initialize depth2offl lut
	auto index = 0;
	depth2offl[index++] = 0;
	for (auto i = N; i > (N >> m); i >>= 1)
	{
		depth2offl[index] = depth2offl[index - 1] + i;
		index++;
	}

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

	std::copy(Y_N.begin(), Y_N.end(), l[0].begin());
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::inte(const int r_d, const int path, const int node_id)
{
	const auto n_elmts = 1 << r_d;
	const auto n_elm_2 = n_elmts >> 1;
	const auto off_l = depth2offl[m - r_d];

	switch (this->polar_patterns.get_node_type(node_id))
	{
		case STANDARD:
			API_polar::g (s[path],l[path], off_l, off_l + n_elm_2, off_s - n_elm_2, off_l + n_elmts, n_elm_2); break;
		case RATE_0_LEFT:
			API_polar::g0(        l[path], off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2);                ; break;
		case REP_LEFT:
			API_polar::gr(s[path],l[path], off_l, off_l + n_elm_2, off_s - n_elm_2, off_l + n_elmts, n_elm_2); break;
		default:
			std::cout << bold_red("(EE) Something went wrong in the \"inte\" method.") << std::endl;
			std::exit(-1);
			break;
	}

	this->rec_left(r_d -1, path);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::rec_left(const int r_d, const int path)
{
	const auto n_elmts = 1 << r_d;
	const auto n_elm_2 = n_elmts >> 1;
	const auto off_l = depth2offl[m - r_d];

	if (r_d > this->polar_patterns.get_leaf_rev_depth(off_s))
	{
		API_polar::f(l[path], off_l + n_elm_2, off_l, off_l + n_elmts, n_elm_2);
		this->rec_left(r_d -1, path);
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::hard_decode()
{
	auto leaf_id = 0;
	off_s = 0;

	// decode first branch all left (applying f till leaf)
	this->rec_left(m, 0);
	this->update_paths(this->polar_patterns.get_leaf_to_node_id(leaf_id++));
	auto node_id_sums = 0;
	this->recursive_compute_sums(0, 1, 0, node_id_sums);
	off_s += 1 << this->polar_patterns.get_leaf_rev_depth(0);

	// decode all branches, applying g once and f till the leaf
	while (off_s < this->N)
	{
		const auto node_id = this->polar_patterns.get_leaf_to_g_node_id(leaf_id);
		const auto r_d = compute_depth(off_s, m) +1;
		for (auto i = 0; i < this->n_active_paths; i++)
			this->inte(r_d, paths[i], node_id);

		this->update_paths(this->polar_patterns.get_leaf_to_node_id(leaf_id));

		const auto node_id_sums = this->polar_patterns.get_leaf_to_node_id_sums(leaf_id);
		const auto r_d_sums = polar_patterns.get_leaf_rev_depth(off_s) +1;
		for (auto i = 0; i < this->n_active_paths; i++)
			this->recursive_compute_sums(off_s, r_d_sums, paths[i], node_id_sums);

		off_s += 1 << this->polar_patterns.get_leaf_rev_depth(off_s);
		leaf_id++;
	}

	this->select_best_path();
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() >= (unsigned) this->K);

	auto k = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i])
			V_K[k++] = this->s[best_path][i] ? 1 : 0;
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
	assert(V_N.size() == this->frozen_bits.size());

	for (unsigned i = 0; i < V_N.size(); i++)
		V_N[i] = !this->frozen_bits[i] && V_N[i];
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
::recursive_compute_sums(const int off_s, const int r_d, const int path, int node_id)
{
	const auto n_elm   = 1 << r_d;
	const auto n_elm_2 = n_elm >> 1;

	if (off_s % n_elm != 0) // must sum
	{
		switch (this->polar_patterns.get_node_type_sums(node_id))
		{
			case STANDARD:
				API_polar::xo (this->s[path], off_s - n_elm_2, off_s, off_s - n_elm_2, n_elm_2); break;
			case RATE_0_LEFT:
				API_polar::xo0(this->s[path],                  off_s, off_s - n_elm_2, n_elm_2); break;
			case REP_LEFT:
				API_polar::xo (this->s[path], off_s - n_elm_2, off_s, off_s - n_elm_2, n_elm_2); break;
			default:
				std::cout << bold_red("(EE) Something went wrong in the \"recursive_compute_sums\" method.") << std::endl;
				std::exit(-1);
				break;
		}
		this->recursive_compute_sums(off_s - n_elm_2, r_d +1, path, ++node_id);
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths(int node_id)
{
	switch (this->polar_patterns.get_node_type(node_id))
	{
		case RATE_0: update_paths_r0 (       ); break;
		case REP:    update_paths_rep(node_id); break;
		case RATE_1: update_paths_r1 (node_id); break;
		case SPC:    update_paths_spc(node_id); break;
		default:
			std::cout << bold_red("(EE) Something went wrong in the \"update_paths\" method.") << std::endl;
			std::exit(-1);
			break;
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_r0()
{
	const auto r_d = this->polar_patterns.get_leaf_rev_depth(off_s);
	const auto n_elmts = 1 << r_d;

	for (auto i = 0; i < n_active_paths; i++)
	{
		auto metric = 0.f;
		const auto path = paths[i];
		for (auto j = 0; j < n_elmts; j++)
			metric -= std::min((float)l[path][depth2offl[m - r_d] +j], 0.f);
		metrics[path] += metric;
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_r1(const int node_id)
{
	const auto r_d = this->polar_patterns.get_leaf_rev_depth(off_s);
	const auto off_l = depth2offl[m - r_d];
	const auto n_elmts = 1 << r_d;

	for (auto i = 0; i < this->n_active_paths; i++)
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
	for (auto i = this->n_active_paths; i < L; i++)
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
	auto n_active_paths_cpy = this->n_active_paths;
	for (auto i = 0; i < n_active_paths_cpy; i++)
		if (dup_count[paths[k]] == 0)
			delete_path(k);
		else
			k++;

	// duplicate paths
	n_active_paths_cpy = this->n_active_paths;
	for (auto i = 0; i < n_active_paths_cpy; i++)
	{
		const auto path = paths[i];
		if (dup_count[path])
		{
			API_polar::h(s[path], l[path], off_l, off_s, n_elmts);
			duplicate_path(path, node_id, dup_count[path]);
			dup_count[path] = 0;
		}
	}
	std::fill(dup_count.begin(), dup_count.end(), 0); // TODO: remove this fill
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_rep(const int node_id)
{
	// update metrics for all paths till last bit
	const auto r_d = this->polar_patterns.get_leaf_rev_depth(off_s);
	const auto n_elmts = 1 << r_d;

	// metrics vec used to store values of hypothetic path metrics
	for (auto i = 0; i < this->n_active_paths; i++)
	{
		const auto path = paths[i];
		auto metric1 = metrics[path];
		auto metric2 = metrics[path];
		for (auto j = 0; j < n_elmts; j++)
		{
			metric1 -= std::min(l[path][depth2offl[m - r_d] +j], (R)0);
			metric2 += std::max(l[path][depth2offl[m - r_d] +j], (R)0);
		}
		metrics_vec[0][2 * path   ] = metric1;
		metrics_vec[0][2 * path +1] = metric2;
	}

	if (n_active_paths <= L / 2)
	{
		const auto n_active_paths_cpy = this->n_active_paths;
		for (auto i = 0; i < n_active_paths_cpy; i++)
			duplicate_path(paths[i], node_id);
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
					duplicate_path(path, node_id);

				dup_count[path] = 0;
			}
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_spc(const int node_id)
{
	const auto r_d = this->polar_patterns.get_leaf_rev_depth(off_s);
	const auto off_l = depth2offl[m - r_d];
	const auto n_elmts = 1 << r_d;
	const auto n_candidates = (L <= 2 ? 2 : 8);

	for (auto i = 0; i < this->n_active_paths; i++)
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
	auto n_active_paths_cpy = this->n_active_paths;
	for (auto i = 0; i < n_active_paths_cpy; i++)
		if (dup_count[paths[k]] == 0)
			delete_path(k);
		else
			k++;

	// duplicate paths
	n_active_paths_cpy = this->n_active_paths;
	for (auto i = 0; i < n_active_paths_cpy; i++)
	{
		const auto path = paths[i];
		if (dup_count[path])
		{
			metrics[path] = metrics_vec[2][n_candidates * path];
			duplicate_path(path, node_id, dup_count[path]);
			dup_count[path] = 0;
		}
	}
	std::fill(dup_count.begin(), dup_count.end(), 0); // TODO: remove this fill
}
template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::duplicate_path(int path, const int node_id, const int nb_dup)
{
	for (auto dup = 2; dup <= nb_dup; dup++)
		this->flip_bits(path, this->duplicate_tree(path), node_id, dup);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::flip_bits(const int old_path, const int new_path, const int node_id, const int dup)
{
	const auto r_d = this->polar_patterns.get_leaf_rev_depth(off_s);
	const auto n_elmts = 1 << r_d;
	switch (this->polar_patterns.get_node_type(node_id))
	{
		case RATE_1:
			//TODO: use bit flips vector in std
			if (r_d == 0)
			{
				s[new_path][off_s] = bit_init<B>(); metrics[new_path] = metrics_vec[0][2 * old_path + 1];
				s[old_path][off_s] = 0;             metrics[old_path] = metrics_vec[0][2 * old_path    ];
			}
			else
			{
				metrics[new_path] = metrics_vec[1][4 * old_path + dup -1];
				switch (dup)
				{
					case 2:
						s[new_path][off_s + bit_flips[4 * old_path   ]] = (s[old_path][off_s + bit_flips[4 * old_path   ]] == 0) ? bit_init<B>() : 0;
						break;
					case 3:
						s[new_path][off_s + bit_flips[4 * old_path +1]] = (s[old_path][off_s + bit_flips[4 * old_path +1]] == 0) ? bit_init<B>() : 0;
						break;
					case 4:
						s[new_path][off_s + bit_flips[4 * old_path   ]] = (s[old_path][off_s + bit_flips[4 * old_path   ]] == 0) ? bit_init<B>() : 0;
						s[new_path][off_s + bit_flips[4 * old_path +1]] = (s[old_path][off_s + bit_flips[4 * old_path +1]] == 0) ? bit_init<B>() : 0;
						break;
					default:
						std::cout << bold_red("(EE) Flip bits error on rate 1 node.") << std::endl;
						std::exit(-1);
						break;
				}
			}
			break;
		case SPC:
			metrics[new_path] = metrics_vec[2][(L <= 2 ? 2 : 8) * old_path + dup -1];
			switch(dup)
			{
				case 2 :
					s[new_path][off_s + bit_flips[4 * old_path   ]] = (s[old_path][off_s + bit_flips[4 * old_path   ]] == 0) ? bit_init<B>() : 0;
					s[new_path][off_s + bit_flips[4 * old_path +1]] = (s[old_path][off_s + bit_flips[4 * old_path +1]] == 0) ? bit_init<B>() : 0;
					break;
				case 3 :
					s[new_path][off_s + bit_flips[4 * old_path   ]] = (s[old_path][off_s + bit_flips[4 * old_path   ]] == 0) ? bit_init<B>() : 0;
					s[new_path][off_s + bit_flips[4 * old_path +2]] = (s[old_path][off_s + bit_flips[4 * old_path +2]] == 0) ? bit_init<B>() : 0;
					break;
				case 4 :
					s[new_path][off_s + bit_flips[4 * old_path   ]] = (s[old_path][off_s + bit_flips[4 * old_path   ]] == 0) ? bit_init<B>() : 0;
					s[new_path][off_s + bit_flips[4 * old_path +3]] = (s[old_path][off_s + bit_flips[4 * old_path +3]] == 0) ? bit_init<B>() : 0;
					break;
				case 5 :
					s[new_path][off_s + bit_flips[4 * old_path +1]] = (s[old_path][off_s + bit_flips[4 * old_path +1]] == 0) ? bit_init<B>() : 0;
					s[new_path][off_s + bit_flips[4 * old_path +2]] = (s[old_path][off_s + bit_flips[4 * old_path +2]] == 0) ? bit_init<B>() : 0;
					break;
				case 6 :
					s[new_path][off_s + bit_flips[4 * old_path +1]] = (s[old_path][off_s + bit_flips[4 * old_path +1]] == 0) ? bit_init<B>() : 0;
					s[new_path][off_s + bit_flips[4 * old_path +3]] = (s[old_path][off_s + bit_flips[4 * old_path +3]] == 0) ? bit_init<B>() : 0;
					break;
				case 7 :
					s[new_path][off_s + bit_flips[4 * old_path +2]] = (s[old_path][off_s + bit_flips[4 * old_path +2]] == 0) ? bit_init<B>() : 0;
					s[new_path][off_s + bit_flips[4 * old_path +3]] = (s[old_path][off_s + bit_flips[4 * old_path +3]] == 0) ? bit_init<B>() : 0;
					break;
				case 8 :
					s[new_path][off_s + bit_flips[4 * old_path   ]] = (s[old_path][off_s + bit_flips[4 * old_path   ]] == 0) ? bit_init<B>() : 0;
					s[new_path][off_s + bit_flips[4 * old_path +1]] = (s[old_path][off_s + bit_flips[4 * old_path +1]] == 0) ? bit_init<B>() : 0;
					s[new_path][off_s + bit_flips[4 * old_path +2]] = (s[old_path][off_s + bit_flips[4 * old_path +2]] == 0) ? bit_init<B>() : 0;
					s[new_path][off_s + bit_flips[4 * old_path +3]] = (s[old_path][off_s + bit_flips[4 * old_path +3]] == 0) ? bit_init<B>() : 0;
					break;
				default:
					std::cout << bold_red("(EE) Flip bits error on SPC node.") << std::endl;
					std::exit(-1);
					break;
			}
			break;
		case REP:
			std::fill(s[new_path].begin() + off_s, s[new_path].begin() + off_s + n_elmts, bit_init<B>());
			std::fill(s[old_path].begin() + off_s, s[old_path].begin() + off_s + n_elmts, 0            );

			metrics[new_path] = metrics_vec[0][2 * old_path +1];
			metrics[old_path] = metrics_vec[0][2 * old_path   ];
			break;
		default:
			std::cout << bold_red("(EE) Flip bits error: wrong node type !") << std::endl;
			std::exit(-1);
			break;
	}
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SCL_fast_sys<B,R,API_polar>
::duplicate_tree(const int old_path)
{
	const auto new_path = paths[n_active_paths++];

	const auto r_d = this->polar_patterns.get_leaf_rev_depth(off_s);;
	const auto n_elmts = 1 << r_d;
	const auto off_l = depth2offl[m - r_d];

	std::copy(s[old_path].begin(), s[old_path].begin() + off_s + n_elmts, s[new_path].begin());
	std::copy(l[old_path].begin(), l[old_path].begin() + off_l          , l[new_path].begin());

	return new_path;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::delete_path(int path_id)
{
	const auto old_path = paths[path_id];
	paths[path_id] = paths[--n_active_paths];
	paths[n_active_paths] = old_path;
}
