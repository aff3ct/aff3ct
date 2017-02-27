#ifndef LAUNCHER_BFERI_HPP_
#define LAUNCHER_BFERI_HPP_

#include "../Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class Launcher_BFERI : public Launcher<B,R,Q>
{
public:
	Launcher_BFERI(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFERI() {};

protected:
	virtual void build_args();
	virtual void store_args();

	virtual std::vector<std::pair<std::string,std::string>> header_simulation ();
	virtual std::vector<std::pair<std::string,std::string>> header_code       ();
	virtual std::vector<std::pair<std::string,std::string>> header_encoder    ();
	virtual std::vector<std::pair<std::string,std::string>> header_interleaver();
	virtual std::vector<std::pair<std::string,std::string>> header_demodulator();
	virtual std::vector<std::pair<std::string,std::string>> header_decoder    ();
	virtual std::vector<std::pair<std::string,std::string>> header_monitor    ();
};
}
}

#endif /* LAUNCHER_BFERI_HPP_ */
