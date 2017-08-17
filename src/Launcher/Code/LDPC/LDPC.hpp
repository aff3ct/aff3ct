#ifndef LAUNCHER_LDPC_HPP_
#define LAUNCHER_LDPC_HPP_

#include "Factory/Module/Code/LDPC/Decoder_LDPC.hpp"
#include "Factory/Module/Code/LDPC/Encoder_LDPC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class LDPC : public C
{
protected:
	factory::Encoder_LDPC::parameters *params_enc = nullptr;
	factory::Decoder_LDPC::parameters *params_dec = nullptr;

public:
	LDPC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~LDPC();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "LDPC.hxx"

#endif /* LAUNCHER_LDPC_HPP_ */
