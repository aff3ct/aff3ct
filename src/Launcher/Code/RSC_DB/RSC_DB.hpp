#ifndef LAUNCHER_RSC_DB_HPP_
#define LAUNCHER_RSC_DB_HPP_

#include <iostream>

#include "Factory/Tools/Codec/RSC_DB/Codec_RSC_DB.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class RSC_DB : public L
{
protected:
	factory::Codec_RSC_DB *params_cdc;

public:
	RSC_DB(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RSC_DB() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_RSC_DB_HPP_ */
