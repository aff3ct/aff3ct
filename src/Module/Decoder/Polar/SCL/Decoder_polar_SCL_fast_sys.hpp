#ifndef DECODER_POLAR_SCL_FAST_SYS
#define DECODER_POLAR_SCL_FAST_SYS

#include <set>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../../Decoder.hpp"
#include "../decoder_polar_functions.h"

template <typename B, typename R, class API_polar>
class Decoder_polar_SCL_fast_sys : public Decoder<B,R>
{
protected:
	const int m;             // graph depth
	const int L;             // maximum paths number
	const float metric_init; // init value of the metrics in the trees
	      int best_path;
	const mipp::vector<B>& frozen_bits;

	std::vector<bool> active_paths;
	int               n_active_paths;

	std::vector<float>   metrics;

	std::vector<mipp::vector<R>> l;
	std::vector<mipp::vector<B>> s;

	std::vector<char> pattern_types_per_id;
	std::vector<char> pattern_types_per_id_sums;

	std::vector<std::vector<float>> metrics_vec;
	std::vector<std::vector<int  >> metrics_idx;

	std::vector<int> dup_count;

	std::vector<std::vector<int>> llr_indexes;
	std::vector<int> bit_flips;

	std::vector<bool> last_active_paths;

	std::vector<bool> is_even;

	std::vector<int> leaves_rev_depth;
	std::vector<int> depth2offl;

	int off_s;

public:
	Decoder_polar_SCL_fast_sys(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits,
	                           const int n_frames = 1, const std::string name = "Decoder_polar_SCL_fast_sys");
	virtual ~Decoder_polar_SCL_fast_sys();

	        void load       (const mipp::vector<R>& Y_N);
	        void hard_decode(                          );
	virtual void store      (mipp::vector<B>& V_N      ) const;
	        void unpack     (mipp::vector<B>& V_N      ) const;
	virtual void store_fast (mipp::vector<B>& V        ) const;

private:
	void duplicate_path            (const int path, const int id, const int nb_dup = 2                 );
	void duplicate_tree            (const int old_path, int &new_path                                  );
	void update_paths              (      int id                                                       );
	void update_paths_r0           (                                                                   );
	void update_paths_r1           (const int id                                                       );
	void update_paths_rep          (const int id                                                       );
	void update_paths_spc          (const int id                                                       );
	void update_paths_std          (const int id                                                       );
	void recursive_get_leaves_depth(Binary_node<Pattern_SC_interface>* node_curr, int &leaf_index      );
	void generate_nodes_indexes    (const Binary_node<Pattern_SC_interface>* node_curr, int &node_index);
	void flip_bits                 (const int old_path, const int new_path, const int id, const int dup);
	void inte                      (const int reverse_depth, const int path, int &id                   );
	void rec_left                  (const int reverse_depth, const int path, int &id                   );
	void normalize_metrics         (std::vector<R> &vec                                                );
protected:
	        void delete_path     (int path);
	virtual void select_best_path(        );

	virtual void recursive_compute_sums(const int off_s, const int reverse_depth, const int path, int &id);

};

#include "Decoder_polar_SCL_fast_sys.hxx"

#endif /* DECODER_POLAR_SCL_FAST_SYS_ */
