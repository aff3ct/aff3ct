#include "Module/Quantizer/Standard/Quantizer_standard.hpp"
#include "Module/Quantizer/Fast/Quantizer_fast.hpp"
#include "Module/Quantizer/Tricky/Quantizer_tricky.hpp"
#include "Module/Quantizer/NO/Quantizer_NO.hpp"

#include "Factory_quantizer.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename R, typename Q>
Quantizer<R,Q>* Factory_quantizer<R,Q>
::build(const std::string type,
        const int         size,
        const int         n_decimals,
        const int         n_bits,
        const float       sigma,
        const float       range,
        const int         n_frames)
{
	     if (type == "STD"     ) return new Quantizer_standard<R,Q>(size, n_decimals, n_bits,        n_frames);
	else if (type == "STD_FAST") return new Quantizer_fast    <R,Q>(size, n_decimals, n_bits,        n_frames);
	else if (type == "TRICKY"  ) return new Quantizer_tricky  <R,Q>(size, range,      n_bits, sigma, n_frames);
	else if (type == "NO"      ) return new Quantizer_NO      <R,Q>(size,                            n_frames);

	return nullptr;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_quantizer<R_8,Q_8>;
template struct aff3ct::tools::Factory_quantizer<R_16,Q_16>;
template struct aff3ct::tools::Factory_quantizer<R_32,Q_32>;
template struct aff3ct::tools::Factory_quantizer<R_64,Q_64>;
#else
template struct aff3ct::tools::Factory_quantizer<R,Q>;
#endif
// ==================================================================================== explicit template instantiation
