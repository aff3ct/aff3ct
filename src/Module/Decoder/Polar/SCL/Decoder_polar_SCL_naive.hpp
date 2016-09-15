#ifndef DECODER_POLAR_SCL_NAIVE
#define DECODER_POLAR_SCL_NAIVE

#include <set>
#include <vector>
#include "Tools/MIPP/mipp.h"
#include "Tools/Polar/SCL/Binary_tree_metric.hpp"

#include "../../Decoder.hpp"
#include "../decoder_polar_functions.h"

template <typename B, typename R>
class Contents_SCL
{
public:
	mipp::vector<R> lambda;
	mipp::vector<B> s;
	B               is_frozen_bit;

	Contents_SCL(int size) : lambda(size), s(size), is_frozen_bit(0) {}
	virtual ~Contents_SCL() {}
};

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
class Decoder_polar_SCL_naive : public Decoder<B,R>
{
protected:
	const int m;           // graph depth
	const R   metric_init; // init value of the metrics in the trees

	const mipp::vector<B>& frozen_bits;

	const int     L; // maximum paths number
	std::set<int> active_paths;

	std::vector<Binary_tree_metric<Contents_SCL<B,R>,R>*> polar_trees;
    std::vector<std::vector<Binary_node<Contents_SCL<B,R>>*>> leaves_array;

public:
	Decoder_polar_SCL_naive(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits, 
	                        const std::string name = "Decoder_polar_SCL_naive");
	virtual ~Decoder_polar_SCL_naive();

	        void load  (const mipp::vector<R>& Y_N);
	        void decode(                          );
	virtual void store (mipp::vector<B>& V_K      ) const;

	void set_n_frames(const int n_frames);

private:
	void recursive_compute_llr        (      Binary_node<Contents_SCL<B,R>>* node_cur, int depth);
	void propagate_sums               (const Binary_node<Contents_SCL<B,R>>* node_cur           );
	void recursive_duplicate_tree_llr (      Binary_node<Contents_SCL<B,R>>* node_a,
	                                         Binary_node<Contents_SCL<B,R>>* node_b             );
	void recursive_duplicate_tree_sums(      Binary_node<Contents_SCL<B,R>>* node_a,
	                                         Binary_node<Contents_SCL<B,R>>* node_b,
	                                         Binary_node<Contents_SCL<B,R>>* node_caller        );

	void duplicate_path(int path, int leaf_index, 
	                    std::vector<std::vector<Binary_node<Contents_SCL<B,R>>*>> leaves_array);

protected:
	virtual void select_best_path();
	virtual void intermediate_select_best_path(int leaf_index){};

	void recursive_allocate_nodes_contents  (      Binary_node<Contents_SCL<B,R>>* node_curr, const int vector_size             );
	void recursive_initialize_frozen_bits   (const Binary_node<Contents_SCL<B,R>>* node_curr, const mipp::vector<B>& frozen_bits);
	void recursive_store                    (const Binary_node<Contents_SCL<B,R>>* node_curr,       mipp::vector<B>& V_K, int &k) const;
	void recursive_deallocate_nodes_contents(      Binary_node<Contents_SCL<B,R>>* node_curr                                    );

	void apply_f     (const Binary_node<Contents_SCL<B,R>>* node_curr);
	void apply_g     (const Binary_node<Contents_SCL<B,R>>* node_curr);
	void compute_sums(const Binary_node<Contents_SCL<B,R>>* node_curr);
};

#include "Decoder_polar_SCL_naive.hxx"

#endif /* DECODER_POLAR_SCL_NAIVE_ */
