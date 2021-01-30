/*!
 * \file
 * \brief Class module::Decoder_polar_SCAN_naive.
 */
#ifndef DECODER_POLAR_SCAN_NAIVE_H_
#define	DECODER_POLAR_SCAN_NAIVE_H_

#include <vector>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_f<  R> F = &tools::f_LLR,
                                                tools::proto_v<  R> V = &tools::v_LLR,
                                                tools::proto_h<B,R> H = &tools::h_LLR,
                                                tools::proto_i<  R> I = &tools::init_LLR,
                                                tools::proto_s<  R> S = &tools::sat_val>
class Decoder_polar_SCAN_naive : public Decoder_SISO<B,R>, public tools::Interface_get_set_frozen_bits
{
protected:
	const int m;            // coded bits log-length
	const int max_iter;
	const int layers_count; // number of layers in the graph = m+1

	std::vector<bool>           frozen_bits;
	std::vector<std::vector<R>> feedback_graph;
	std::vector<std::vector<R>> soft_graph;

public:
	Decoder_polar_SCAN_naive(const int &K, const int &N, const int &max_iter, const std::vector<bool> &frozen_bits);
	virtual ~Decoder_polar_SCAN_naive() = default;

	virtual Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>* clone() const;

	virtual void set_n_frames(const size_t n_frames);

	virtual void set_frozen_bits(const std::vector<bool>& frozen_bits);
	virtual const std::vector<bool>& get_frozen_bits() const;	

protected:
	void _reset(const size_t frame_id);

	        void _load          (const R *Y_N                                 );
	        int  _decode_siho   (const R *Y_N,  B *V_K , const size_t frame_id);
	        int  _decode_siho_cw(const R *Y_N,  B *V_N , const size_t frame_id);
	virtual int  _decode_siso   (const R *Y_N1, R *Y_N2, const size_t frame_id);
	virtual void _store         (               B *V_KN, bool coded = false   ) const;

	void _decode();

private:
	void set_soft_val_and_propagate(const int l, const int j, const R v);
	void fb_compute_soft_output    (const int &i);
	void display_decoder_graph     ();
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive.hxx"
#endif

#endif	/* DECODER_POLAR_SCAN_NAIVE_H_ */
