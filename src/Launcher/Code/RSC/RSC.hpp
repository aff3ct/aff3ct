#ifndef LAUNCHER_RSC_HPP_
#define LAUNCHER_RSC_HPP_

#include "Factory/Module/Code/RSC/Decoder_RSC.hpp"
#include "Factory/Module/Code/RSC/Encoder_RSC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R, typename QD = Q>
class RSC : public C
{
protected:
	factory::Encoder_RSC::parameters *params_enc = nullptr;
	factory::Decoder_RSC::parameters *params_dec = nullptr;

public:
	RSC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RSC();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "RSC.hxx"

#endif /* LAUNCHER_RSC_HPP_ */
