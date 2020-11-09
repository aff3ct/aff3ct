/*!
 * \file
 * \brief Class module::Encoder_turbo_product.
 */
#ifndef ENCODER_TURBO_PRODUCT_HPP_
#define ENCODER_TURBO_PRODUCT_HPP_

#include <memory>
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
	std::shared_ptr<Interleaver<B>> pi; // the interleaver

	std::shared_ptr<Encoder<B>> enc_r; // row encoder
	std::shared_ptr<Encoder<B>> enc_c; // col encoder

	const bool parity_extended;

	std::vector<B> X_N_i; // internal buffer in the interleaved domain

public:
	Encoder_turbo_product(const Encoder<B> &enc_r, const Encoder<B> &enc_c, const Interleaver<B> &pi);
	virtual ~Encoder_turbo_product() = default;
	virtual Encoder_turbo_product<B>* clone() const;

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void _encode(const B *U_K, B *X_N, const size_t frame_id);

	virtual void deep_copy(const Encoder_turbo_product<B> &m);

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
