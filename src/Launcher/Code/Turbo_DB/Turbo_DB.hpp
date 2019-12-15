#ifndef LAUNCHER_TURBO_DB_HPP_
#define LAUNCHER_TURBO_DB_HPP_

#include <iostream>

#include "Factory/Tools/Codec/Turbo_DB/Codec_turbo_DB.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Turbo_DB : public L
{
protected:
	factory::Codec_turbo_DB *params_cdc;

public:
	Turbo_DB(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Turbo_DB() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_TURBO_DB_HPP_ */
