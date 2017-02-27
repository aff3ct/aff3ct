#ifndef DECODER_POLAR_SCAN_NAIVE_H_
#define	DECODER_POLAR_SCAN_NAIVE_H_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../../Decoder.hpp"
#include "../decoder_polar_functions.h"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float,
          proto_i<R> I = tools::init_LLR, proto_f<R> F = f_LLR, proto_v<R> V = v_LLR, proto_h<B,R> H = h_LLR>
class Decoder_polar_SCAN_naive : public Decoder<B,R>
{
protected:
	const int m;            // coded bits log-length
	const int max_iter;
	const int layers_count; // number of layers in the graph = m+1

	const mipp::vector<B>&       frozen_bits;
	std::vector<mipp::vector<R>> feedback_graph;
	std::vector<mipp::vector<R>> soft_graph;

public:
	Decoder_polar_SCAN_naive(const int &K, const int &m, const int &max_iter, const mipp::vector<B> &frozen_bits,
	                         const int n_frames = 1, const std::string name = "Decoder_polar_SCAN_naive");
	virtual ~Decoder_polar_SCAN_naive() {}

protected:
	        void  load       (const mipp::vector<R>& Y_N);
	        void _hard_decode(                          );
	virtual void  store      (      mipp::vector<B>& V_K) const;

	void load_init();
	void decode();

private:
	void set_soft_val_and_propagate(const int l, const int j, const R v);
	void fb_compute_soft_output    (const int &i);
	void display_decoder_graph     ();
};
}
}

#include "Decoder_polar_SCAN_naive.hxx"

#endif	/* DECODER_POLAR_SCAN_NAIVE_H_ */
