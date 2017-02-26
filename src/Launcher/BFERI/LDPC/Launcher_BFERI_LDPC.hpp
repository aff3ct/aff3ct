#ifndef LAUNCHER_BFERI_LDPC_HPP_
#define LAUNCHER_BFERI_LDPC_HPP_

#include "../Launcher_BFERI.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class Launcher_BFERI_LDPC : public Launcher_BFERI<B,R,Q>
{
public:
	Launcher_BFERI_LDPC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFERI_LDPC() {};

protected:
	virtual void build_args();
	virtual void store_args();
	virtual simulation::Simulation* build_simu();

	virtual std::vector<std::pair<std::string,std::string>> header_code   ();
	virtual std::vector<std::pair<std::string,std::string>> header_decoder();
};
}
}

#endif /* LAUNCHER_BFERI_LDPC_HPP_ */
