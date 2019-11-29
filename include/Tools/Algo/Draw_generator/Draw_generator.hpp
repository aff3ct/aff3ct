/*!
 * \file
 * \brief Class tools::Draw_generator.
 */
#ifndef DRAW_GENERATOR_HPP_
#define DRAW_GENERATOR_HPP_

#ifndef _MSC_VER
#include "Tools/Interface/Interface_clone.hpp"
#endif
#include "Tools/Interface/Interface_set_seed.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
#ifdef _MSC_VER
class Draw_generator : public Interface_set_seed
#else
class Draw_generator : public Interface_set_seed, public Interface_clone
#endif
{
public:
	Draw_generator() = default;
	virtual ~Draw_generator() = default;
	virtual Draw_generator<R>* clone() const = 0;

	virtual void set_seed(const int seed) = 0;
};

}
}

#endif /* DRAW_GENERATOR_HPP_ */
