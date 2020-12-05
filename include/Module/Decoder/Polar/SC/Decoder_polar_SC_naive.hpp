/*!
 * \file
 * \brief Class module::Decoder_polar_SC_naive.
 */
#ifndef DECODER_POLAR_SC_NAIVE_
#define DECODER_POLAR_SC_NAIVE_

#include <vector>

#include "Tools/Algo/Tree/Binary/Binary_node.hpp"
#include "Tools/Algo/Tree/Binary/Binary_tree.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Contents_SC
{
public:
	std::vector<R> lambda;
	std::vector<B> s;
	bool           is_frozen_bit;

	explicit Contents_SC(int size) : lambda(size), s(size), is_frozen_bit(false) {}
	virtual ~Contents_SC() {}
};

template <typename B = int, typename R = float, tools::proto_f<  R> F = tools::f_LLR,
                                                tools::proto_g<B,R> G = tools::g_LLR,
                                                tools::proto_h<B,R> H = tools::h_LLR>
class Decoder_polar_SC_naive : public Decoder_SIHO<B,R>, public tools::Interface_get_set_frozen_bits
{
protected:
	const int m; // graph depth

	std::vector<bool> frozen_bits;
	tools::Binary_tree<Contents_SC<B,R>> polar_tree;

public:
	Decoder_polar_SC_naive(const int& K, const int& N, const std::vector<bool>& frozen_bits);
	virtual ~Decoder_polar_SC_naive();

	virtual Decoder_polar_SC_naive<B,R,F,G,H>* clone() const;

	virtual void set_frozen_bits(const std::vector<bool>& frozen_bits);
	virtual const std::vector<bool>& get_frozen_bits() const;

protected:
	virtual void deep_copy          (const Decoder_polar_SC_naive<B,R,F,G,H>& m);
	        void recursive_deep_copy(const tools::Binary_node<Contents_SC<B,R>> *nref,
	                                       tools::Binary_node<Contents_SC<B,R>> *nclone);
	        void _load              (const R *Y_N                                         );
	virtual int  _decode_siho       (const R *Y_N, B *V_K, const size_t frame_id          );
	virtual int  _decode_siho_cw    (const R *Y_N, B *V_N, const size_t frame_id          );
	virtual void _store             (              B *V,   bool coded = false             ) const;
	virtual void recursive_decode   (const tools::Binary_node<Contents_SC<B,R>>* node_curr);

private:
	void recursive_allocate_nodes_contents  (      tools::Binary_node<Contents_SC<B,R>>* node_curr, const int vector_size               );
	void recursive_initialize_frozen_bits   (const tools::Binary_node<Contents_SC<B,R>>* node_curr, const std::vector<bool> &frozen_bits);
	void recursive_store                    (const tools::Binary_node<Contents_SC<B,R>>* node_curr, B *V_K, int &k                      ) const;
	void recursive_deallocate_nodes_contents(      tools::Binary_node<Contents_SC<B,R>>* node_curr                                      );
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive.hxx"
#endif

#endif /* DECODER_POLAR_SC_NAIVE_ */
