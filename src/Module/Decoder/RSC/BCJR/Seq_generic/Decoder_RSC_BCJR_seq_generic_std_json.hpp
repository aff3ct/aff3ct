#ifndef DECODER_RSC_BCJR_STD_GENERIC_SEQ_JSON_HPP_
#define DECODER_RSC_BCJR_STD_GENERIC_SEQ_JSON_HPP_

#include <vector>
#include <iostream>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/max.h"

#include "Decoder_RSC_BCJR_seq_generic_std.hpp"

namespace aff3ct
{
template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
class Decoder_RSC_BCJR_seq_generic_std_json : public Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
{
private:
	const int n_ite;
		  int ite_counter;
	std::ostream &stream;

public:
	Decoder_RSC_BCJR_seq_generic_std_json(const int &K, 
	                                      const std::vector<std::vector<int>> &trellis,
	                                      const int n_ite = 1, 
	                                      const bool buffered_encoding = true,
	                                      std::ostream &stream = std::cout,
	                                      const int n_frames = 1,
	                                      const std::string name = "Decoder_RSC_BCJR_seq_generic_std_json");
	virtual ~Decoder_RSC_BCJR_seq_generic_std_json();

	virtual void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

protected:
	void compute_ext(const mipp::vector<R> &sys, mipp::vector<R> &ext);
};
}

#include "Decoder_RSC_BCJR_seq_generic_std_json.hxx"

#endif /* DECODER_RSC_BCJR_STD_GENERIC_SEQ_JSON_HPP_ */
