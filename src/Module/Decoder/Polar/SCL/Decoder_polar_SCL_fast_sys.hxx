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
: Decoder<B,R>     (K, N, n_frames, API_polar::get_n_frames(), name),
  m                (std::log2(N)),
  L                (L),
  metric_init      (std::numeric_limits<float>::min()),
  frozen_bits      (frozen_bits),
  active_paths     (L, false),
  metrics          (L),
  l                (L, mipp::vector<R>(2 * N + mipp::nElReg<R>())),
  s                (L, mipp::vector<B>(N     + mipp::nElReg<B>())),
  metrics_vec      (3, std::vector<float>()),
  metrics_idx      (3, std::vector<int  >()),
  dup_count        (L),
  llr_indexes      (),
  bit_flips        (4 * L),
  last_active_paths(L),
  is_even          (L),
  leaves_rev_depth (N),
  depth2offl       (m +1)
{
	static_assert(API_polar::get_n_frames() == 1, "Only the intra-frame API_polar is supported.");
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

	for (auto i = 0 ; i <= m ; i ++)
	{
		llr_indexes.push_back(std::vector<int>(std::exp2(i)));
		for (size_t j = 0 ; j < llr_indexes[i].size() ; j++)
			llr_indexes[i][j] = j;
	}

	Pattern_SC_interface* pattern_SC_r0 = new Pattern_SC<pattern_SC_type::RATE_0>();
	Pattern_SC_interface* pattern_SC_r1 = new Pattern_SC<pattern_SC_type::RATE_1>();

	std::vector<Pattern_SC_interface*> patterns_SC;
	patterns_SC.push_back(new Pattern_SC<pattern_SC_type::STANDARD   >());
	patterns_SC.push_back(new Pattern_SC<pattern_SC_type::RATE_0_LEFT>());
	patterns_SC.push_back(pattern_SC_r0                                 );
	patterns_SC.push_back(pattern_SC_r1                                 );
	patterns_SC.push_back(new Pattern_SC<pattern_SC_type::REP_LEFT   >());
	patterns_SC.push_back(new Pattern_SC<pattern_SC_type::REP        >());
//	patterns_SC.push_back(new Pattern_SC<pattern_SC_type::SPC        >()); // decoding performance degradation with SPC nodes

	mipp::vector<int> fb_int32(N);
	for (unsigned i = 0; i < frozen_bits.size(); i++)
		fb_int32[i] = (int)frozen_bits[i];

	Pattern_parser_polar *parser = new Pattern_parser_polar(N, fb_int32, patterns_SC, *pattern_SC_r0, *pattern_SC_r1);

	int leaf_index, node_index;
	recursive_get_leaves_depth(parser->get_polar_tree()->get_root(), leaf_index = 0);
	generate_nodes_indexes    (parser->get_polar_tree()->get_root(), node_index = 0);

	delete parser;
	for (unsigned i = 0; i < patterns_SC.size(); i++)
		delete patterns_SC[i];
}

template <typename B, typename R, class API_polar>
Decoder_polar_SCL_fast_sys<B,R,API_polar>
::~Decoder_polar_SCL_fast_sys()
{
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::load(const mipp::vector<R>& Y_N)
{
	std::fill(metrics.begin(), metrics.end(), metric_init);
	std::fill(active_paths.begin(), active_paths.end(), false);

	for (auto i = 0 ; i < 3 ; i++)
		std::fill(metrics_vec[i].begin(), metrics_vec[i].end(), std::numeric_limits<float>::max());

	std::copy(Y_N.begin(), Y_N.end(), l[0].begin());

	active_paths[0] = true;
	n_active_paths = 1;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::inte(const int r_d, const int path, int &id)
{
	const auto n_elmts = 1 << r_d;
	const auto n_elm_2 = n_elmts >> 1;
	const auto off_l = depth2offl[m - r_d];

	switch ((pattern_SC_type)pattern_types_per_id[id])
	{
		case STANDARD:
			API_polar::g (s[path],l[path], off_l, off_l + n_elm_2, off_s - n_elm_2, off_l + n_elmts, n_elm_2); break;
		case RATE_0_LEFT:
			API_polar::g0(        l[path], off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2);                ; break;
		case REP_LEFT:
			API_polar::gr(s[path],l[path], off_l, off_l + n_elm_2, off_s - n_elm_2, off_l + n_elmts, n_elm_2); break;
		default:
			break;
	}

	this->rec_left(r_d -1, path, ++id);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::rec_left(const int r_d, const int path, int &id)
{
	const auto n_elmts = 1 << r_d;
	const auto n_elm_2 = n_elmts >> 1;
	const auto off_l = depth2offl[m - r_d];

	if (r_d > leaves_rev_depth[off_s])
	{
		API_polar::f(l[path], off_l + n_elm_2, off_l, off_l + n_elmts, n_elm_2);
		this->rec_left(r_d -1, path, ++id);
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::hard_decode()
{
	auto id = 0, sums_id = 0, tmp_sums_id = 0, tmp_id = 0;
	off_s = 0;

	// decode first branch all left (applying f till leaf)
	this->rec_left(m, 0, id);
	this->update_paths(id++);
	this->recursive_compute_sums(0, 1, 0, sums_id);
	off_s += 1 << leaves_rev_depth[0];

	// decode all branches, applying g once and f till the leaf
	while (off_s < this->N)
	{
		for (auto path = 0 ; path < this->L ; path++)
			if (active_paths[path])
			{
				tmp_id = id;
				this->inte(compute_depth(off_s, m) + 1, path, tmp_id);
			}

		id = tmp_id;

		this->update_paths(id);

		id++;

		for (auto path = 0 ; path < L ; path++)
			if (active_paths[path])
			{
				tmp_sums_id = sums_id;
				this->recursive_compute_sums(off_s, leaves_rev_depth[off_s] +1, path, tmp_sums_id);
			}

		sums_id = tmp_sums_id;

		off_s += 1 << leaves_rev_depth[off_s];
	}

	this->select_best_path();
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() == (unsigned) this->K);

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
	for (auto path = 0 ; path < L ; path++)
		if (active_paths[path])
			if (best_path == -1 || metrics[path] < metrics[best_path])
				best_path = path;

	if (best_path == -1)
		best_path = 0;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::recursive_compute_sums(const int off_s, const int r_d, const int path, int &id)
{
	const auto n_elm   = 1 << r_d;
	const auto n_elm_2 = n_elm >> 1;

	if (off_s % n_elm != 0) // must sum 
	{
		switch ((pattern_SC_type)pattern_types_per_id_sums[id])
		{
			case STANDARD:
				API_polar::xo (this->s[path], off_s - n_elm_2, off_s, off_s - n_elm_2, n_elm_2); break;
			case RATE_0_LEFT:
				API_polar::xo0(this->s[path],                  off_s, off_s - n_elm_2, n_elm_2); break;
			case REP_LEFT:
				API_polar::xo (this->s[path], off_s - n_elm_2, off_s, off_s - n_elm_2, n_elm_2); break;
			default:
				break;
		}
		this->recursive_compute_sums(off_s - n_elm_2, r_d +1, path, ++id);
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths(int id)
{
	const auto my_pattern_type = (pattern_SC_type)pattern_types_per_id[id];
	switch (my_pattern_type)
	{
		case RATE_0: update_paths_r0 (  ); break;
		case REP:    update_paths_rep(id); break;
		case RATE_1: update_paths_r1 (id); break;
		case SPC:    update_paths_spc(id); break;
		default:
			break;
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_r0()
{
	const auto r_d = leaves_rev_depth[off_s];
	const auto n_elmts = 1 << r_d;

	for (auto i = 0 ; i < n_elmts ; i++)
		for (auto path = 0 ; path < L ; path++)
			if (active_paths[path])
				metrics[path] -= std::min((float)l[path][depth2offl[m - r_d] + i], 0.f);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_r1(const int id)
{
	const auto r_d = leaves_rev_depth[off_s];
	const auto off_l = depth2offl[m - r_d];
	const auto n_elmts = 1 << r_d;

	for (auto path = 0 ; path < L ; path ++)
	{
		if (active_paths[path])
		{
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
		else
			for (auto i = 0 ; i < 4 ; i++)
				metrics_vec[1][4 * path +i] = std::numeric_limits<float>::max();
	}

	std::partial_sort(metrics_idx[1].begin(), metrics_idx[1].begin() + L, metrics_idx[1].end(),
		[this](int x, int y) {
			return metrics_vec[1][x] < metrics_vec[1][y];
		});

	// L first of the lists are the L best paths
	for (auto i = 0 ; i < L ; i++)
		dup_count[metrics_idx[1][i] / 4]++;

	// erase paths
	for (auto path = 0 ; path < L ; path++)
		if (dup_count[path] == 0 && active_paths[path])
			delete_path(path);

	last_active_paths = active_paths;
	for (auto path = 0 ; path < L ; path++)
	{
		if (dup_count[path])
		{
			if (last_active_paths[path])
			{
				API_polar::h(s[path], l[path], off_l, off_s, n_elmts);
				duplicate_path(path, id, dup_count[path]);
			}
			dup_count[path] = 0;
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_rep(const int id)
{
	// update metrics for all paths till last bit 
	const auto r_d = leaves_rev_depth[off_s];
	const auto n_elmts = 1 << r_d;

	for (auto path = 0 ; path < L ; path++)
		if (active_paths[path])
		{
			metrics_vec[0][2 * path   ] = metrics[path];
			metrics_vec[0][2 * path +1] = metrics[path];
		}
	
	// metrics vec used to store values of hypothetic path metrics
	for (auto i = 0 ; i < n_elmts ; i++)
		for (auto path = 0 ; path < L ; path++)
			if (active_paths[path])
			{
				metrics_vec[0][2 * path   ] -= std::min(l[path][depth2offl[m - r_d] + i], (R)0);
				metrics_vec[0][2 * path +1] += std::max(l[path][depth2offl[m - r_d] + i], (R)0);
			}

	if (n_active_paths <= L / 2)
	{
		last_active_paths = active_paths;
		for (auto path = 0 ; path < L ; path++)
			if (last_active_paths[path])
				this->duplicate_path(path, id);
	}
	else
	{
		// sort hypothetic metrics
		std::sort(metrics_idx[0].begin(), metrics_idx[0].end(),
			[this](int x, int y) {
				return metrics_vec[0][x] < metrics_vec[0][y];
			});

		for (auto i = 0 ; i < L ; i++)
			dup_count[metrics_idx[0][i] / 2]++;

		// erase paths
		for (auto path = 0 ; path < L ; path++)
			if (dup_count[path] == 0 && active_paths[path])
				delete_path(path);

		for (auto path = 0 ; path < L ; path++)
			if (dup_count[path])
			{
				if (dup_count[path] == 1)
				{
					if (metrics_vec[0][2 * path] > metrics_vec[0][2 * path +1])
					{
						std::fill(s[path].begin() + off_s, s[path].begin() + off_s + n_elmts, bit_init<B>());
						metrics[path] = metrics_vec[0][2*path +1];	
					}
					else
					{
						std::fill(s[path].begin() + off_s, s[path].begin() + off_s + n_elmts, 0);
						metrics[path] = metrics_vec[0][2*path];
					}

					dup_count[path] = 0;
				}
				else
				{
					duplicate_path(path, id);
					dup_count[path] = 0;
				}
			}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::update_paths_spc(const int id)
{
	const auto r_d = leaves_rev_depth[off_s];
	const auto off_l = depth2offl[m - r_d];
	const auto n_elmts = 1 << r_d;
	const auto n_candidates = (L <= 2 ? 2 : 8);

	for (auto path = 0 ; path < L ; path ++)
		if (active_paths[path])
		{
			std::partial_sort(llr_indexes[r_d].begin(), llr_indexes[r_d].begin() + (L <= 2 ? 2 : 4), llr_indexes[r_d].end(),
				[this, path, off_l](int x, int y){
					return std::abs(l[path][off_l + x]) < std::abs(l[path][off_l + y]);
				});

			for (auto i = 0 ; i < 4 ; i++)
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
	for (auto i = 0 ; i < L ; i++)
		dup_count[metrics_idx[2][i] / n_candidates]++;

	// erase paths
	for (auto path = 0 ; path < L ; path++)
		if (dup_count[path] == 0 && active_paths[path])
			delete_path(path);

	for (auto path = 0 ; path < L ; path++)
	{
		if (dup_count[path])
		{
			metrics[path] = metrics_vec[2][n_candidates * path];
			duplicate_path(path, id, dup_count[path]);
			dup_count[path] = 0;
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::recursive_get_leaves_depth(Binary_node<Pattern_SC_interface>* node_curr, int &leaf_index)
{
	const auto my_pattern_type = node_curr->get_c()->type();

	const bool is_terminal_pattern = (my_pattern_type == pattern_SC_type::RATE_0) ||
	                                 (my_pattern_type == pattern_SC_type::RATE_1) ||
	                                 (my_pattern_type == pattern_SC_type::REP)    ||
	                                 (my_pattern_type == pattern_SC_type::SPC);

	if (!node_curr->is_leaf() && !is_terminal_pattern)
	{
		this->recursive_get_leaves_depth(node_curr->get_left (), leaf_index); // recursive call
		this->recursive_get_leaves_depth(node_curr->get_right(), leaf_index); // recursive call
	}
	else
	{
		leaves_rev_depth[leaf_index] = m - node_curr->get_depth();
		leaf_index += node_curr->get_c()->get_size();
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::generate_nodes_indexes(const Binary_node<Pattern_SC_interface>* node_curr, int& node_index)
{
	pattern_types_per_id.push_back((char)node_curr->get_c()->type());

	if (!node_curr->is_leaf()) // stop condition
	{
		this->generate_nodes_indexes(node_curr->get_left(),  node_index); // recursive call
		pattern_types_per_id.push_back((char)node_curr->get_c()->type());
		this->generate_nodes_indexes(node_curr->get_right(), node_index); // recursive call
		pattern_types_per_id_sums.push_back((char)node_curr->get_c()->type());
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::duplicate_path(int path, const int id, const int nb_dup )
{
	int new_path;

	n_active_paths += nb_dup -1;

	for (auto dup = 2 ; dup <= nb_dup ; dup++)
	{
		duplicate_tree(path, new_path);
		flip_bits(path, new_path, id, dup);
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::flip_bits(const int old_path, const int new_path, const int id, const int dup)
{
	const auto my_pattern_type = (pattern_SC_type)pattern_types_per_id[id];
	const auto r_d = leaves_rev_depth[off_s];
	const auto n_elmts = 1 << r_d;
	switch (my_pattern_type)
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
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::duplicate_tree(const int old_path, int &new_path)
{
	const auto r_d = leaves_rev_depth[off_s];
	const auto n_elmts = 1 << r_d;
	const auto off_l = depth2offl[m - r_d];

	for (new_path = 0 ; new_path < L ; new_path++)
		if (!active_paths[new_path])
			break;
	active_paths[new_path] = true;

	std::copy(s[old_path].begin(), s[old_path].begin() + off_s + n_elmts, s[new_path].begin());
	std::copy(l[old_path].begin(), l[old_path].begin() + off_l          , l[new_path].begin());
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SCL_fast_sys<B,R,API_polar>
::delete_path(int path)
{
	active_paths[path] = false;
	n_active_paths--;
}
