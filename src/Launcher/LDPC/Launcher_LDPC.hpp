#ifndef LAUNCHER_LDPC_HPP_
#define LAUNCHER_LDPC_HPP_

#include "Tools/Factory/LDPC/Factory_decoder_LDPC.hpp"
#include "Tools/Factory/LDPC/Factory_encoder_LDPC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R>
class Launcher_LDPC : public cLauncher
{
public:
	Launcher_LDPC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_LDPC();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();

	typename tools::Factory_encoder_LDPC<B  >::encoder_parameters      *m_enc = nullptr;
	typename tools::Factory_decoder_LDPC<B,Q>::decoder_parameters_LDPC *m_dec = nullptr;
};
}
}

#include "Launcher_LDPC.hxx"

#endif /* LAUNCHER_LDPC_HPP_ */
