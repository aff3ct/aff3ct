#include <string>
#include <sstream>
#include <type_traits>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/LDPC/BF/PPBF/Decoder_LDPC_probabilistic_parallel_bit_flipping.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_LDPC_probabilistic_parallel_bit_flipping<B,R>
::Decoder_LDPC_probabilistic_parallel_bit_flipping(const int &K, const int &N, const int& n_ite,
                                                   const tools::Sparse_matrix &H,
                                                   const std::vector<unsigned> &info_bits_pos,
                                                   const std::vector<float> &bernouilli_probas,
                                                   const bool enable_syndrome,
                                                   const int syndrome_depth,
                                                   const int seed)
: Decoder_LDPC_bit_flipping_hard<B,R>(K, N, n_ite, H, info_bits_pos, enable_syndrome, syndrome_depth),
  rd_engine(seed)
{
	const std::string name = "Decoder_LDPC_probabilistic_parallel_bit_flipping";
	this->set_name(name);

	if (std::is_same<R, signed char>::value)
	{
		std::stringstream message;
		message << "This decoder does not work in 8-bit fixed-point (try in 16-bit).";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (bernouilli_probas.size() != (this->H.get_rows_max_degree() + 2))
	{
		std::stringstream message;
		message << "'bernouilli_probas.size()' must be equal to the biggest variable node degree plus 2"
		        << "('bernouilli_probas.size() = '" << bernouilli_probas.size() << ", 'variable node max degree' = "
		        << this->H.get_rows_max_degree() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	// generate Bernouilli distributions
	for (unsigned i = 0; i < bernouilli_probas.size(); i++)
		bernouilli_dist.push_back(std::bernoulli_distribution(bernouilli_probas[i]));
}

template <typename B, typename R>
Decoder_LDPC_probabilistic_parallel_bit_flipping<B,R>* Decoder_LDPC_probabilistic_parallel_bit_flipping<B,R>
::clone() const
{
	auto m = new Decoder_LDPC_probabilistic_parallel_bit_flipping(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_LDPC_probabilistic_parallel_bit_flipping<B,R>
::cn_process(const B *VN, B *CN, const size_t frame_id)
{
	// for each check nodes
	const auto n_chk_nodes = (int)this->H.get_n_cols();
	for (auto c = 0; c < n_chk_nodes; c++)
	{
		const auto& chk_node = this->H.get_col_to_rows()[c];
		const auto chk_degree = chk_node.size();

		CN[c] = 0;
		for (unsigned v = 0; v < chk_degree; v++)
			CN[c] ^= VN[chk_node[v]];
	}
}

template <typename B, typename R>
void Decoder_LDPC_probabilistic_parallel_bit_flipping<B,R>
::vn_process(const B *Y_N, B *VN, const B *CN, const size_t frame_id)
{
	// for each variable nodes
	const auto n_var_nodes = (int)this->H.get_n_rows();
	for (auto v = 0; v < n_var_nodes; v++)
	{
		const auto& var_node = this->H.get_row_to_cols()[v];
		const auto var_degree = var_node.size();

		auto energy = VN[v] ^ Y_N[v];
		for (unsigned c = 0; c < var_degree; c++)
			energy += CN[var_node[c]];

		VN[v] ^= (B)(bernouilli_dist[energy])(this->rd_engine);
	}
}

template <typename B, typename R>
void Decoder_LDPC_probabilistic_parallel_bit_flipping<B,R>
::set_seed(const int seed)
{
	rd_engine.seed(seed);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_LDPC_probabilistic_parallel_bit_flipping<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_probabilistic_parallel_bit_flipping<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_probabilistic_parallel_bit_flipping<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_probabilistic_parallel_bit_flipping<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_probabilistic_parallel_bit_flipping<B,Q>;
#endif
// ==================================================================================== explicit template instantiation

