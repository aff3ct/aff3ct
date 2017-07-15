#ifndef LAUNCHER_LDPC_HPP_
#define LAUNCHER_LDPC_HPP_

#include "Tools/Factory/Module/Code/LDPC/Factory_decoder_LDPC.hpp"
#include "Tools/Factory/Module/Code/LDPC/Factory_encoder_LDPC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class Launcher_LDPC : public C
{
protected:
	tools::Factory_encoder_LDPC::parameters *m_enc = nullptr;
	tools::Factory_decoder_LDPC::parameters *m_dec = nullptr;

public:
	Launcher_LDPC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_LDPC();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Launcher_LDPC.hxx"

#endif /* LAUNCHER_LDPC_HPP_ */
