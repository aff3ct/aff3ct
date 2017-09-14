#ifndef LAUNCHER_LDPC_HPP_
#define LAUNCHER_LDPC_HPP_

#include "Factory/Module/Codec/LDPC/Codec_LDPC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class LDPC : public L
{
protected:
	factory::Codec_LDPC::parameters *params_cdc;

public:
	LDPC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~LDPC();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();
};
}
}

#endif /* LAUNCHER_LDPC_HPP_ */
