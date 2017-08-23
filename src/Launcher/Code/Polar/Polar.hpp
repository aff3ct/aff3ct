#ifndef LAUNCHER_POLAR_HPP_
#define LAUNCHER_POLAR_HPP_

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Polar : public L
{
public:
	Polar(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Polar();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();
};
}
}

#include "Polar.hxx"

#endif /* LAUNCHER_POLAR_HPP_ */
