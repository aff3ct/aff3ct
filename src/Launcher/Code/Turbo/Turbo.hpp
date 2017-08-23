#ifndef LAUNCHER_TURBO_HPP_
#define LAUNCHER_TURBO_HPP_

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Turbo : public L
{
public:
	Turbo(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Turbo();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();
};
}
}

#include "Turbo.hxx"

#endif /* LAUNCHER_TURBO_HPP_ */
