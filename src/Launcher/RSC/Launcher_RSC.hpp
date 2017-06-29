#ifndef LAUNCHER_RSC_HPP_
#define LAUNCHER_RSC_HPP_

#include "Tools/Factory/RSC/Factory_decoder_RSC.hpp"
#include "Tools/Factory/RSC/Factory_encoder_RSC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R, typename QD = Q>
class Launcher_RSC : public cLauncher
{
public:
	Launcher_RSC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_RSC();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();

	typename tools::Factory_encoder_RSC<B     >::encoder_parameters_RSC *m_enc = nullptr;
	typename tools::Factory_decoder_RSC<B,Q,QD>::decoder_parameters_RSC *m_dec = nullptr;
};
}
}

#include "Launcher_RSC.hxx"

#endif /* LAUNCHER_RSC_HPP_ */
