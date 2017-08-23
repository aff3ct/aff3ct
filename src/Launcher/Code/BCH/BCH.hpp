#ifndef LAUNCHER_BCH_HPP_
#define LAUNCHER_BCH_HPP_

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class BCH : public L
{
public:
	BCH(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~BCH();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();
};
}
}

#include "BCH.hxx"

#endif /* LAUNCHER_BCH_HPP_ */
