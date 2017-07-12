#ifndef LAUNCHER_RA_HPP_
#define LAUNCHER_RA_HPP_

#include "Tools/Factory/Code/RA/Factory_decoder_RA.hpp"
#include "Tools/Factory/Code/RA/Factory_encoder_RA.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R>
class Launcher_RA : public cLauncher
{
protected:
	tools::Factory_encoder_RA::parameters *m_enc = nullptr;
	tools::Factory_decoder_RA::parameters *m_dec = nullptr;

public:
	Launcher_RA(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_RA();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Launcher_RA.hxx"

#endif /* LAUNCHER_RA_HPP_ */
