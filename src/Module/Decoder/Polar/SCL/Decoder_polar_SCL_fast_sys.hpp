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
	const int                       m;                         // graph depth
	const int                       L;                         // maximum paths number
	const float                     metric_init;               // init value of the metrics in the trees
	const mipp::vector<B>&          frozen_bits;

	std::vector<bool              > active_paths;              // active paths
	std::vector<float             > metrics;                   // path metrics
	std::vector<mipp::vector<R>   > l;                         // llrs
	std::vector<mipp::vector<B>   > s;                         // partial sums
	std::vector<std::vector<float>> metrics_vec;               // list of candidate metrics to be sorted
	std::vector<std::vector<int  >> metrics_idx;               // indexes tables used to sort the metrics
	std::vector<int               > dup_count;                 // number of duplications of a path, at updating time
	std::vector<std::vector<int>  > llr_indexes;               // indexes used to sort a list of llrs (to be flipped)
	std::vector<int               > bit_flips;                 // index of the bits to be flipped
	std::vector<bool              > last_active_paths;         // used to store a former state of active paths
	std::vector<bool              > is_even;                   // used to store parity of a spc node
	std::vector<int               > depth2offl;                // lut of offl, function of depth

	int                             best_path;
	int                             n_active_paths;
	int                             off_s;

	const Pattern_parser_polar<B>   polar_patterns;

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
	inline void duplicate_path        (const int path, const int node_id, const int nb_dup = 2                 );
	inline int  duplicate_tree        (const int old_path                                                      ); // return the new_path
	inline void update_paths          (      int node_id                                                       );
	inline void update_paths_r0       (                                                                        );
	inline void update_paths_r1       (const int node_id                                                       );
	inline void update_paths_rep      (const int node_id                                                       );
	inline void update_paths_spc      (const int node_id                                                       );
	inline void update_paths_std      (const int node_id                                                       );
	inline void flip_bits             (const int old_path, const int new_path, const int node_id, const int dup);
	inline void inte                  (const int reverse_depth, const int path, const int node_id              );
	inline void rec_left              (const int reverse_depth, const int path                                 );
	inline void normalize_metrics     (std::vector<R> &vec                                                     );
	inline void recursive_compute_sums(const int off_s, const int reverse_depth, const int path, int &id       );

protected:
	        inline void delete_path     (int path);
	virtual inline void select_best_path(        );
};

#include "Decoder_polar_SCL_fast_sys.hxx"

#endif /* DECODER_POLAR_SCL_FAST_SYS_ */
