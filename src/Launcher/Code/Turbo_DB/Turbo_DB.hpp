#ifndef LAUNCHER_TURBO_DB_HPP_
#define LAUNCHER_TURBO_DB_HPP_

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Turbo_DB : public L
{
public:
	Turbo_DB(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Turbo_DB();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();
};
}
}

#include "Turbo_DB.hxx"

#endif /* LAUNCHER_TURBO_DB_HPP_ */
