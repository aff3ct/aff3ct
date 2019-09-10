#ifndef DECODER_POLAR_MK_SCL_NAIVE
#define DECODER_POLAR_MK_SCL_NAIVE

#include <set>
#include <vector>

#include "Tools/Algo/Tree/Generic/Generic_tree_metric.hpp"
#include "Tools/Code/Polar/Frozenbits_notifier.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"

#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Contents_MK_SCL
{
public:
	std::vector<R> l;
	std::vector<B> s;
	bool           is_frozen_bit;
	int            max_depth_llrs;

	explicit Contents_MK_SCL(int size) : l(size), s(size), is_frozen_bit(false), max_depth_llrs(-1) {}
	virtual ~Contents_MK_SCL() {}
};

template <typename B, typename R>
class Decoder_polar_MK_SCL_naive : public Decoder_SIHO<B,R>, public tools::Frozenbits_notifier
{
protected:
	const R metric_init; // init value of the metrics in the trees
	const int L; // maximum paths number
	std::set<int> active_paths;

	const tools::Polar_code &code;
	const std::vector<bool> &frozen_bits;
	std::vector<tools::Generic_tree_metric<Contents_MK_SCL<B,R>,R>*> polar_trees;
	std::vector<std::vector<tools::Generic_node<Contents_MK_SCL<B,R>>*>> leaves_array;
	std::vector<std::vector<B>> Ke;
	std::vector<uint32_t> idx;
	std::vector<B> u;

	std::vector<R> LLRs;
	std::vector<B> bits;
	std::vector<std::vector<std::function<R(const std::vector<R> &LLRs, const std::vector<B> &bits)>>> lambdas;

public:
	Decoder_polar_MK_SCL_naive(const int&               K,
	                           const int&               N,
	                           const int&               L,
	                           const tools::Polar_code& code,
	                           const std::vector<bool>& frozen_bits,
	                           const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
	                                                                         const std::vector<B> &bits)>>> &lambdas,
	                           const int                n_frames = 1);

	Decoder_polar_MK_SCL_naive(const int&               K,
	                           const int&               N,
	                           const int&               L,
	                           const tools::Polar_code& code,
	                           const std::vector<bool>& frozen_bits,
	                           const int                n_frames = 1);
	virtual ~Decoder_polar_MK_SCL_naive();

	virtual void notify_frozenbits_update();

protected:
	        void _load          (const R *Y_N                            );
	        void _decode        (                                        );
	virtual void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	virtual void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);
	virtual void _store         (              B *V,   bool coded = false) const;

private:
	void recursive_compute_llr        (      tools::Generic_node<Contents_MK_SCL<B,R>>* node_cur, int depth);
	void recursive_propagate_sums     (const tools::Generic_node<Contents_MK_SCL<B,R>>* node_cur           );
	void recursive_duplicate_tree_llr (      tools::Generic_node<Contents_MK_SCL<B,R>>* node_a,
	                                         tools::Generic_node<Contents_MK_SCL<B,R>>* node_b             );
	void recursive_duplicate_tree_sums(      tools::Generic_node<Contents_MK_SCL<B,R>>* node_a,
	                                         tools::Generic_node<Contents_MK_SCL<B,R>>* node_b,
	                                         tools::Generic_node<Contents_MK_SCL<B,R>>* node_caller        );

	void duplicate_path(int path, int leaf_index,
	                    std::vector<std::vector<tools::Generic_node<Contents_MK_SCL<B,R>>*>> leaves_array);

protected:
	virtual void select_best_path();

	void recursive_allocate_nodes_contents  (      tools::Generic_node<Contents_MK_SCL<B,R>>* node_curr, const int vector_size, int &max_depth_llrs);
	void recursive_initialize_frozen_bits   (const tools::Generic_node<Contents_MK_SCL<B,R>>* node_curr, const std::vector<bool>& frozen_bits);
	void recursive_store                    (const tools::Generic_node<Contents_MK_SCL<B,R>>* node_curr, B *V_K, int &k                      ) const;
	void recursive_deallocate_nodes_contents(      tools::Generic_node<Contents_MK_SCL<B,R>>* node_curr                                      );
};
}
}

#endif /* DECODER_POLAR_MK_SCL_NAIVE_ */
