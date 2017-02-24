#ifndef LAUNCHER_BFER_POLAR_HPP_
#define LAUNCHER_BFER_POLAR_HPP_

#include "../Launcher_BFER.hpp"

namespace aff3ct
{
template <typename B, typename R, typename Q>
class Launcher_BFER_polar : public Launcher_BFER<B,R,Q>
{
public:
	Launcher_BFER_polar(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER_polar() {};

protected:
	virtual void build_args  ();
	virtual void store_args  ();
	virtual Simulation* build_simu  ();

	virtual std::vector<std::pair<std::string,std::string>> header_code   ();
	virtual std::vector<std::pair<std::string,std::string>> header_crc    ();
	virtual std::vector<std::pair<std::string,std::string>> header_decoder();
};
}

#endif /* LAUNCHER_BFER_POLAR_HPP_ */
