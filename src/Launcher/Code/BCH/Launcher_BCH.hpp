#ifndef LAUNCHER_BCH_HPP_
#define LAUNCHER_BCH_HPP_

#include "Tools/Factory/Module/Code/BCH/Factory_decoder_BCH.hpp"
#include "Tools/Factory/Module/Code/BCH/Factory_encoder_BCH.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class Launcher_BCH : public C
{
protected :
	tools::Factory_encoder_BCH::parameters *m_enc = nullptr;
	tools::Factory_decoder_BCH::parameters *m_dec = nullptr;

public:
	Launcher_BCH(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BCH();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();

};
}
}

#include "Launcher_BCH.hxx"

#endif /* LAUNCHER_BCH_HPP_ */
