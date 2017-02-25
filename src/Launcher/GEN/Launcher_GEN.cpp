#include <string>
#include <iostream>

#include "Tools/Display/bash_tools.h"

#include "Launcher_GEN.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
Launcher_GEN<B,R,Q>
::Launcher_GEN(const int argc, const char **argv, std::ostream &stream)
: Launcher<B,R,Q>(argc, argv, stream)
{
	this->params.simulation.type       = "GEN";
	this->params.encoder   .systematic = true;
}

template <typename B, typename R, typename Q>
void Launcher_GEN<B,R,Q>
::build_args()
{
	Launcher<B,R,Q>::build_args();
}

template <typename B, typename R, typename Q>
void Launcher_GEN<B,R,Q>
::store_args()
{
	Launcher<B,R,Q>::store_args();
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_GEN<B_8,R_8,Q_8>;
template class Launcher_GEN<B_16,R_16,Q_16>;
template class Launcher_GEN<B_32,R_32,Q_32>;
template class Launcher_GEN<B_64,R_64,Q_64>;
#else
template class Launcher_GEN<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
