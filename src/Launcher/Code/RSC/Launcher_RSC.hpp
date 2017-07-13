#ifndef LAUNCHER_RSC_HPP_
#define LAUNCHER_RSC_HPP_

#include "Tools/Factory/Module/Code/RSC/Factory_decoder_RSC.hpp"
#include "Tools/Factory/Module/Code/RSC/Factory_encoder_RSC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R, typename QD = Q>
class Launcher_RSC : public C
{
protected:
	tools::Factory_encoder_RSC::parameters *m_enc = nullptr;
	tools::Factory_decoder_RSC::parameters *m_dec = nullptr;

public:
	Launcher_RSC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_RSC();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Launcher_RSC.hxx"

#endif /* LAUNCHER_RSC_HPP_ */
