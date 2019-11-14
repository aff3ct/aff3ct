#ifndef LAUNCHER_LDPC_HPP_
#define LAUNCHER_LDPC_HPP_

#include <iostream>

#include "Factory/Tools/Codec/LDPC/Codec_LDPC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class LDPC : public L
{
protected:
	factory::Codec_LDPC *params_cdc;

public:
	LDPC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~LDPC() = default;

protected:
	virtual void get_description_args();
	virtual void store_args();
};
}
}

#endif /* LAUNCHER_LDPC_HPP_ */
