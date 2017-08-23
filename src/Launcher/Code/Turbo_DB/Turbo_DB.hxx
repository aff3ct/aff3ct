#include <iostream>

#include "Factory/Module/Codec/Turbo_DB/Codec_turbo_DB.hpp"

#include "Turbo_DB.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B, typename R, typename Q>
Turbo_DB<L,B,R,Q>
::Turbo_DB(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream)
{
}

template <class L, typename B, typename R, typename Q>
Turbo_DB<L,B,R,Q>
::~Turbo_DB()
{
}

template <class L, typename B, typename R, typename Q>
void Turbo_DB<L,B,R,Q>
::build_args()
{
	factory::Codec_turbo_DB::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",  "F"});
	this->opt_args.erase({"enc-seed", "S"});
	this->opt_args.erase({"itl-seed", "S"});

	L::build_args();
}

template <class L, typename B, typename R, typename Q>
void Turbo_DB<L,B,R,Q>
::store_args()
{
	factory::Codec_turbo_DB::store_args(this->ar.get_args(), this->params.cdc);

	if (std::is_same<Q,int8_t>())
	{
		this->params.qnt.n_bits     = 6;
		this->params.qnt.n_decimals = 2;
	}
	else if (std::is_same<Q,int16_t>())
	{
		this->params.qnt.n_bits     = 6;
		this->params.qnt.n_decimals = 3;
	}

	L::store_args();

	this->params.cdc.enc.n_frames = this->params.src.n_frames;
	this->params.cdc.pct.n_frames = this->params.src.n_frames;
	this->params.cdc.dec.n_frames = this->params.src.n_frames;

	this->params.cdc.enc.itl.core.seed     = this->params.global_seed;
	this->params.cdc.dec.itl.core.seed     = this->params.global_seed;
	this->params.cdc.enc.itl.core.n_frames = this->params.src.n_frames;
	this->params.cdc.dec.itl.core.n_frames = this->params.src.n_frames;

	this->params.cdc.enc.sub.n_frames = this->params.cdc.enc.n_frames;
	this->params.cdc.dec.sub.n_frames = this->params.cdc.dec.n_frames;

	if (this->params.err_track_revert)
	{
		this->params.cdc.enc.itl.core.type = "USER";
		this->params.cdc.dec.itl.core.type = "USER";
		this->params.cdc.enc.itl.core.path = this->params.err_track_path + std::string("_$snr.itl");
		this->params.cdc.dec.itl.core.path = this->params.err_track_path + std::string("_$snr.itl");
	}
}

template <class L, typename B, typename R, typename Q>
void Turbo_DB<L,B,R,Q>
::print_header()
{
	factory::params_list trash;
	auto &pl_enc = (this->params.cdc.enc.type != "NO") ? this->pl_enc : trash;
	auto &pl_pct = (this->params.cdc.pct.type != "NO") ? this->pl_pct : trash;

	factory::Codec_turbo_DB::make_header(pl_enc, this->pl_dec, pl_pct, this->pl_itl, this->params.cdc, false);

	L::print_header();
}
}
}
