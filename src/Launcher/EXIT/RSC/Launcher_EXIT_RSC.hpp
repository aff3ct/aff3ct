#ifndef LAUNCHER_EXIT_RSC_HPP_
#define LAUNCHER_EXIT_RSC_HPP_

#include "../Launcher_EXIT.hpp"

template <typename B, typename R, typename Q, typename QD>
class Launcher_EXIT_RSC : public Launcher_EXIT<B,R,Q>
{
public:
	Launcher_EXIT_RSC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_EXIT_RSC() {};

protected:
	virtual void build_args();
	virtual void store_args();
	virtual Simulation* build_simu();

	virtual std::vector<std::pair<std::string,std::string>> header_decoder();
};

#endif /* LAUNCHER_EXIT_RSC_HPP_ */
