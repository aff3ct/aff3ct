#ifndef LAUNCHER_NO_HPP_
#define LAUNCHER_NO_HPP_

#include "Tools/Factory/NO/Factory_decoder_NO.hpp"
#include "Tools/Factory/Factory_encoder_common.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R>
class Launcher_NO : public cLauncher
{
protected:
	typename tools::Factory_encoder_common<B  >::encoder_parameters *m_enc = nullptr;
	typename tools::Factory_decoder_NO    <B,Q>::decoder_parameters *m_dec = nullptr;

public:
	Launcher_NO(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_NO();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Launcher_NO.hxx"

#endif /* LAUNCHER_NO_HPP_ */
