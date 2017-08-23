#ifndef LAUNCHER_RSC_DB_HPP_
#define LAUNCHER_RSC_DB_HPP_

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class RSC_DB : public L
{
public:
	RSC_DB(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RSC_DB();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();
};
}
}

#include "RSC_DB.hxx"

#endif /* LAUNCHER_RSC_DB_HPP_ */
