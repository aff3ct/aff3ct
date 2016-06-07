#include "../../Channel/NO/Channel_NO.hpp"
#include "../../Channel/AWGN/Channel_AWGN_LR.hpp"
#include "../../Channel/AWGN/Channel_AWGN_LLR.hpp"
#ifdef CHANNEL_MKL
#include "../../Channel/AWGN_MKL/Channel_AWGN_MKL_LR.hpp"
#include "../../Channel/AWGN_MKL/Channel_AWGN_MKL_LLR.hpp"
#endif
#ifdef CHANNEL_GSL
#include "../../Channel/AWGN_GSL/Channel_AWGN_GSL_LR.hpp"
#include "../../Channel/AWGN_GSL/Channel_AWGN_GSL_LLR.hpp"
#endif

#include "Factory_channel.hpp"

template <typename B, typename R>	
Channel<B,R>* Factory_channel<B,R>
::build(const t_channel_param &chan_params, const R& sigma, const int seed, const R scaling_factor)
{
	Channel<B,R> *channel = nullptr;

	// build the channels
	if (chan_params.type == "AWGN")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_LLR<B,R>(sigma, seed +1, scaling_factor);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_LR<B,R>(sigma, seed +1, scaling_factor);
	}
#ifdef CHANNEL_MKL
	else if (chan_params.type == "AWGN_MKL")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_MKL_LLR<B,R>(sigma, seed +1, scaling_factor);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_MKL_LR<B,R>(sigma, seed +1, scaling_factor);
	}
#endif
#ifdef CHANNEL_GSL
	else if (chan_params.type == "AWGN_GSL")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_GSL_LLR<B,R>(sigma, seed +1, scaling_factor);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_GSL_LR<B,R>(sigma, seed +1, scaling_factor);
	}
#endif
	else if (chan_params.type == "NO")
	{
		channel = new Channel_NO<B,R>();
	}

	return channel;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_channel<B_8,R_8>;
template struct Factory_channel<B_16,R_16>;
template struct Factory_channel<B_32,R_32>;
template struct Factory_channel<B_64,R_64>;
#else
template struct Factory_channel<B,R>;
#endif
// ==================================================================================== explicit template instantiation
