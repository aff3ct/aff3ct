#ifndef DECODER_RSC_BCJR_STD_GENERIC_SEQ_JSON_HPP_
#define DECODER_RSC_BCJR_STD_GENERIC_SEQ_JSON_HPP_

#include <vector>
#include <iostream>
#include <mipp.h>

#include "Tools/Math/max.h"

#include "Decoder_RSC_BCJR_seq_generic_std.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename RD = float,
          tools::proto_max<R> MAX1 = tools::max, tools::proto_max<RD> MAX2 = tools::max>
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
	                                      const int n_frames = 1);
	virtual ~Decoder_RSC_BCJR_seq_generic_std_json() = default;

protected:
	virtual void _decode_siso(const R *sys, const R *par, R *ext, const int frame_id);
	void compute_ext(const R *sys, R *ext);
};
}
}

#include "Decoder_RSC_BCJR_seq_generic_std_json.hxx"

#endif /* DECODER_RSC_BCJR_STD_GENERIC_SEQ_JSON_HPP_ */
