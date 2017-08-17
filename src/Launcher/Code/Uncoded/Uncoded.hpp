#ifndef LAUNCHER_NO_HPP_
#define LAUNCHER_NO_HPP_

#include "Factory/Module/Code/NO/Decoder_NO.hpp"
#include "Factory/Module/Code/Encoder.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class Uncoded : public C
{
protected:
	factory::Encoder   ::parameters *params_enc = nullptr;
	factory::Decoder_NO::parameters *params_dec = nullptr;

public:
	Uncoded(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Uncoded();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Uncoded.hxx"

#endif /* LAUNCHER_NO_HPP_ */
