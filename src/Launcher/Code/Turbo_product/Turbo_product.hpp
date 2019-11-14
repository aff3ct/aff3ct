#ifndef LAUNCHER_TURBO_PRODUCT_HPP_
#define LAUNCHER_TURBO_PRODUCT_HPP_

#include <iostream>

#include "Factory/Tools/Codec/Turbo_product/Codec_turbo_product.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Turbo_product : public L
{
protected:
	factory::Codec_turbo_product *params_cdc;

public:
	Turbo_product(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Turbo_product() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_TURBO_PRODUCT_HPP_ */
