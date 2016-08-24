#include "../../Quantizer/Quantizer_standard.hpp"
#include "../../Quantizer/Quantizer_fast.hpp"
#include "../../Quantizer/Quantizer_tricky.hpp"
#include "../../Quantizer/Quantizer_NO.hpp"

#include "Factory_quantizer.hpp"

template <typename R, typename Q>
Quantizer<R,Q>* Factory_quantizer<R,Q>
::build(const t_code_param &code_params, const t_channel_param &chan_params, const R& sigma, const int size)
{
	Quantizer<R,Q> *quantizer = nullptr;

	// build the quantizer
	if (typeid(R) == typeid(Q))
		quantizer = new Quantizer_NO<R,Q>(size);
	else
	{
		if (chan_params.quantizer_type == "STD")
			quantizer = new Quantizer_standard<R,Q>(size, chan_params.quant_point_pos, chan_params.quant_n_bits);
		else if (chan_params.quantizer_type == "STD_FAST")
			quantizer = new Quantizer_fast<R,Q>(size, chan_params.quant_point_pos, chan_params.quant_n_bits);
		else if (chan_params.quantizer_type == "TRICKY")
		{
			if (chan_params.quant_min_max == 0.f)
				quantizer = new Quantizer_tricky<R,Q>(size, (short)chan_params.quant_n_bits, sigma); // auto mode
			else
				quantizer = new Quantizer_tricky<R,Q>(size, (R)chan_params.quant_min_max, chan_params.quant_n_bits, sigma);
		}
	}

	return quantizer;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_quantizer<R_8,Q_8>;
template struct Factory_quantizer<R_16,Q_16>;
template struct Factory_quantizer<R_32,Q_32>;
template struct Factory_quantizer<R_64,Q_64>;
#else
template struct Factory_quantizer<R,Q>;
#endif
// ==================================================================================== explicit template instantiation