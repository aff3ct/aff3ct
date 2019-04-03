#include <chrono>
#include <algorithm>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Decoder_polar_MK_SC_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
Decoder_polar_MK_SC_naive<B,R>
::Decoder_polar_MK_SC_naive(const int& K,
                            const int& N,
                            const std::vector<bool>& frozen_bits,
                            const int base,
                            const int n_frames)
: Decoder          (K, N, n_frames, 1),
  Decoder_SIHO<B,R>(K, N, n_frames, 1),
  base(base), m((int)std::log2(N)), frozen_bits(frozen_bits), polar_tree(m +1, 2)
{
	const std::string name = "Decoder_polar_MK_SC_naive";
	this->set_name(name);

	if (base < 2)
	{
		std::stringstream message;
		message << "'base' has to be bigger or equal to 2 ('base' = " << this->base << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (!tools::is_power_of_2(this->N))
	{
		std::stringstream message;
		message << "'N' has to be a power of 2 ('N' = " << N << ").";
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

	this->recursive_allocate_nodes_contents(this->polar_tree.get_root(), this->N);
	this->recursive_initialize_frozen_bits(this->polar_tree.get_root(), frozen_bits);
}

template <typename B, typename R>
Decoder_polar_MK_SC_naive<B,R>
::~Decoder_polar_MK_SC_naive()
{
	this->recursive_deallocate_nodes_contents(this->polar_tree.get_root());
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::notify_frozenbits_update()
{
	this->recursive_initialize_frozen_bits(this->polar_tree.get_root(), frozen_bits);
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::_load(const R *Y_N)
{
	auto *contents = this->polar_tree.get_root()->get_contents();

	for (auto i = 0; i < this->N; i++)
		contents->lambda[i] = Y_N[i];
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->recursive_decode(this->polar_tree.get_root());
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->recursive_decode(this->polar_tree.get_root());
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_N, true);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::_store(B *V, bool coded) const
{
	if (!coded)
	{
		auto k = 0;
		this->recursive_store(this->polar_tree.get_root(), V, k);
	}
	else
	{
		auto *contents_root = this->polar_tree.get_root()->get_c();
		std::copy(contents_root->s.begin(), contents_root->s.begin() + this->N, V);
	}
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::recursive_allocate_nodes_contents(tools::Generic_node<Contents_MK_SC<B,R>>* node_curr, const int vector_size)
{
	node_curr->set_contents(new Contents_MK_SC<B,R>(vector_size));

	for (auto c : node_curr->get_children())
		this->recursive_allocate_nodes_contents(c, vector_size / 2);
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::recursive_initialize_frozen_bits(const tools::Generic_node<Contents_MK_SC<B,R>>* node_curr,
                                   const std::vector<bool>& frozen_bits)
{
	auto *contents = node_curr->get_contents();

	if (!node_curr->is_leaf()) // stop condition
	{
		for (auto c : node_curr->get_children())
			this->recursive_initialize_frozen_bits(c, frozen_bits); // recursive call
	}
	else
		contents->is_frozen_bit = frozen_bits[node_curr->get_lane_id()];
}

template <typename R>
R F(const R& lambda_a, const R& lambda_b)
{
	return tools::f_LLR<R>(lambda_a, lambda_b);
}

template <typename B, typename R>
R G(const R& lambda_a, const R& lambda_b, const B& u)
{
	return tools::g_LLR<B,R>(lambda_a, lambda_b, u);
}

template <typename B, typename R>
B H(const R& lambda_a)
{
	return tools::h_LLR<B,R>(lambda_a);
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::recursive_decode(const tools::Generic_node<Contents_MK_SC<B,R>>* node_curr)
{
	if (!node_curr->is_leaf()) // stop condition
	{
		const auto size = (int)node_curr->get_c()->lambda.size();
		const auto size_2 = size / 2;

		const auto *node_left  = node_curr->get_children()[0]; // get left node
		const auto *node_right = node_curr->get_children()[1]; // get right node

		for (auto i = 0; i < size_2; i++)
			node_left->get_c()->lambda[i] = F<R>(node_curr->get_c()->lambda[        i],  // apply f()
			                                     node_curr->get_c()->lambda[size_2 +i]);

		this->recursive_decode(node_left); // recursive call

		for (auto i = 0; i < size_2; i++)
			node_right->get_c()->lambda[i] = G<B,R>(node_curr->get_c()->lambda[        i], // apply g()
			                                        node_curr->get_c()->lambda[size_2 +i],
			                                        node_left->get_c()->s     [        i]);

		this->recursive_decode(node_right); // recursive call

		for (auto i = 0; i < size_2; i++)
			node_curr->get_c()->s[i] = node_left ->get_c()->s[i] ^ node_right->get_c()->s[i]; // bit xor

		for (auto i = 0; i < size_2; i++)
			node_curr->get_c()->s[size_2 +i] = node_right->get_c()->s[i]; // bit eq
	}
	else // specific leaf treatment
	{
		node_curr->get_c()->s[0] = (!node_curr->get_c()->is_frozen_bit && // if this is a frozen bit then s == 0
		                            H<B,R>(node_curr->get_c()->lambda[0])); // apply h()
	}
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::recursive_store(const tools::Generic_node<Contents_MK_SC<B,R>>* node_curr, B *V_K, int &k) const
{
	auto *contents = node_curr->get_contents();

	if (!node_curr->is_leaf()) // stop condition
		for (auto c : node_curr->get_children())
			this->recursive_store(c, V_K, k); // recursive call
	else
		if (!frozen_bits[node_curr->get_lane_id()])
			V_K[k++] = contents->s[0];
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::recursive_deallocate_nodes_contents(tools::Generic_node<Contents_MK_SC<B,R>>* node_curr)
{
	if (node_curr != nullptr)
	{
		for (auto c : node_curr->get_children())
			this->recursive_deallocate_nodes_contents(c); // recursive call

		auto *contents = node_curr->get_contents();
		delete contents;
		node_curr->set_contents(nullptr);
	}
}
}
}
