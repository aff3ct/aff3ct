#include <algorithm>
#include <sstream>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/fb_assert.h"
#include "Module/Decoder/Polar_MK/SC/Decoder_polar_MK_SC_naive.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

std::vector<uint32_t> gen_sequence(const tools::Polar_code& code)
{
	std::vector<uint32_t> sequence(code.get_stages().size());
	for (size_t s  = 0; s < code.get_stages().size(); s++)
		sequence[s] = code.get_kernel_matrices()[code.get_stages()[(sequence.size()-1)-s]].size();

	return sequence;
}

template <typename B, typename R>
Decoder_polar_MK_SC_naive<B,R>
::Decoder_polar_MK_SC_naive(const int& K,
                            const int& N,
                            const tools::Polar_code& code,
                            const std::vector<bool>& frozen_bits,
                            const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
                                                                          const std::vector<B> &bits)>>> &lambdas)
: Decoder_SIHO<B,R>(K, N),
  code(code),
  frozen_bits(frozen_bits),
  polar_tree(gen_sequence(code)),
  Ke(code.get_kernel_matrices().size()),
  idx(code.get_biggest_kernel_size()),
  u(code.get_biggest_kernel_size()),
  LLRs(code.get_biggest_kernel_size()),
  bits(code.get_biggest_kernel_size() -1),
  lambdas(code.get_kernel_matrices().size())
{
	const std::string name = "Decoder_polar_MK_SC_naive";
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

	this->recursive_allocate_nodes_contents(this->polar_tree.get_root(), this->N);
	this->recursive_initialize_frozen_bits(this->polar_tree.get_root(), frozen_bits);

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
Decoder_polar_MK_SC_naive<B,R>
::Decoder_polar_MK_SC_naive(const int& K,
                            const int& N,
                            const tools::Polar_code& code,
                            const std::vector<bool>& frozen_bits)
: Decoder_polar_MK_SC_naive<B,R>(K, N, code, frozen_bits, {})
{
}

template <typename B, typename R>
Decoder_polar_MK_SC_naive<B,R>
::~Decoder_polar_MK_SC_naive()
{
	this->recursive_deallocate_nodes_contents(this->polar_tree.get_root());
}

template <typename B, typename R>
Decoder_polar_MK_SC_naive<B,R>* Decoder_polar_MK_SC_naive<B,R>
::clone() const
{
	auto m = new Decoder_polar_MK_SC_naive(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::deep_copy(const Decoder_polar_MK_SC_naive<B,R> &m)
{
	Module::deep_copy(m);
	this->recursive_deep_copy(m.polar_tree.get_root(), this->polar_tree.get_root());
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::recursive_deep_copy(const tools::Generic_node<Contents_MK_SC<B,R>> *nref, tools::Generic_node<Contents_MK_SC<B,R>> *nclone)
{
	auto cref = nref->get_contents();
	auto cclone = new Contents_MK_SC<B,R>(*cref);
	nclone->set_contents(cclone);

	if (!nref->is_leaf() && !nclone->is_leaf())
		for (size_t c = 0; c < nref->get_children().size(); c++)
			if (nref->get_children()[c] != nullptr)
				this->recursive_deep_copy(nref->get_children()[c], nclone->get_children()[c]);
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::set_frozen_bits(const std::vector<bool>& fb)
{
	aff3ct::tools::fb_assert(frozen_bits, this->K, this->N);
	std::copy(fb.begin(), fb.end(), this->frozen_bits.begin());
	this->recursive_initialize_frozen_bits(this->polar_tree.get_root(), fb);
}

template <typename B, typename R>
const std::vector<bool>& Decoder_polar_MK_SC_naive<B,R>
::get_frozen_bits() const
{
	return this->frozen_bits;
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::_load(const R *Y_N)
{
	auto *contents = this->polar_tree.get_root()->get_contents();

	for (auto i = 0; i < this->N; i++)
		contents->l[i] = Y_N[i];
}

template <typename B, typename R>
int Decoder_polar_MK_SC_naive<B,R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
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

	return 0;
}

template <typename B, typename R>
int Decoder_polar_MK_SC_naive<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
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

	return 0;
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

	if (!node_curr->is_leaf())
	{
		const auto stage = polar_tree.get_depth() - node_curr->get_depth() -2;
		const auto new_vector_size = vector_size / this->code.get_kernel_matrices()[code.get_stages()[stage]].size();
		for (auto c : node_curr->get_children())
			this->recursive_allocate_nodes_contents(c, new_vector_size);
	}
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::recursive_initialize_frozen_bits(const tools::Generic_node<Contents_MK_SC<B,R>>* node_curr,
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
void Decoder_polar_MK_SC_naive<B,R>
::recursive_decode(const tools::Generic_node<Contents_MK_SC<B,R>>* node_curr)
{
	if (!node_curr->is_leaf()) // stop condition
	{
		const auto stage = polar_tree.get_depth() - node_curr->get_depth() -2;
		const auto kern_size = (int)node_curr->get_children().size();
		const auto size = (int)node_curr->get_c()->l.size();
		const auto n_kernels = size / kern_size;

		for (auto child = 0; child < kern_size; child++)
		{
			const auto *node_child = node_curr->get_children()[child];
			for (auto k = 0; k < n_kernels; k++)
			{
				for (auto l = 0; l < kern_size; l++) LLRs[l] = node_curr->get_c()->l[l * n_kernels +k];
				for (auto c = 0; c < child;     c++) bits[c] = node_curr->get_children()[c]->get_c()->s[k];

				node_child->get_c()->l[k] = lambdas[this->code.get_stages()[stage]][child](LLRs, bits);
			}

			this->recursive_decode(node_child); // recursive call
		}

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
				this->u[i] = node_curr->get_children()[(this->idx[i]/n_kernels)]->get_c()->s[this->idx[i]%n_kernels];
			}

			encode_polar_kernel(this->u.data(),
			                    this->idx.data(),
			                    this->Ke[code.get_stages()[stage]].data(),
			                    node_curr->get_c()->s.data(),
			                    kern_size);
		}
	}
	else // specific leaf treatment
	{
		node_curr->get_c()->s[0] = (!node_curr->get_c()->is_frozen_bit && // if this is a frozen bit then s == 0
		                            tools::h_LLR<B,R>(node_curr->get_c()->l[0])); // apply h()
	}
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::recursive_store(const tools::Generic_node<Contents_MK_SC<B,R>>* node_curr, B *V_K, int &k) const
{
	if (!node_curr->is_leaf()) // stop condition
		for (auto c : node_curr->get_children())
			this->recursive_store(c, V_K, k); // recursive call
	else
		if (!frozen_bits[node_curr->get_lane_id()])
			V_K[k++] = node_curr->get_contents()->s[0];
}

template <typename B, typename R>
void Decoder_polar_MK_SC_naive<B,R>
::recursive_deallocate_nodes_contents(tools::Generic_node<Contents_MK_SC<B,R>>* node_curr)
{
	for (auto c : node_curr->get_children())
		this->recursive_deallocate_nodes_contents(c); // recursive call

	delete node_curr->get_contents();
	node_curr->set_contents(nullptr);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_polar_MK_SC_naive<B_8,Q_8>;
template class aff3ct::module::Decoder_polar_MK_SC_naive<B_16,Q_16>;
template class aff3ct::module::Decoder_polar_MK_SC_naive<B_32,Q_32>;
template class aff3ct::module::Decoder_polar_MK_SC_naive<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_polar_MK_SC_naive<B,Q>;
#if !defined(AFF3CT_64BIT_PREC)
template class aff3ct::module::Decoder_polar_MK_SC_naive<B_64,Q_64>;
#endif
#endif
// ==================================================================================== explicit template instantiation
