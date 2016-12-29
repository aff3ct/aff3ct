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
	const int                        m;              // graph depth
	const int                        L;              // maximum paths number
	const mipp::vector<B>&           frozen_bits;
	const Pattern_parser_polar<B>    polar_patterns;

	            std ::vector<int  >  paths;          // active paths
	            std ::vector<int  >  last_paths;     // active paths cpy
	            std ::vector<float>  metrics;        // path metrics
	            mipp::vector<R    >  Y_N;            // channel llrs
	std::vector<mipp::vector<R    >> l;              // llrs
	std::vector<mipp::vector<B    >> s;              // partial sums
	std::vector<std ::vector<float>> metrics_vec;    // list of candidate metrics to be sorted
	std::vector<std ::vector<int  >> metrics_idx;    // indexes tables used to sort the metrics
	            std ::vector<int  >  dup_count;      // number of duplications of a path, at updating time
	std::vector<std ::vector<int  >> llr_indexes;    // indexes used to sort a list of llrs (to be flipped)
	            std ::vector<int  >  bit_flips;      // index of the bits to be flipped
	            std ::vector<bool >  is_even;        // used to store parity of a spc node

	int                              best_path;
	int                              n_active_paths;

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
	inline void recursive_decode(const int off_l, const int off_s, const int rev_depth, int &node_id     );

	inline void update_paths_r0 (                     const int off_l, const int off_s, const int n_elmts);
	inline void update_paths_r1 (const int rev_depth, const int off_l, const int off_s, const int n_elmts);
	inline void update_paths_rep(                     const int off_l, const int off_s, const int n_elmts);
	inline void update_paths_spc(const int rev_depth, const int off_l, const int off_s, const int n_elmts);

	inline void flip_bits_r1    (const int old_path, const int new_path, const int dup, const int off_s, const int n_elmts);
	inline void flip_bits_rep   (const int old_path, const int new_path,                const int off_s, const int n_elmts);
	inline void flip_bits_spc   (const int old_path, const int new_path, const int dup, const int off_s, const int n_elmts);

	// return the new_path
	inline int  duplicate_tree  (const int old_path, const int off_l, const int off_s, const int n_elmts );

protected:
	        inline void delete_path     (int path_id);
	virtual inline void select_best_path(           );
};

#include "Decoder_polar_SCL_fast_sys.hxx"

#endif /* DECODER_POLAR_SCL_FAST_SYS_ */
