#ifndef LAUNCHER_TURBO_PRODUCT_CODE_HPP_
#define LAUNCHER_TURBO_PRODUCT_CODE_HPP_

#include "Factory/Module/Codec/Turbo_product_code/Codec_turbo_product_code.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Turbo_product_code : public L
{
protected:
	factory::Codec_turbo_product_code::parameters *params_cdc;

public:
	Turbo_product_code(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Turbo_product_code();

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_TURBO_PRODUCT_CODE_HPP_ */
