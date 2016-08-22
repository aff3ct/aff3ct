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
::build(const t_code_param &code_params, const t_channel_param &chan_params, const R& sigma, const int seed)
{
	Channel<R> *channel = nullptr;

	// build the channels
	if (chan_params.type == "AWGN")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_std_LLR<R>(code_params.N + code_params.tail_length, sigma, seed +1);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_std_LR<R>(code_params.N + code_params.tail_length, sigma, seed +1);
	}
	else if (chan_params.type == "AWGN_FAST")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_fast_LLR<R>(code_params.N + code_params.tail_length, sigma, seed +1);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_fast_LR<R>(code_params.N + code_params.tail_length, sigma, seed +1);
	}
#ifdef CHANNEL_MKL
	else if (chan_params.type == "AWGN_MKL")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_MKL_LLR<R>(code_params.N + code_params.tail_length, sigma, seed +1);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_MKL_LR<R>(code_params.N + code_params.tail_length, sigma, seed +1);
	}
#endif
#ifdef CHANNEL_GSL
	else if (chan_params.type == "AWGN_GSL")
	{
		if (chan_params.domain == "LLR")
			channel = new Channel_AWGN_GSL_LLR<R>(code_params.N + code_params.tail_length, sigma, seed +1);
		else if (chan_params.domain == "LR")
			channel = new Channel_AWGN_GSL_LR<R>(code_params.N + code_params.tail_length, sigma, seed +1);
	}
#endif
	else if (chan_params.type == "NO")
	{
		channel = new Channel_NO<R>(code_params.N + code_params.tail_length);
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
