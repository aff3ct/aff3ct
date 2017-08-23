#ifndef LAUNCHER_RSC_HPP_
#define LAUNCHER_RSC_HPP_

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class RSC : public L
{
public:
	RSC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RSC();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();
};
}
}

#include "RSC.hxx"

#endif /* LAUNCHER_RSC_HPP_ */
