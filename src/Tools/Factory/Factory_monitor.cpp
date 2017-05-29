#include <stdexcept>

#include "Module/Monitor/Standard/Monitor_std.hpp"

#include "Factory_monitor.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Monitor<B>* Factory_monitor<B>
::build(const std::string type,
        const int         K,
        const int         fe,
        const int         n_frames)
{
	if (type == "STD") return new Monitor_std<B>(K, fe, n_frames);

	throw std::runtime_error("aff3ct::tools::Factory_monitor: the factory could not allocate the object.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_monitor<B_8>;
template struct aff3ct::tools::Factory_monitor<B_16>;
template struct aff3ct::tools::Factory_monitor<B_32>;
template struct aff3ct::tools::Factory_monitor<B_64>;
#else
template struct aff3ct::tools::Factory_monitor<B>;
#endif
// ==================================================================================== explicit template instantiation
