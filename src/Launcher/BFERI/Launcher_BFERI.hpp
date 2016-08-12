#ifndef LAUNCHER_BFERI_HPP_
#define LAUNCHER_BFERI_HPP_

#include "../Launcher.hpp"

template <typename B, typename R, typename Q>
class Launcher_BFERI : public Launcher<B,R,Q>
{
public:
	Launcher_BFERI(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFERI() {};

protected:
	virtual void build_args  ();
	virtual void store_args  ();
	virtual void print_header();
};

#endif /* LAUNCHER_BFERI_HPP_ */