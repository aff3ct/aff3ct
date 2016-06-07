#ifndef LAUNCHER_EXIT_HPP_
#define LAUNCHER_EXIT_HPP_

#include "../Launcher.hpp"

template <typename B, typename R, typename Q>
class Launcher_EXIT : public Launcher<B,R,Q>
{
public:
	Launcher_EXIT(const int argc, const char **argv);
	virtual ~Launcher_EXIT() {};

protected:
	virtual void build_args  ();
	virtual void store_args  ();
	virtual void print_header();
};

#endif /* LAUNCHER_EXIT_HPP_ */