#ifndef LAUNCHER_TURBO_HPP_
#define LAUNCHER_TURBO_HPP_

#include "Factory/Module/Code/Turbo/Decoder_turbo.hpp"
#include "Factory/Module/Code/Turbo/Encoder_turbo.hpp"
#include "Factory/Module/Code/Turbo/Puncturer_turbo.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R, typename QD = Q>
class Turbo : public C
{
protected:
	factory::Encoder_turbo  ::parameters<> *params_enc = nullptr;
	factory::Puncturer_turbo::parameters   *params_pct = nullptr;
	factory::Decoder_turbo  ::parameters<> *params_dec = nullptr;

public:
	Turbo(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Turbo();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Turbo.hxx"

#endif /* LAUNCHER_TURBO_HPP_ */
