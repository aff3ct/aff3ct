#ifndef LAUNCHER_RA_HPP_
#define LAUNCHER_RA_HPP_

#include "Factory/Module/Code/RA/Decoder_RA.hpp"
#include "Factory/Module/Code/RA/Encoder_RA.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class RA : public C
{
protected:
	factory::Encoder_RA::parameters *params_enc = nullptr;
	factory::Decoder_RA::parameters *params_dec = nullptr;

public:
	RA(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RA();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "RA.hxx"

#endif /* LAUNCHER_RA_HPP_ */
