#include "../../Channel/NO/Channel_NO.hpp"
#include "../../Channel/AWGN/Standard/Channel_AWGN_std_LR.hpp"
#include "../../Channel/AWGN/Standard/Channel_AWGN_std_LLR.hpp"
#include "../../Channel/AWGN/Fast/Channel_AWGN_fast_LR.hpp"
#include "../../Channel/AWGN/Fast/Channel_AWGN_fast_LLR.hpp"
#ifdef CHANNEL_MKL
#include "../../Channel/AWGN/MKL/Channel_AWGN_MKL_LR.hpp"
#include "../../Channel/AWGN/MKL/Channel_AWGN_MKL_LLR.hpp"
#endif
#ifdef CHANNEL_GSL
#include "../../Channel/AWGN/GSL/Channel_AWGN_GSL_LR.hpp"
#include "../../Channel/AWGN/GSL/Channel_AWGN_GSL_LLR.hpp"
#endif

#include "Factory_channel.hpp"

template <typename R>	
Channel<R>* Factory_channel<R>
::build(const t_channel_param &chan_params, const R& sigma, const int seed, R scaling_factor)
{
	Channel<R> *channel = nullptr;

	if (!chan_params.estimator)
		scaling_factor = (R)1;

	// build the channels
	if (chan_params.type == "AWGN")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_std_LLR<R>(sigma, seed +1, scaling_factor);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_std_LR<R>(sigma, seed +1, scaling_factor);
	}
	else if (chan_params.type == "AWGN_FAST")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_fast_LLR<R>(sigma, seed +1, scaling_factor);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_fast_LR<R>(sigma, seed +1, scaling_factor);
	}
#ifdef CHANNEL_MKL
	else if (chan_params.type == "AWGN_MKL")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_MKL_LLR<R>(sigma, seed +1, scaling_factor);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_MKL_LR<R>(sigma, seed +1, scaling_factor);
	}
#endif
#ifdef CHANNEL_GSL
	else if (chan_params.type == "AWGN_GSL")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_GSL_LLR<R>(sigma, seed +1, scaling_factor);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_GSL_LR<R>(sigma, seed +1, scaling_factor);
	}
#endif
	else if (chan_params.type == "NO")
	{
		channel = new Channel_NO<R>();
	}

	return channel;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_channel<R_32>;
template struct Factory_channel<R_64>;
#else
template struct Factory_channel<R>;
#endif
// ==================================================================================== explicit template instantiation
