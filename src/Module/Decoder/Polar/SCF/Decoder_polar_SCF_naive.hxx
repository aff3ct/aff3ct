#include "Decoder_polar_SCF_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G, tools::proto_h<B,R> H>
Decoder_polar_SCF_naive<B,R,F,G,H>
::Decoder_polar_SCF_naive(const int& K, const int& N, const std::vector<bool>& frozen_bits, 
                          CRC<B>& crc, const int n_frames)
: Decoder(K, N, n_frames, 1),
  Decoder_polar_SC_naive<B,R,F,G,H>(K, N, frozen_bits, n_frames),
  crc(crc)
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
	}
}



}
}
