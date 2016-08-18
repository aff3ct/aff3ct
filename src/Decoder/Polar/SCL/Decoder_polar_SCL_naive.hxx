#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <map>

#include "Decoder_polar_SCL_naive.hpp"

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
Decoder_polar_SCL_naive<B,R,F,G>
::Decoder_polar_SCL_naive(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits, 
                          const std::string name)
: Decoder<B,R>(K, N, name.c_str()), m(log2(N)), metric_init(std::numeric_limits<R>::min()), frozen_bits(frozen_bits), L(L)
{
	this->active_paths.insert(0);
	for (auto i = 0 ; i < L ; i++)
	{
		this->polar_trees.push_back(new Binary_tree_metric<Contents_SCL<B,R>,R>(this->m + 1, metric_init));
		this->recursive_allocate_nodes_contents(this->polar_trees[i]->get_root(), this->N);
		this->recursive_initialize_frozen_bits(this->polar_trees[i]->get_root(), frozen_bits);
	}
	for (auto i = 0 ; i < L ; i++)
		leaves_array.push_back(this->polar_trees[i]->get_leaves());
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
Decoder_polar_SCL_naive<B,R,F,G>
::~Decoder_polar_SCL_naive()
{
	for (auto i = 0 ; i < L ; i++)
	{
		auto *cur_tree = this->polar_trees.back();
		this->recursive_deallocate_nodes_contents(cur_tree->get_root());
		this->recursive_deallocate_nodes_contents(cur_tree->get_root()->get_left());
		this->recursive_deallocate_nodes_contents(cur_tree->get_root()->get_right());

		this->polar_trees.pop_back();
		delete cur_tree;
	}
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::load(const mipp::vector<R>& Y_N)
{

	for(auto path = 0 ; path < this->L ; path ++)
	{
		auto *contents = this->polar_trees[path]->get_root()->get_contents();
		for (auto i = 0; i < this->N; i++)
			contents->lambda[i] = Y_N[i];

		polar_trees[path]->set_path_metric(metric_init);
	}

	// initialization
	active_paths.clear();
	active_paths.insert(0);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::decode()
{
	typedef typename std::vector<std::tuple<int,B,R>>::iterator it_type;
	std::set<int> last_active_paths;
	int cur_path;

	// tuples to be sorted. <Path,estimated bit,metric>
	std::vector<std::tuple<int,B,R>> metrics_vec;

	// run through each leaf
	for (auto leaf_index = 0 ; leaf_index < this->N ; leaf_index++)
	{
		// compute LLR for current leaf
		for (auto path : active_paths)
			this->recursive_compute_llr(leaves_array[path][leaf_index], compute_depth(leaf_index, this->m));

		// if current leaf is a frozen bit
		if (leaves_array[0][leaf_index]->get_c()->is_frozen_bit)
		{
			R min_phi = std::numeric_limits<R>::max();
			for (auto path : active_paths)
			{
				auto cur_leaf = leaves_array[path][leaf_index];
				cur_leaf->get_c()->s[0] = 0;
				auto phi_cur = phi<R>(polar_trees[path]->get_path_metric(), cur_leaf->get_c()->lambda[0], 0);
				this->polar_trees[path]->set_path_metric(phi_cur);
				min_phi = std::min<R>(min_phi, phi_cur);
			}
			
			// normalization
			for(auto path : active_paths)
				this->polar_trees[path]->set_path_metric(this->polar_trees[path]->get_path_metric() - min_phi);
		}
		else
		{
			// metrics vec used to store values of hypothetic path metrics
			metrics_vec.clear();
			R min_phi = std::numeric_limits<R>::max();
			for (auto path : active_paths)
			{
				auto cur_leaf = leaves_array[path][leaf_index];
				R phi0 = phi<R>(polar_trees[path]->get_path_metric(), cur_leaf->get_c()->lambda[0],             0);
				R phi1 = phi<R>(polar_trees[path]->get_path_metric(), cur_leaf->get_c()->lambda[0], bit_init<B>());
				metrics_vec.push_back(std::make_tuple(path,             0, phi0));
				metrics_vec.push_back(std::make_tuple(path, bit_init<B>(), phi1));

				min_phi = std::min<R>(min_phi, phi0);
				min_phi = std::min<R>(min_phi, phi1);
			}

			// normalization
			for(auto vec : metrics_vec)
				std::get<2>(vec) -= min_phi;

			if (active_paths.size() <= (unsigned)(L / 2))
			{
				last_active_paths = active_paths;
				for (auto path : last_active_paths)
					this->duplicate_path(path, leaf_index, leaves_array);
			}
			else
			{
				// sort hypothetic metrics
				std::sort(metrics_vec.begin(), metrics_vec.end(),
						[](std::tuple<int,B,R> x, std::tuple<int,B,R> y)
							{ return std::get<2>(x) < std::get<2>(y); }
				);

				// search in worst metrics. If a path is found twice, erase it
				for (it_type it = metrics_vec.begin() + metrics_vec.size() / 2 ;
					it != metrics_vec.end() ;
					++it)
				{
					cur_path = std::get<0>(*it);

					it_type it_double = std::find_if(it + 1, metrics_vec.end(),
												[cur_path](std::tuple<int,B,R> x)
												{ return std::get<0>(x) == cur_path; });

					if(it_double != metrics_vec.end())
						active_paths.erase(std::get<0>(*it));
				}

				// remove worst metrics from list
				metrics_vec.resize(metrics_vec.size()/2);

				for (it_type it = metrics_vec.begin(); it != metrics_vec.end(); ++it)
				{
					cur_path = std::get<0>(*it);

					it_type it_double = std::find_if(it + 1, metrics_vec.end(),
							[cur_path](std::tuple<int,B,R> x)
							{ return std::get<0>(x) == cur_path; });

					if (it_double != metrics_vec.end())
					{
						// duplicate
						metrics_vec.erase(it_double);
						duplicate_path(std::get<0>(*it), leaf_index, leaves_array);
					}
					else
					{
						// choose
						leaves_array[std::get<0>(*it)][leaf_index]->get_c()->s[0] = std::get<1>(*it);
						polar_trees[std::get<0>(*it)]->set_path_metric(std::get<2>(*it));
					}
				}
			}
		}

		// if double crc aided, intermediate crc check
		this->intermediate_select_best_path(leaf_index);

		// propagate sums
		for (auto path : active_paths)
			this->propagate_sums(leaves_array[path][leaf_index]);
	}

	this->select_best_path();
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() == (unsigned) this->K);

	auto k = 0;
	this->recursive_store((Binary_node<Contents_SCL<B,R>>*)this->polar_trees[*active_paths.begin()]->get_root(), V_K, k);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::recursive_compute_llr(Binary_node<Contents_SCL<B, R> >* node_cur, int depth)
{
	if(depth != 0)
		recursive_compute_llr(node_cur->get_father(), --depth);

	if(node_cur->is_left())
		this->apply_f((Binary_node<Contents_SCL<B,R>>*)node_cur->get_father());

	if(node_cur->is_right())
		this->apply_g((Binary_node<Contents_SCL<B,R>>*)node_cur->get_father());
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::propagate_sums(const Binary_node<Contents_SCL<B, R> >* node_cur)
{
	if(!node_cur->is_leaf())
		this->compute_sums((Binary_node<Contents_SCL<B,R>>*)node_cur);

	if(node_cur->is_right() && !node_cur->is_root())
		this->propagate_sums(node_cur->get_father());

}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::recursive_duplicate_tree_llr(Binary_node<Contents_SCL<B,R>>* node_a,
                               Binary_node<Contents_SCL<B,R> >* node_b)
{
	node_b->get_c()->lambda = node_a->get_c()->lambda;

	if(!node_a->get_father()->is_root())
		this->recursive_duplicate_tree_llr(node_a->get_father(), node_b->get_father());		
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::recursive_duplicate_tree_sums(Binary_node<Contents_SCL<B,R>>* node_a,
                                Binary_node<Contents_SCL<B,R> >* node_b,
                                Binary_node<Contents_SCL<B,R> >* node_caller)
{
	if(node_a->get_left() != node_caller && !node_a->is_leaf())
		node_b->get_left()->get_c()->s = node_a->get_left()->get_c()->s;

	if(!node_a->is_root())
		this->recursive_duplicate_tree_sums(node_a->get_father(), node_b->get_father(), node_a);		
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::duplicate_path(int path, int leaf_index, std::vector<std::vector<Binary_node<Contents_SCL<B,R>>*>> leaves_array)
{
	std::vector<Binary_node<Contents_SCL<B,R>>*> path_leaves, newpath_leaves;
	int newpath = 0;
	while(active_paths.find(newpath++) != active_paths.end()){};
	newpath--;

	active_paths.insert(newpath);

	path_leaves = leaves_array[path];

	newpath_leaves = leaves_array[newpath];

	for(auto i = 0 ; i < leaf_index ; i++)
		newpath_leaves[i]->get_c()->s = path_leaves[i]->get_c()->s;

	recursive_duplicate_tree_sums(leaves_array[path][leaf_index], leaves_array[newpath][leaf_index], nullptr);

	if(leaf_index < this->N - 1)
		recursive_duplicate_tree_llr(leaves_array[path][leaf_index + 1], leaves_array[newpath][leaf_index + 1]);	

	leaves_array[newpath][leaf_index]->get_c()->s[0] = bit_init<B>();
	polar_trees[newpath]->set_path_metric(phi<R>(polar_trees[path]->get_path_metric(),
	                                             leaves_array[path][leaf_index]->get_c()->lambda[0], 
	                                             bit_init<B>()));

	leaves_array[path][leaf_index]->get_c()->s[0] = 0;
	polar_trees[path]->set_path_metric(phi<R>(polar_trees[path]->get_path_metric(),
	                                          leaves_array[path][leaf_index]->get_c()->lambda[0], 
	                                          0));
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::select_best_path() 
{
	int best_path = 0;
	if(active_paths.size() >= 1)
		best_path = *active_paths.begin();

	for(int path : active_paths)
	{
		if(polar_trees[path]->get_path_metric() < polar_trees[best_path]->get_path_metric())
			best_path = path;
	}

	active_paths.clear();
	active_paths.insert(best_path);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::recursive_allocate_nodes_contents(Binary_node<Contents_SCL<B,R>>* node_curr,
                                    const int vector_size)
{
	if (node_curr != nullptr)
	{
		node_curr->set_contents(new Contents_SCL<B,R>(vector_size));

		this->recursive_allocate_nodes_contents(node_curr->get_left() , vector_size / 2);
		this->recursive_allocate_nodes_contents(node_curr->get_right(), vector_size / 2);
	}
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::recursive_initialize_frozen_bits(const Binary_node<Contents_SCL<B,R>>* node_curr,
                                   const mipp::vector<B>& frozen_bits)
{
	auto *contents = node_curr->get_contents();

	if (!node_curr->is_leaf()) // stop condition
	{
		this->recursive_initialize_frozen_bits(node_curr->get_left(),  frozen_bits); // recursive call
		this->recursive_initialize_frozen_bits(node_curr->get_right(), frozen_bits); // recursive call
	}
	else
		contents->is_frozen_bit = frozen_bits[node_curr->get_lane_id()];
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::recursive_store(const Binary_node<Contents_SCL<B,R>>* node_curr, mipp::vector<B>& V_K, int &k) const
{
	auto *contents = node_curr->get_contents();

	if (!node_curr->is_leaf()) // stop condition
	{
		this->recursive_store(node_curr->get_left(),  V_K, k); // recursive call
		this->recursive_store(node_curr->get_right(), V_K, k); // recursive call
	}
	else
		if (!frozen_bits[node_curr->get_lane_id()])
			V_K[k++] = contents->s[0] ? 1 : 0;
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::recursive_deallocate_nodes_contents(Binary_node<Contents_SCL<B,R>>* node_curr)
{
	if (node_curr != nullptr)
	{
		this->recursive_deallocate_nodes_contents(node_curr->get_left()); // recursive call
		this->recursive_deallocate_nodes_contents(node_curr->get_right()); // recursive call

		auto *contents = node_curr->get_contents();
		delete contents;
		node_curr->set_contents(nullptr);
	}
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::apply_f(const Binary_node<Contents_SCL<B,R>>* node_curr)
{
	const auto size = (int)node_curr->get_c()->lambda.size();
	const auto size_2 = size / 2;

	const auto *node_left  = node_curr->get_left(); // get left node

	for (auto i = 0; i < size_2; i++)
		node_left->get_c()->lambda[i] = F(node_curr->get_c()->lambda[        i],  // apply f()
		                                  node_curr->get_c()->lambda[size_2 +i]);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::apply_g(const Binary_node<Contents_SCL<B,R>>* node_curr)
{
	const auto size = (int)node_curr->get_c()->lambda.size();
	const auto size_2 = size / 2;

	const auto *node_left  = node_curr->get_left(); // get left node
	const auto *node_right = node_curr->get_right(); // get right node

	for (auto i = 0; i < size_2; i++)
		node_right->get_c()->lambda[i] = G(node_curr->get_c()->lambda[        i], // apply g()
		                                   node_curr->get_c()->lambda[size_2 +i],
		                                   node_left->get_c()->s     [        i]);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive<B,R,F,G>
::compute_sums(const Binary_node<Contents_SCL<B,R>>* node_curr)
{
	const auto size = (int)node_curr->get_c()->lambda.size();
	const auto size_2 = size / 2;

	const auto *node_left  = node_curr->get_left(); // get left node
	const auto *node_right = node_curr->get_right(); // get right node

	for (auto i = 0; i < size_2; i++)
		node_curr->get_c()->s[i] = node_left->get_c()->s[i] ^ node_right->get_c()->s[i]; // bit xor

	for (auto i = 0; i < size_2; i++)
		node_curr->get_c()->s[size_2 +i] = node_right->get_c()->s[i]; // bit eq
}
