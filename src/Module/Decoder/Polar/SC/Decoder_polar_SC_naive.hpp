#ifndef DECODER_POLAR_SC_NAIVE_
#define DECODER_POLAR_SC_NAIVE_

#include <vector>

#include "Tools/Algo/Tree/Binary_node.hpp"
#include "Tools/Algo/Tree/Binary_tree.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Code/Polar/Frozenbits_notifier.hpp"
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
class Decoder_polar_SC_naive : public Decoder_SIHO<B,R>, public tools::Frozenbits_notifier
{
protected:
	const int m; // graph depth

	const std::vector<bool> &frozen_bits;
	tools::Binary_tree<Contents_SC<B,R>> polar_tree;

public:
	Decoder_polar_SC_naive(const int& K, const int& N, const std::vector<bool>& frozen_bits, const int n_frames = 1);
	virtual ~Decoder_polar_SC_naive();

	virtual void notify_frozenbits_update();

protected:
	        void _load           (const R *Y_N                                         );
	virtual void _decode_siho    (const R *Y_N, B *V_K, const int frame_id             );
	virtual void _decode_siho_cw (const R *Y_N, B *V_N, const int frame_id             );
	virtual void _store          (              B *V,   bool coded = false             ) const;
	virtual void recursive_decode(const tools::Binary_node<Contents_SC<B,R>>* node_curr);

private:
	void recursive_allocate_nodes_contents  (      tools::Binary_node<Contents_SC<B,R>>* node_curr, const int vector_size               );
	void recursive_initialize_frozen_bits   (const tools::Binary_node<Contents_SC<B,R>>* node_curr, const std::vector<bool> &frozen_bits);
	void recursive_store                    (const tools::Binary_node<Contents_SC<B,R>>* node_curr, B *V_K, int &k                      ) const;
	void recursive_deallocate_nodes_contents(      tools::Binary_node<Contents_SC<B,R>>* node_curr                                      );
};
}
}

#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive.hxx"

#endif /* DECODER_POLAR_SC_NAIVE_ */
