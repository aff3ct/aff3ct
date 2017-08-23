#ifndef LAUNCHER_RA_HPP_
#define LAUNCHER_RA_HPP_

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class RA : public L
{
public:
	RA(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RA();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();
};
}
}

#include "RA.hxx"

#endif /* LAUNCHER_RA_HPP_ */
