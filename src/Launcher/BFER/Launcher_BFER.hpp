#ifndef LAUNCHER_BFER_HPP_
#define LAUNCHER_BFER_HPP_

#include "../Launcher.hpp"

template <typename B, typename R, typename Q>
class Launcher_BFER : public Launcher<B,R,Q>
{
public:
	Launcher_BFER(const int argc, const char **argv);
	virtual ~Launcher_BFER() {};

protected:
	virtual void build_args  ();
	virtual void store_args  ();
	virtual void print_header();
};

#endif /* LAUNCHER_BFER_HPP_ */