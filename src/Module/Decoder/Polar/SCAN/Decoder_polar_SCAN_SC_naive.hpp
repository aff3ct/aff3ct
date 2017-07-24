#ifndef DECODER_POLAR_SCAN_SC_NAIVE_H_
#define	DECODER_POLAR_SCAN_SC_NAIVE_H_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/utils.h"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"

#include "../../Decoder_SISO.hpp"
#include "../decoder_polar_functions.h"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float,
          proto_i<  R> I = tools::init_LLR,
		  proto_l<  R> L = tools::l_LLR,
          proto_f<  R> F = f_LLR,
          proto_v<  R> V = v_LLR,
          proto_h<B,R> H = h_LLR,
          proto_s<  R> S = tools::sat_val>
class Decoder_polar_SCAN_SC_naive : public Decoder_SISO<B,R>
{
protected:
	const int m;            // coded bits log-length
	const int max_iter;
	const int layers_count; // number of layers in the graph = m+1

	mipp::vector<int>      frozen_bits;


public:
	Decoder_polar_SCAN_SC_naive(const int &K, const int &N, const int &max_iter, const mipp::vector<int> &frozen_bits,
	                         const int n_frames = 1, const std::string name = "Decoder_polar_SCAN_SC_naive");
	virtual ~Decoder_polar_SCAN_SC_naive() {}
	tools::Frame_trace<> tracer;
	std::vector<mipp::vector<R>> feedback_graph;
	std::vector<mipp::vector<R>> soft_graph;
	std::vector<mipp::vector<R>> L2R_graph;
	std::vector<mipp::vector<R>> R2L_graph;

	void set_frozen_bits(const mipp::vector<int> &frozen_bits) {this->frozen_bits = frozen_bits;}

protected:
	        void _load       (const R *Y_N          );
	        void _hard_decode(const R *Y_N,  B *V_K );
	virtual void _soft_decode(const R *Y_N1, R *Y_N2);
	virtual void _store      (               B *V_K ) const;

	void _load_init();
	void _decode();

private:
	void set_soft_val_and_propagate(const int l, const int j, const R v);
	void fb_compute_soft_output    (const int &i);
	void display_decoder_graph     ();
	void display_decoder_graph_de  ();
	mipp::vector<R> soft_output;
};
}
}

#include "Decoder_polar_SCAN_SC_naive.hxx"

#endif	/* DECODER_POLAR_SCAN_SC_NAIVE_H_ */
