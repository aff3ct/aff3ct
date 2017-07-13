#ifndef LAUNCHER_NO_HPP_
#define LAUNCHER_NO_HPP_

#include "Tools/Factory/Module/Code/NO/Factory_decoder_NO.hpp"
#include "Tools/Factory/Module/Code/Factory_encoder.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class Launcher_NO : public C
{
protected:
	tools::Factory_encoder   ::parameters *m_enc = nullptr;
	tools::Factory_decoder_NO::parameters *m_dec = nullptr;

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
