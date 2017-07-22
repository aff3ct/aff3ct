#ifndef LAUNCHER_BCH_HPP_
#define LAUNCHER_BCH_HPP_

#include "Factory/Module/Code/BCH/Decoder_BCH.hpp"
#include "Factory/Module/Code/BCH/Encoder_BCH.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class Launcher_BCH : public C
{
protected :
	factory::Encoder_BCH::parameters *params_enc = nullptr;
	factory::Decoder_BCH::parameters *params_dec = nullptr;

public:
	Launcher_BCH(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BCH();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();

	virtual void build_codec();

};
}
}

#include "Launcher_BCH.hxx"

#endif /* LAUNCHER_BCH_HPP_ */
