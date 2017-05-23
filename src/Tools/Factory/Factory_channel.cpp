#include "Module/Channel/NO/Channel_NO.hpp"
#include "Module/Channel/User/Channel_user.hpp"
#include "Module/Channel/AWGN/Channel_AWGN_LLR.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR.hpp"

#include "Tools/Algo/Noise/Standard/Noise_std.hpp"
#include "Tools/Algo/Noise/Fast/Noise_fast.hpp"

#ifdef CHANNEL_MKL
#include "Tools/Algo/Noise/MKL/Noise_MKL.hpp"
#endif
#ifdef CHANNEL_GSL
#include "Tools/Algo/Noise/GSL/Noise_GSL.hpp"
#endif

#include "Factory_channel.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename R>	
Channel<R>* Factory_channel<R>
::build(const parameters &params, const R& sigma, const int size, const int seed)
{
	Channel<R> *channel = nullptr;

	// build the channels
	if (params.channel.domain == "LLR")
	{
		if (params.channel.type == "AWGN")
		{
			channel = new Channel_AWGN_LLR<R>(size, sigma, new tools::Noise_std<R>(seed), params.simulation.inter_frame_level);
		}
		else if (params.channel.type == "AWGN_FAST")
		{
			channel = new Channel_AWGN_LLR<R>(size, sigma, new tools::Noise_fast<R>(seed), params.simulation.inter_frame_level);
		}
#ifdef CHANNEL_MKL
		else if (params.channel.type == "AWGN_MKL")
		{
			channel = new Channel_AWGN_LLR<R>(size, sigma, new tools::Noise_MKL<R>(seed), params.simulation.inter_frame_level);
		}
#endif
#ifdef CHANNEL_GSL
		else if (params.channel.type == "AWGN_GSL")
		{
			channel = new Channel_AWGN_LLR<R>(size, sigma, new tools::Noise_GSL<R>(seed), params.simulation.inter_frame_level);
		}
#endif
		else if (params.channel.type == "RAYLEIGH" && params.channel.block_fading == "NO")
		{
			channel = new Channel_Rayleigh_LLR<R>(size, sigma, params.modulator.complex, new tools::Noise_std<R>(seed), params.simulation.inter_frame_level);
		}
		else if (params.channel.type == "RAYLEIGH_FAST" && params.channel.block_fading == "NO")
		{
			channel = new Channel_Rayleigh_LLR<R>(size, sigma, params.modulator.complex, new tools::Noise_fast<R>(seed), params.simulation.inter_frame_level);
		}
#ifdef CHANNEL_MKL
		else if (params.channel.type == "RAYLEIGH_MKL" && params.channel.block_fading == "NO")
		{
			channel = new Channel_Rayleigh_LLR<R>(size, sigma, params.modulator.complex, new tools::Noise_MKL<R>(seed), params.simulation.inter_frame_level);
		}
#endif
#ifdef CHANNEL_GSL
		else if (params.channel.type == "RAYLEIGH_GSL" && params.channel.block_fading == "NO")
		{
			channel = new Channel_Rayleigh_LLR<R>(size, sigma, params.modulator.complex, new tools::Noise_GSL<R>(seed), params.simulation.inter_frame_level);
		}
#endif
		else if (params.channel.type == "USER")
		{
			channel = new Channel_user<R>(size, params.channel.path, params.simulation.inter_frame_level);
		}
		else if (params.channel.type == "NO")
		{
			channel = new Channel_NO<R>(size, params.simulation.inter_frame_level);
		}
	}

	return channel;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_channel<R_32>;
template struct aff3ct::tools::Factory_channel<R_64>;
#else
template struct aff3ct::tools::Factory_channel<R>;
#endif
// ==================================================================================== explicit template instantiation
