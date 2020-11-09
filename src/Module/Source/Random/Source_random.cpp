#include "Module/Source/Random/Source_random.hpp"

using namespace aff3ct::module;

template <typename B>
Source_random<B>
::Source_random(const int K, const int seed)
: Source<B>(K),
  rd_engine(seed),
  uniform_dist(0, 1)
{
	const std::string name = "Source_random";
	this->set_name(name);
}

template <typename B>
Source_random<B>* Source_random<B>
::clone() const
{
	auto m = new Source_random(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Source_random<B>
::_generate(B *U_K, const size_t frame_id)
{
	// generate a random k bits vector U_k
	for (auto i = 0; i < this->K; i++)
		U_K[i] = (B)this->uniform_dist(this->rd_engine);
}

template <typename B>
void Source_random<B>
::set_seed(const int seed)
{
	rd_engine.seed(seed);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Source_random<B_8>;
template class aff3ct::module::Source_random<B_16>;
template class aff3ct::module::Source_random<B_32>;
template class aff3ct::module::Source_random<B_64>;
#else
template class aff3ct::module::Source_random<B>;
#endif
// ==================================================================================== explicit template instantiation
