#ifndef LAUNCHER_RA_HPP_
#define LAUNCHER_RA_HPP_

#include "Tools/Factory/RA/Factory_decoder_RA.hpp"
#include "Tools/Factory/RA/Factory_encoder_RA.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R>
class Launcher_RA : public cLauncher
{
public:
	Launcher_RA(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_RA();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();

	typename tools::Factory_encoder_RA<B  >::encoder_parameters    *m_enc = nullptr;
	typename tools::Factory_decoder_RA<B,Q>::decoder_parameters_RA *m_dec = nullptr;
};
}
}

#include "Launcher_RA.hxx"

#endif /* LAUNCHER_RA_HPP_ */
