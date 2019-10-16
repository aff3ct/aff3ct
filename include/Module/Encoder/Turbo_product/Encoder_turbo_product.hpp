#ifndef ENCODER_TURBO_PRODUCT_HPP_
#define ENCODER_TURBO_PRODUCT_HPP_

#include <vector>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_turbo_product : public Encoder<B>
{
protected:
	const Interleaver<B> &pi; // the interleaver

	Encoder<B> &enc_r;       // row encoder
	Encoder<B> &enc_c;       // col encoder

	const bool parity_extended;

	std::vector<B> X_N_i;     // internal buffer in the interleaved domain

public:
	Encoder_turbo_product(const Interleaver<B> &pi, Encoder<B> &enc_r, Encoder<B> &enc_c, const int n_frames = 1);
	virtual ~Encoder_turbo_product() = default;

	virtual void _encode(const B *U_K, B *X_N, const int frame_id); using Encoder<B>::encode;

	/* to use with a GENIUS BCH decoder to save data in the sub encoders : will work only with a inter frame level of 1
	   else data is overwritten by the following frames
private :

	// access to enc_r protected/private members thanks to this robber
	// source:  http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html
	template <typename Tag>
	struct result {
		typedef typename Tag::type type;
		static type ptr;
	};


	template<typename Tag, typename Tag::type p>
	struct rob : result<Tag> {
		struct filler {
		filler() { result<Tag>::ptr = p; }
		};
		static filler filler_obj;
	};

	struct Enc_X_N_mem { typedef std::vector<std::vector<B>> Encoder<B>::*type; };
	*/
};
}
}

#endif // ENCODER_TURBO_PRODUCT_HPP_
