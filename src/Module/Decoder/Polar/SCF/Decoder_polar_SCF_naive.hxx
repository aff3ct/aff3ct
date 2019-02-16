#include "Decoder_polar_SCF_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G, tools::proto_h<B,R> H>
Decoder_polar_SCF_naive<B,R,F,G,H>
::Decoder_polar_SCF_naive(const int& K, const int& N, const std::vector<bool>& frozen_bits,
                          CRC<B>& crc, const int n_flips, const int n_frames)
: Decoder(K, N, n_frames, 1),
  Decoder_polar_SC_naive<B,R,F,G,H>(K, N, frozen_bits, n_frames),
  crc(crc),
  n_flips(n_flips),
  index(K),
  current_flip_index(-1)
{
	const std::string name = "Decoder_polar_SCF_naive";
	this->set_name(name);

	if (crc.get_size() > K)
	{
		std::stringstream message;
		message << "'crc.get_size()' has to be equal or smaller than 'K' ('crc.get_size()' = " << crc.get_size()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// get tree leaves
	leaves = this->polar_tree.get_leaves();
}



template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G, tools::proto_h<B,R> H>
void Decoder_polar_SCF_naive<B,R,F,G,H>
::recursive_decode(const tools::Binary_node<Contents_SC<B,R>>* node_curr)
{

	if (!node_curr->is_leaf()) // stop condition
	{
		const auto size = (int)node_curr->get_c()->lambda.size();
		const auto size_2 = size / 2;

		const auto *node_left  = node_curr->get_left(); // get left node
		const auto *node_right = node_curr->get_right(); // get right node

		for (auto i = 0; i < size_2; i++)
			node_left->get_c()->lambda[i] = F(node_curr->get_c()->lambda[        i],  // apply f()
			                                  node_curr->get_c()->lambda[size_2 +i]);

		this->recursive_decode(node_left); // recursive call

		for (auto i = 0; i < size_2; i++)
			node_right->get_c()->lambda[i] = G(node_curr->get_c()->lambda[        i], // apply g()
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
		                            H(node_curr->get_c()->lambda[0])); // apply h()

		if (current_flip_index == node_curr->get_lane_id())
			node_curr->get_c()->s[0] = !node_curr->get_c()->s[0];
	}

}

template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G, tools::proto_h<B,R> H>
void Decoder_polar_SCF_naive<B,R,F,G,H>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
	// initialization
	bool decode_result = false;
	int n_ite = 0;

	// initialize indices with inf bits indices
	// TODO: move to notify_frozen ?
	auto j = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i])
			index[j++] = i;

	// get tree leaves
	auto leaves = this->polar_tree.get_leaves();

//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE

	current_flip_index = -1;

	this->recursive_decode(this->polar_tree.get_root());

	// identify the n_flips weakest llrs
	std::partial_sort(index.begin(), index.begin() + n_flips, index.end(),
	                  [this](const int& a, const int& b)
	                  {return std::abs(this->leaves[a]->get_c()->lambda[0]) < std::abs(this->leaves[b]->get_c()->lambda[0]);}
	                 );

	decode_result = this->check_crc();

	while ((n_ite < n_flips) && (!decode_result))
	{
		current_flip_index = index[n_ite];

		this->recursive_decode(this->polar_tree.get_root());

		decode_result = this->check_crc();

		n_ite ++;
	}
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G, tools::proto_h<B,R> H>
void Decoder_polar_SCF_naive<B,R,F,G,H>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
	// initialization
	bool decode_result = false;
	int n_ite = 0;

	// initialize indices with inf bits indices
	// TODO: move to notify_frozen ?
	auto j = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i])
			index[j++] = i;

//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE

	current_flip_index = -1;

	this->recursive_decode(this->polar_tree.get_root());

	// identify the n_flips weakest llrs
	std::partial_sort(index.begin(), index.begin() + n_flips, index.end(),
	                  [this](const int& a, const int& b)
	                  {return std::abs(this->leaves[a]->get_c()->lambda[0]) < std::abs(this->leaves[b]->get_c()->lambda[0]);}
	                 );

	decode_result = check_crc();

	while ((n_ite < n_flips) && (!decode_result))
	{
		current_flip_index = index[n_ite];

		this->recursive_decode(this->polar_tree.get_root());

		decode_result = check_crc();

		n_ite ++;
	}
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_N, true);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
}

template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G, tools::proto_h<B,R> H>
bool Decoder_polar_SCF_naive<B,R,F,G,H>
::check_crc()
{
	std::vector<B> U_test;
	U_test.clear();
	for (auto leaf = 0 ; leaf < this->N ; leaf++)
		if (!this->frozen_bits[leaf])
			U_test.push_back(leaves[leaf]->get_c()->s[0]);
	return this->crc.check(U_test, this->get_simd_inter_frame_level());
}
}
}

