#ifndef LAUNCHER_TURBO_DB_HPP_
#define LAUNCHER_TURBO_DB_HPP_

#include "Factory/Module/Code/Turbo_DB/Decoder_turbo_DB.hpp"
#include "Factory/Module/Code/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Code/Turbo_DB/Puncturer_turbo_DB.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B = int, typename R = float, typename Q = R>
class Turbo_DB : public C
{
protected:
	factory::Encoder_turbo_DB  ::parameters *params_enc = nullptr;
	factory::Puncturer_turbo_DB::parameters *params_pct = nullptr;
	factory::Decoder_turbo_DB  ::parameters *params_dec = nullptr;

public:
	Turbo_DB(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Turbo_DB();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();

	virtual void build_codec();
};
}
}

#include "Turbo_DB.hxx"

#endif /* LAUNCHER_TURBO_DB_HPP_ */
