#include "Module/Channel/NO/Channel_NO.hpp"
#include "Module/Channel/Additive/User/Channel_additive_user.hpp"
#include "Module/Channel/Additive/AWGN/Standard/Channel_AWGN_std_LR.hpp"
#include "Module/Channel/Additive/AWGN/Standard/Channel_AWGN_std_LLR.hpp"
#include "Module/Channel/Additive/AWGN/Fast/Channel_AWGN_fast_LR.hpp"
#include "Module/Channel/Additive/AWGN/Fast/Channel_AWGN_fast_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR.hpp"
#ifdef CHANNEL_MKL
#include "Module/Channel/Additive/AWGN/MKL/Channel_AWGN_MKL_LR.hpp"
#include "Module/Channel/Additive/AWGN/MKL/Channel_AWGN_MKL_LLR.hpp"
#endif
#ifdef CHANNEL_GSL
#include "Module/Channel/Additive/AWGN/GSL/Channel_AWGN_GSL_LR.hpp"
#include "Module/Channel/Additive/AWGN/GSL/Channel_AWGN_GSL_LLR.hpp"
#endif

#include "Factory_channel.hpp"
using namespace aff3ct;

template <typename R>	
Channel<R>* Factory_channel<R>
::build(const parameters &params, const R& sigma, const int size, const int seed)
{
	Channel<R> *channel = nullptr;

	// build the channels
	if (params.channel.type == "AWGN")
	{
		if (params.channel.domain == "LLR")
			channel = new Channel_AWGN_std_LLR<R>(size, sigma, seed +1);
		else if (params.channel.domain == "LR")
			channel = new Channel_AWGN_std_LR<R>(size, sigma, seed +1);
	}
	else if (params.channel.type == "AWGN_FAST")
	{
		if (params.channel.domain == "LLR")
			channel = new Channel_AWGN_fast_LLR<R>(size, sigma, seed +1);
		else if (params.channel.domain == "LR")
			channel = new Channel_AWGN_fast_LR<R>(size, sigma, seed +1);
	}
#ifdef CHANNEL_MKL
	else if (params.channel.type == "AWGN_MKL")
	{
		if (params.channel.domain == "LLR")
			channel = new Channel_AWGN_MKL_LLR<R>(size, sigma, seed +1);
		else if (params.channel.domain == "LR")
			channel = new Channel_AWGN_MKL_LR<R>(size, sigma, seed +1);
	}
#endif
#ifdef CHANNEL_GSL
	else if (params.channel.type == "AWGN_GSL")
	{
		if (params.channel.domain == "LLR")
			channel = new Channel_AWGN_GSL_LLR<R>(size, sigma, seed +1);
		else if (params.channel.domain == "LR")
			channel = new Channel_AWGN_GSL_LR<R>(size, sigma, seed +1);
	}
#endif
	else if (params.channel.type == "RAYLEIGH")
	{
		if (params.channel.domain == "LLR")
		{
			if (params.channel.block_fading == "NO")
				channel = new Channel_Rayleigh_LLR<R>(size, sigma, params.modulator.complex, seed +1);
			else if (params.channel.block_fading == "ONETAP")
				channel = nullptr;
			else if (params.channel.block_fading == "FRAME")
				channel = nullptr;
		}
	}
	else if (params.channel.type == "NO")
	{
		channel = new Channel_NO<R>(size);
	}
	else if (params.channel.type == "USER")
	{
		channel = new Channel_additive_user<R>(size, params.channel.path);
	}

	return channel;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_channel<R_32>;
template struct Factory_channel<R_64>;
#else
template struct Factory_channel<R>;
#endif
// ==================================================================================== explicit template instantiation
