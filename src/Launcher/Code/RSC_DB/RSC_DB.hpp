#ifndef LAUNCHER_RSC_DB_HPP_
#define LAUNCHER_RSC_DB_HPP_

#include "Factory/Module/Code/RSC_DB/Decoder_RSC_DB.hpp"
#include "Factory/Module/Code/RSC_DB/Encoder_RSC_DB.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class RSC_DB : public C
{
protected:
	factory::Encoder_RSC_DB::parameters *params_enc = nullptr;
	factory::Decoder_RSC_DB::parameters *params_dec = nullptr;

public:
	RSC_DB(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~RSC_DB();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "RSC_DB.hxx"

#endif /* LAUNCHER_RSC_DB_HPP_ */
