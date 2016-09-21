#ifndef LAUNCHER_BFER_RSC_HPP_
#define LAUNCHER_BFER_RSC_HPP_

#include "../Launcher_BFER.hpp"

template <typename B, typename R, typename Q, typename QD>
class Launcher_BFER_RSC : public Launcher_BFER<B,R,Q>
{
public:
	Launcher_BFER_RSC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER_RSC() {};

protected:
	virtual void build_args();
	virtual void store_args();
	virtual Simulation* build_simu();

	virtual std::vector<std::pair<std::string,std::string>> header_encoder();
	virtual std::vector<std::pair<std::string,std::string>> header_decoder();
};

#endif /* LAUNCHER_BFER_RSC_HPP_ */
