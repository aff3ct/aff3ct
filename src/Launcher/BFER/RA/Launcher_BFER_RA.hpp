#ifndef LAUNCHER_BFER_RA_HPP_
#define LAUNCHER_BFER_RA_HPP_

#include "../Launcher_BFER.hpp"

template <typename B, typename R, typename Q>
class Launcher_BFER_RA : public Launcher_BFER<B,R,Q>
{
public:
	Launcher_BFER_RA(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER_RA() {};

protected:
	virtual void build_args  ();
	virtual void store_args  ();
	virtual void print_header();
	virtual void build_simu  ();
};

#endif /* LAUNCHER_BFER_RA_HPP_ */
