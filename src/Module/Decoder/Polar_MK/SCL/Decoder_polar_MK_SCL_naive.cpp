#include <algorithm>
#include <sstream>
#include <string>
#include <limits>
#include <tuple>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Tools/Code/Polar/fb_assert.h"
#include "Module/Decoder/Polar_MK/SCL/Decoder_polar_MK_SCL_naive.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_polar_MK_SCL_naive<B,R>
::Decoder_polar_MK_SCL_naive(const int& K,
                             const int& N,
                             const int& L,
                             const tools::Polar_code& code,
                             const std::vector<bool>& frozen_bits,
                             const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
                                                                           const std::vector<B> &bits)>>> &lambdas)
: Decoder_SIHO<B,R>(K, N),
  metric_init(std::numeric_limits<R>::min()),
  L(L),
  code(code),
  frozen_bits(frozen_bits),
  Ke(code.get_kernel_matrices().size()),
  idx(code.get_biggest_kernel_size()),
  u(code.get_biggest_kernel_size()),
  LLRs(code.get_biggest_kernel_size()),
  bits(code.get_biggest_kernel_size() -1),
  lambdas(code.get_kernel_matrices().size())
{
	const std::string name = "Decoder_polar_MK_SCL_naive";
	this->set_name(name);

	if (this->N != code.get_codeword_size())
	{
		std::stringstream message;
		message << "'N' has to be equal to 'code.get_codeword_size()' ('N' = " << N
		        << ", 'code.get_codeword_size()' = " << code.get_codeword_size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != (int)frozen_bits.size())
	{
		std::stringstream message;
		message << "'frozen_bits.size()' has to be equal to 'N' ('frozen_bits.size()' = " << frozen_bits.size()
		        << ", 'N' = " << N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto k = 0; for (auto i = 0; i < this->N; i++) if (frozen_bits[i] == 0) k++;
	if (this->K != k)
	{
		std::stringstream message;
		message << "The number of information bits in the frozen_bits is invalid ('K' = " << K << ", 'k' = "
		        << k << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->L <= 0 || !tools::is_power_of_2(this->L))
	{
		std::stringstream message;
		message << "'L' has to be a positive power of 2 ('L' = " << L << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->active_paths.insert(0);

	std::vector<uint32_t> sequence(code.get_stages().size());
	for (size_t s  = 0; s < code.get_stages().size(); s++)
		sequence[s] = code.get_kernel_matrices()[code.get_stages()[(sequence.size()-1)-s]].size();

	for (auto i = 0; i < L; i++)
	{
		this->polar_trees.push_back(tools::Generic_tree_metric<Contents_MK_SCL<B,R>,R>(sequence, metric_init));
		int max_depth_llrs = (int)this->code.get_stages().size() -1;
		this->recursive_allocate_nodes_contents(this->polar_trees[i].get_root(), this->N, max_depth_llrs);
		this->recursive_initialize_frozen_bits(this->polar_trees[i].get_root(), frozen_bits);
	}
	for (auto i = 0; i < L; i++)
		leaves_array.push_back(this->polar_trees[i].get_leaves());

	for (auto ke = 0; ke < (int)this->code.get_kernel_matrices().size(); ke++)
	{
		const auto kernel_size = (int)this->code.get_kernel_matrices()[ke].size();
		Ke[ke].resize(kernel_size * kernel_size);
		for (auto i = 0; i < kernel_size; i++)
			for (auto j = 0; j < kernel_size; j++)
				this->Ke[ke][i * kernel_size +j] = (B)this->code.get_kernel_matrices()[ke][j][i];
	}

	if (lambdas.size() == 0)
	{
		for (size_t l = 0; l < this->lambdas.size(); l++)
		{
			if (tools::Polar_lambdas<B,R>::functions.find(code.get_kernel_matrices()[l]) ==
			    tools::Polar_lambdas<B,R>::functions.end())
				throw tools::runtime_error(__FILE__, __LINE__, __func__, "Unsupported polar kernel.");
			this->lambdas[l] = tools::Polar_lambdas<B,R>::functions[code.get_kernel_matrices()[l]];
		}
	}
	else
	{
		if (lambdas.size() != code.get_kernel_matrices().size())
		{
			std::stringstream message;
			message << "'lambdas.size()' has to be equal to 'code.get_kernel_matrices().size()' ("
			        << "'lambdas.size()' = " << lambdas.size() << ", "
			        << "'code.get_kernel_matrices().size()' = " << code.get_kernel_matrices().size() << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		for (size_t l = 0; l < code.get_kernel_matrices().size(); l++)
		{
			if (lambdas[l].size() != code.get_kernel_matrices()[l].size())
			{
				std::stringstream message;
				message << "'lambdas[l].size()' has to be equal to 'code.get_kernel_matrices()[l].size()' ("
				        << "'l' = " << l  << ", "
				        << "'lambdas[l].size()' = " << lambdas[l].size() << ", "
				        << "'code.get_kernel_matrices()[l].size()' = " << code.get_kernel_matrices()[l].size() << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		}

		this->lambdas = lambdas;
	}
}

template <typename B, typename R>
Decoder_polar_MK_SCL_naive<B,R>
::Decoder_polar_MK_SCL_naive(const int& K,
                             const int& N,
                             const int& L,
                             const tools::Polar_code& code,
                             const std::vector<bool>& frozen_bits)
: Decoder_polar_MK_SCL_naive<B,R>(K, N, L, code, frozen_bits, {})
{
}

template <typename B, typename R>
Decoder_polar_MK_SCL_naive<B,R>
::~Decoder_polar_MK_SCL_naive()
{
	for (auto i = 0; i < L; i++)
		this->recursive_deallocate_nodes_contents(this->polar_trees[i].get_root());
}

template <typename B, typename R>
Decoder_polar_MK_SCL_naive<B,R>* Decoder_polar_MK_SCL_naive<B,R>
::clone() const
{
	auto m = new Decoder_polar_MK_SCL_naive(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::deep_copy(const Decoder_polar_MK_SCL_naive<B,R> &m)
{
	Module::deep_copy(m);
	this->leaves_array.clear();
	for (auto i = 0; i < L; i++)
	{
		this->recursive_deep_copy(m.polar_trees[i].get_root(), this->polar_trees[i].get_root());
		this->leaves_array.push_back(this->polar_trees[i].get_leaves());
	}
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::recursive_deep_copy(const tools::Generic_node<Contents_MK_SCL<B,R>> *nref, tools::Generic_node<Contents_MK_SCL<B,R>> *nclone)
{
	auto cref = nref->get_contents();
	auto cclone = new Contents_MK_SCL<B,R>(*cref);
	nclone->set_contents(cclone);

	if (!nref->is_leaf() && !nclone->is_leaf())
		for (size_t c = 0; c < nref->get_children().size(); c++)
			if (nref->get_children()[c] != nullptr)
				this->recursive_deep_copy(nref->get_children()[c], nclone->get_children()[c]);
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::set_frozen_bits(const std::vector<bool>& fb)
{
	aff3ct::tools::fb_assert(frozen_bits, this->K, this->N);
	std::copy(fb.begin(), fb.end(), this->frozen_bits.begin());
	for (auto i = 0; i < L; i++)
		this->recursive_initialize_frozen_bits(this->polar_trees[i].get_root(), fb);

}

template <typename B, typename R>
const std::vector<bool>& Decoder_polar_MK_SCL_naive<B,R>
::get_frozen_bits() const
{
	return this->frozen_bits;
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::_load(const R *Y_N)
{
	for (auto path = 0; path < this->L; path ++)
	{
		std::copy(Y_N, Y_N + this->N, this->polar_trees[path].get_root()->get_contents()->l.data());
		polar_trees[path].set_path_metric(metric_init);
	}

	// initialization
	active_paths.clear();
	active_paths.insert(0);
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::_decode(const size_t frame_id)
{
	std::set<int> last_active_paths;
	int cur_path;

	// tuples to be sorted. <Path,estimated bit,metric>
	std::vector<std::tuple<int,B,R>> metrics_vec;

	// run through each leaf
	for (auto leaf_index = 0 ; leaf_index < this->N; leaf_index++)
	{
		// compute LLR for current leaf
		for (auto path : active_paths)
			this->recursive_compute_llr(leaves_array[path][leaf_index],
			                            leaves_array[path][leaf_index]->get_c()->max_depth_llrs);

		// if current leaf is a frozen bit
		if (leaves_array[0][leaf_index]->get_c()->is_frozen_bit)
		{
			auto min_phi = std::numeric_limits<R>::max();
			for (auto path : active_paths)
			{
				auto cur_leaf = leaves_array[path][leaf_index];
				cur_leaf->get_c()->s[0] = 0;
				auto phi_cur = tools::phi<R>(polar_trees[path].get_path_metric(), cur_leaf->get_c()->l[0], 0);
				this->polar_trees[path].set_path_metric(phi_cur);
				min_phi = std::min<R>(min_phi, phi_cur);
			}

			// normalization
			for (auto path : active_paths)
				this->polar_trees[path].set_path_metric(this->polar_trees[path].get_path_metric() - min_phi);
		}
		else
		{
			// metrics vec used to store values of hypothetic path metrics
			metrics_vec.clear();
			auto min_phi = std::numeric_limits<R>::max();
			for (auto path : active_paths)
			{
				auto cur_leaf = leaves_array[path][leaf_index];
				R phi0 = tools::phi<B,R>(polar_trees[path].get_path_metric(), cur_leaf->get_c()->l[0], (B)0);
				R phi1 = tools::phi<B,R>(polar_trees[path].get_path_metric(), cur_leaf->get_c()->l[0], (B)1);
				metrics_vec.push_back(std::make_tuple(path, (B)0, phi0));
				metrics_vec.push_back(std::make_tuple(path, (B)1, phi1));

				min_phi = std::min<R>(min_phi, phi0);
				min_phi = std::min<R>(min_phi, phi1);
			}

			// normalization
			for (auto vec : metrics_vec)
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
					[](std::tuple<int,B,R> x, std::tuple<int,B,R> y){
						return std::get<2>(x) < std::get<2>(y);
					});

				// search in worst metrics. If a path is found twice, erase it
				for (auto it = metrics_vec.begin() + metrics_vec.size() / 2; it != metrics_vec.end(); ++it)
				{
					cur_path = std::get<0>(*it);

					auto it_double = std::find_if(it +1, metrics_vec.end(),
						[cur_path](std::tuple<int,B,R> x){
							return std::get<0>(x) == cur_path;
						});

					if (it_double != metrics_vec.end())
						active_paths.erase(std::get<0>(*it));
				}

				// remove worst metrics from list
				metrics_vec.resize(metrics_vec.size() / 2);

				for (auto it = metrics_vec.begin(); it != metrics_vec.end(); ++it)
				{
					cur_path = std::get<0>(*it);

					auto it_double = std::find_if(it +1, metrics_vec.end(),
						[cur_path](std::tuple<int,B,R> x){
							return std::get<0>(x) == cur_path;
						});

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
						polar_trees[std::get<0>(*it)].set_path_metric(std::get<2>(*it));
					}
				}
			}
		}

		// propagate sums
		for (auto path : active_paths)
			this->recursive_propagate_sums(leaves_array[path][leaf_index]);
	}

	this->select_best_path(frame_id);
}

template <typename B, typename R>
int Decoder_polar_MK_SCL_naive<B,R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	return 0;
}

template <typename B, typename R>
int Decoder_polar_MK_SCL_naive<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode(frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_N, true);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	return 0;
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::_store(B *V, bool coded) const
{
	auto *root = this->polar_trees[*active_paths.begin()].get_root();
	if (!coded)
	{
		auto k = 0;
		this->recursive_store(root, V, k);
	}
	else
		std::copy(root->get_c()->s.begin(), root->get_c()->s.begin() + this->N, V);
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::recursive_compute_llr(tools::Generic_node<Contents_MK_SCL<B,R>>* node_cur, int depth)
{
	auto node_father = node_cur->get_father();

	if (depth != 0)
		recursive_compute_llr(node_father, --depth);

	if (!node_cur->is_root())
	{
		const auto child = node_cur->get_child_id();
		const auto stage = polar_trees[0].get_depth() - node_father->get_depth() -2;
		const auto kern_size = (int)node_father->get_children().size();
		const auto size = (int)node_father->get_c()->l.size();
		const auto n_kernels = size / kern_size;

		for (auto k = 0; k < n_kernels; k++)
		{
			for (auto l = 0; l < kern_size; l++) LLRs[l] = node_father->get_c()->l[l * n_kernels +k];
			for (auto c = 0; c < child;     c++) bits[c] = node_father->get_children()[c]->get_c()->s[k];

			node_cur->get_c()->l[k] = lambdas[this->code.get_stages()[stage]][child](LLRs, bits);
		}
	}
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::recursive_propagate_sums(const tools::Generic_node<Contents_MK_SCL<B,R>>* node_cur)
{
	if (!node_cur->is_leaf())
	{
		const auto stage = polar_trees[0].get_depth() - node_cur->get_depth() -2;
		const auto kern_size = (int)node_cur->get_children().size();
		const auto size = (int)node_cur->get_c()->s.size();
		const auto n_kernels = size / kern_size;

		auto encode_polar_kernel = [](const B *u, const uint32_t *idx, const B *Ke, B *x, const int size)
		{
			for (auto i = 0; i < size; i++)
			{
				const auto stride = i * size;
				auto sum = 0;
				for (auto j = 0; j < size; j++)
					sum += u[j] & Ke[stride +j];
				x[idx[i]] = sum & (B)1;
			}
		};

		// re-encode the bits (partial sums) (generalized to all kernels)
		for (auto k = 0; k < n_kernels; k++)
		{
			for (auto i = 0; i < kern_size; i++)
			{
				this->idx[i] = (uint32_t)(n_kernels * i +k);
				this->u[i] = node_cur->get_children()[(this->idx[i]/n_kernels)]->get_c()->s[this->idx[i]%n_kernels];
			}

			encode_polar_kernel(this->u.data(),
			                    this->idx.data(),
			                    this->Ke[code.get_stages()[stage]].data(),
			                    node_cur->get_c()->s.data(),
			                    kern_size);
		}
	}

	if (!node_cur->is_root() &&
	    ((size_t)node_cur->get_child_id() == node_cur->get_father()->get_children().size() -1))
		this->recursive_propagate_sums(node_cur->get_father());
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::recursive_duplicate_tree_llr(tools::Generic_node<Contents_MK_SCL<B,R>>* node_a,
                               tools::Generic_node<Contents_MK_SCL<B,R>>* node_b)
{
	node_b->get_c()->l = node_a->get_c()->l;

	if(!node_a->get_father()->is_root())
		this->recursive_duplicate_tree_llr(node_a->get_father(), node_b->get_father());
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::recursive_duplicate_tree_sums(tools::Generic_node<Contents_MK_SCL<B,R>>* node_a,
                                tools::Generic_node<Contents_MK_SCL<B,R>>* node_b,
                                tools::Generic_node<Contents_MK_SCL<B,R>>* node_caller)
{
	if (!node_a->is_leaf())
		for (size_t c = 0; c < node_a->get_children().size()-1; c++)
		{
			auto child_a = node_a->get_children()[c];
			if (child_a != node_caller)
				node_b->get_children()[c]->get_c()->s = child_a->get_c()->s;
		}

	if (!node_a->is_root())
		this->recursive_duplicate_tree_sums(node_a->get_father(), node_b->get_father(), node_a);
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::duplicate_path(int path, int leaf_index, std::vector<std::vector<tools::Generic_node<Contents_MK_SCL<B,R>>*>> leaves_array)
{
	std::vector<tools::Generic_node<Contents_MK_SCL<B,R>>*> path_leaves, newpath_leaves;
	int newpath = 0;
	while (active_paths.find(newpath++) != active_paths.end()){};
	newpath--;

	active_paths.insert(newpath);

	path_leaves = leaves_array[path];

	newpath_leaves = leaves_array[newpath];

	for (auto i = 0; i < leaf_index; i++)
		newpath_leaves[i]->get_c()->s = path_leaves[i]->get_c()->s;

	recursive_duplicate_tree_sums(leaves_array[path][leaf_index], leaves_array[newpath][leaf_index], nullptr);

	if (leaf_index < this->N - 1)
		recursive_duplicate_tree_llr(leaves_array[path][leaf_index + 1], leaves_array[newpath][leaf_index + 1]);

	leaves_array[newpath][leaf_index]->get_c()->s[0] = (B)1;
	polar_trees[newpath].set_path_metric(tools::phi<B,R>(polar_trees[path].get_path_metric(),
	                                                     leaves_array[path][leaf_index]->get_c()->l[0],
	                                                     (B)1));

	leaves_array[path][leaf_index]->get_c()->s[0] = (B)0;
	polar_trees[path].set_path_metric(tools::phi<B,R>(polar_trees[path].get_path_metric(),
	                                                  leaves_array[path][leaf_index]->get_c()->l[0],
	                                                  (B)0));
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::select_best_path(const size_t frame_id)
{
	int best_path = 0;
	if (active_paths.size() >= 1)
		best_path = *active_paths.begin();

	for (int path : active_paths)
		if(polar_trees[path].get_path_metric() < polar_trees[best_path].get_path_metric())
			best_path = path;

	active_paths.clear();
	active_paths.insert(best_path);
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::recursive_allocate_nodes_contents(tools::Generic_node<Contents_MK_SCL<B,R>>* node_curr, const int vector_size, int &max_depth_llrs)
{
	node_curr->set_contents(new Contents_MK_SCL<B,R>(vector_size));

	if (!node_curr->is_leaf())
	{
		const auto stage = polar_trees[0].get_depth() - node_curr->get_depth() -2;
		const auto new_vector_size = vector_size / this->code.get_kernel_matrices()[code.get_stages()[stage]].size();
		for (auto c : node_curr->get_children())
			this->recursive_allocate_nodes_contents(c, new_vector_size, max_depth_llrs);
	}
	else
		node_curr->get_c()->max_depth_llrs = max_depth_llrs;

	max_depth_llrs = this->polar_trees[0].get_depth() - node_curr->get_depth() -1;
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::recursive_initialize_frozen_bits(const tools::Generic_node<Contents_MK_SCL<B,R>>* node_curr,
                                   const std::vector<bool>& frozen_bits)
{
	if (!node_curr->is_leaf()) // stop condition
	{
		for (auto c : node_curr->get_children())
			this->recursive_initialize_frozen_bits(c, frozen_bits); // recursive call
	}
	else
		node_curr->get_contents()->is_frozen_bit = frozen_bits[node_curr->get_lane_id()];
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::recursive_store(const tools::Generic_node<Contents_MK_SCL<B,R>>* node_curr, B *V_K, int &k) const
{
	if (!node_curr->is_leaf()) // stop condition
		for (auto c : node_curr->get_children())
			this->recursive_store(c, V_K, k); // recursive call
	else
		if (!frozen_bits[node_curr->get_lane_id()])
			V_K[k++] = node_curr->get_contents()->s[0] ? 1 : 0;
}

template <typename B, typename R>
void Decoder_polar_MK_SCL_naive<B,R>
::recursive_deallocate_nodes_contents(tools::Generic_node<Contents_MK_SCL<B,R>>* node_curr)
{
	for (auto c : node_curr->get_children())
		this->recursive_deallocate_nodes_contents(c); // recursive call

	delete node_curr->get_contents();
	node_curr->set_contents(nullptr);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_polar_MK_SCL_naive<B_8,Q_8>;
template class aff3ct::module::Decoder_polar_MK_SCL_naive<B_16,Q_16>;
template class aff3ct::module::Decoder_polar_MK_SCL_naive<B_32,Q_32>;
template class aff3ct::module::Decoder_polar_MK_SCL_naive<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_polar_MK_SCL_naive<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
