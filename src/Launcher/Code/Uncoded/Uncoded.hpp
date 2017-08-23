#ifndef LAUNCHER_NO_HPP_
#define LAUNCHER_NO_HPP_

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Uncoded : public L
{
public:
	Uncoded(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Uncoded();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();
};
}
}

#include "Uncoded.hxx"

#endif /* LAUNCHER_NO_HPP_ */
