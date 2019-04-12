#ifndef DECODER_POLAR_MK_SC_NAIVE_
#define DECODER_POLAR_MK_SC_NAIVE_

#include <vector>

#include "Tools/Algo/Tree/Generic/Generic_tree.hpp"
#include "Tools/Code/Polar/Frozenbits_notifier.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"

#include "../../Decoder_SIHO.hpp"

namespace aff3ct
{
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
class Decoder_polar_MK_SC_naive : public Decoder_SIHO<B,R>, public tools::Frozenbits_notifier
{
	friend Decoder_polar_MK_ASCL_naive_CA<B,R>;

protected:
	const tools::Polar_code &code;
	const std::vector<bool> &frozen_bits;
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
	                          const int                n_frames = 1);
	virtual ~Decoder_polar_MK_SC_naive();

	virtual void notify_frozenbits_update();

protected:
	        void _load          (const R *Y_N                            );
	        void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	        void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);
	virtual void _store         (              B *V,   bool coded = false) const;

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
