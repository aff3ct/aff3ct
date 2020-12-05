/*!
 * \file
 * \brief Class module::Decoder_polar_SCL_naive.
 */
#ifndef DECODER_POLAR_SCL_NAIVE
#define DECODER_POLAR_SCL_NAIVE

#include <set>
#include <vector>

#include "Tools/Algo/Tree/Binary/Binary_node.hpp"
#include "Tools/Algo/Tree/Binary/Binary_tree_metric.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"

#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int32_t, typename R = float>
class Contents_SCL
{
public:
	std::vector<R> lambda;
	std::vector<B> s;
	bool           is_frozen_bit;

	explicit Contents_SCL(int size) : lambda(size), s(size), is_frozen_bit(0) {}
	virtual ~Contents_SCL() {}
};

template <typename B=int, typename R=float, tools::proto_f<R> F = tools::f_LLR, tools::proto_g<B,R> G = tools::g_LLR>
class Decoder_polar_SCL_naive : public Decoder_SIHO<B,R>, public tools::Interface_get_set_frozen_bits
{
protected:
	const int m;           // graph depth
	const R   metric_init; // init value of the metrics in the trees

	std::vector<bool> frozen_bits;

	const int     L; // maximum paths number
	std::set<int> active_paths;

	std::vector<tools::Binary_tree_metric<Contents_SCL<B,R>,R>> polar_trees;
	std::vector<std::vector<tools::Binary_node<Contents_SCL<B,R>>*>> leaves_array;

public:
	Decoder_polar_SCL_naive(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits);
	virtual ~Decoder_polar_SCL_naive();

	virtual Decoder_polar_SCL_naive<B,R,F,G>* clone() const;

	virtual void set_frozen_bits(const std::vector<bool>& frozen_bits);
	virtual const std::vector<bool>& get_frozen_bits() const;

protected:
	virtual void deep_copy          (const Decoder_polar_SCL_naive<B,R,F,G>& m);
	        void recursive_deep_copy(const tools::Binary_node<Contents_SCL<B,R>> *nref,
	                                       tools::Binary_node<Contents_SCL<B,R>> *nclone);

	        void _load          (const R *Y_N                               );
	        void _decode        (const size_t frame_id                      );
	        int  _decode_siho   (const R *Y_N, B *V_K, const size_t frame_id);
	        int  _decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id);
	virtual void _store         (              B *V,   bool coded = false   ) const;

private:
	void recursive_compute_llr        (      tools::Binary_node<Contents_SCL<B,R>>* node_cur, int depth);
	void propagate_sums               (const tools::Binary_node<Contents_SCL<B,R>>* node_cur           );
	void recursive_duplicate_tree_llr (      tools::Binary_node<Contents_SCL<B,R>>* node_a,
	                                         tools::Binary_node<Contents_SCL<B,R>>* node_b             );
	void recursive_duplicate_tree_sums(      tools::Binary_node<Contents_SCL<B,R>>* node_a,
	                                         tools::Binary_node<Contents_SCL<B,R>>* node_b,
	                                         tools::Binary_node<Contents_SCL<B,R>>* node_caller        );

	void duplicate_path(int path, int leaf_index);

protected:
	virtual void select_best_path(const size_t frame_id);

	void recursive_allocate_nodes_contents  (      tools::Binary_node<Contents_SCL<B,R>>* node_curr, const int vector_size               );
	void recursive_initialize_frozen_bits   (const tools::Binary_node<Contents_SCL<B,R>>* node_curr, const std::vector<bool>& frozen_bits);
	void recursive_store                    (const tools::Binary_node<Contents_SCL<B,R>>* node_curr, B *V_K, int &k                      ) const;
	void recursive_deallocate_nodes_contents(      tools::Binary_node<Contents_SCL<B,R>>* node_curr                                      );

	void apply_f     (const tools::Binary_node<Contents_SCL<B,R>>* node_curr);
	void apply_g     (const tools::Binary_node<Contents_SCL<B,R>>* node_curr);
	void compute_sums(const tools::Binary_node<Contents_SCL<B,R>>* node_curr);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive.hxx"
#endif

#endif /* DECODER_POLAR_SCL_NAIVE_ */
