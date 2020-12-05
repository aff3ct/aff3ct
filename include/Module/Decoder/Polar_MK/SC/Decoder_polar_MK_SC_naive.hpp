/*!
 * \file
 * \brief Class module::Decoder_polar_MK_SC_naive.
 */
#ifndef DECODER_POLAR_MK_SC_NAIVE_
#define DECODER_POLAR_MK_SC_NAIVE_

#include <functional>
#include <cstdint>
#include <vector>

#include "Tools/Algo/Tree/Generic/Generic_tree.hpp"
#include "Tools/Algo/Tree/Generic/Generic_node.hpp"
#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
class Frozenbits_generator_GA;
}
namespace module
{
template <typename B = int, typename R = float>
class Contents_MK_SC
{
public:
	std::vector<R> l;
	std::vector<B> s;
	bool           is_frozen_bit;

	explicit Contents_MK_SC(int size) : l(size), s(size), is_frozen_bit(false) {}
	virtual ~Contents_MK_SC() {}
};

template <typename B, typename R>
class Decoder_polar_MK_ASCL_naive_CA;

template <typename B = int, typename R = float>
class Decoder_polar_MK_SC_naive : public Decoder_SIHO<B,R>, public tools::Interface_get_set_frozen_bits
{
	friend Decoder_polar_MK_ASCL_naive_CA<B,R>;
	friend tools::Frozenbits_generator_GA;

protected:
	const tools::Polar_code &code;
	std::vector<bool> frozen_bits;
	tools::Generic_tree<Contents_MK_SC<B,R>> polar_tree;
	std::vector<std::vector<B>> Ke;
	std::vector<uint32_t> idx;
	std::vector<B> u;

	std::vector<R> LLRs;
	std::vector<B> bits;
	std::vector<std::vector<std::function<R(const std::vector<R> &LLRs, const std::vector<B> &bits)>>> lambdas;

public:
	Decoder_polar_MK_SC_naive(const int&               K,
	                          const int&               N,
	                          const tools::Polar_code& code,
	                          const std::vector<bool>& frozen_bits,
	                          const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
	                                                                        const std::vector<B> &bits)>>> &lambdas);

	Decoder_polar_MK_SC_naive(const int&               K,
	                          const int&               N,
	                          const tools::Polar_code& code,
	                          const std::vector<bool>& frozen_bits);

	virtual ~Decoder_polar_MK_SC_naive();

	virtual Decoder_polar_MK_SC_naive<B,R>* clone() const;

	virtual void set_frozen_bits(const std::vector<bool>& frozen_bits);
	virtual const std::vector<bool>& get_frozen_bits() const;

protected:
	virtual void deep_copy          (const Decoder_polar_MK_SC_naive<B,R>& m );
	        void recursive_deep_copy(const tools::Generic_node<Contents_MK_SC<B,R>> *nref,
	                                       tools::Generic_node<Contents_MK_SC<B,R>> *nclone);

	        void _load          (const R *Y_N                               );
	        int  _decode_siho   (const R *Y_N, B *V_K, const size_t frame_id);
	        int  _decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id);
	virtual void _store         (              B *V,   bool coded = false   ) const;

private:
	void recursive_allocate_nodes_contents  (      tools::Generic_node<Contents_MK_SC<B,R>>* node_curr, const int vector_size               );
	void recursive_initialize_frozen_bits   (const tools::Generic_node<Contents_MK_SC<B,R>>* node_curr, const std::vector<bool> &frozen_bits);
	void recursive_decode                   (const tools::Generic_node<Contents_MK_SC<B,R>>* node_curr                                      );
	void recursive_store                    (const tools::Generic_node<Contents_MK_SC<B,R>>* node_curr, B *V_K, int &k                      ) const;
	void recursive_deallocate_nodes_contents(      tools::Generic_node<Contents_MK_SC<B,R>>* node_curr                                      );
};
}
}

#endif /* DECODER_POLAR_MK_SC_NAIVE_ */
