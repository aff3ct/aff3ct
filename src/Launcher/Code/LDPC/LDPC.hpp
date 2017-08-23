#ifndef LAUNCHER_LDPC_HPP_
#define LAUNCHER_LDPC_HPP_

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class LDPC : public L
{
public:
	LDPC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~LDPC();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();
};
}
}

#include "LDPC.hxx"

#endif /* LAUNCHER_LDPC_HPP_ */
