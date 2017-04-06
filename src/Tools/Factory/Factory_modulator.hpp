#ifndef FACTORY_MODULATOR_HPP
#define FACTORY_MODULATOR_HPP

#include "Module/Modulator/Modulator.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float, typename Q = R>
struct Factory_modulator : public Factory
{
	static module::Modulator<B,R,Q>* build(const parameters &params, const float sigma);

	static int get_buffer_size_after_modulation(const parameters &params, const int N);

	static int get_buffer_size_after_filtering (const parameters &params, const int N);
};
}
}

#endif /* FACTORY_MODULATOR_HPP */
