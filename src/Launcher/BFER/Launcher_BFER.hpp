#ifndef LAUNCHER_BFER_HPP_
#define LAUNCHER_BFER_HPP_

#include "../Launcher.hpp"

namespace aff3ct
{
template <typename B, typename R, typename Q>
class Launcher_BFER : public Launcher<B,R,Q>
{
public:
	Launcher_BFER(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER() {};

protected:
	virtual void build_args  ();
	virtual void store_args  ();

	virtual std::vector<std::pair<std::string,std::string>> header_simulation();
	virtual std::vector<std::pair<std::string,std::string>> header_code      ();
	virtual std::vector<std::pair<std::string,std::string>> header_encoder   ();
	virtual std::vector<std::pair<std::string,std::string>> header_decoder   ();
	virtual std::vector<std::pair<std::string,std::string>> header_monitor   ();
};
}

#endif /* LAUNCHER_BFER_HPP_ */
