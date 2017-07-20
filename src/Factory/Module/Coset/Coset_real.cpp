#include "Tools/Exception/exception.hpp"

#include "Module/Coset/Real/Coset_real.hpp"

#include "Coset_real.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B, typename Q>
module::Coset<B,Q>* Coset_real
::build(const std::string type,
        const int         N,
        const int         n_frames)
{
	if (type == "STD") return new module::Coset_real<B,Q>(N, n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Coset<B_8 ,Q_8 >* aff3ct::factory::Coset_real::build<B_8 ,Q_8 >(const std::string, const int, const int);
template aff3ct::module::Coset<B_16,Q_16>* aff3ct::factory::Coset_real::build<B_16,Q_16>(const std::string, const int, const int);
template aff3ct::module::Coset<B_32,Q_32>* aff3ct::factory::Coset_real::build<B_32,Q_32>(const std::string, const int, const int);
template aff3ct::module::Coset<B_64,Q_64>* aff3ct::factory::Coset_real::build<B_64,Q_64>(const std::string, const int, const int);
#else
template aff3ct::module::Coset<B,Q>* aff3ct::factory::Coset_real::build<B,Q>(const std::string, const int, const int);
#endif
// ==================================================================================== explicit template instantiation
