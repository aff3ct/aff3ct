#include <iostream>
#include <mipp.h>

#include "Factory/Module/Codec/Turbo/Codec_turbo.hpp"

#include "Turbo.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B, typename R, typename Q>
Turbo<L,B,R,Q>
::Turbo(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream)
{
}

template <class L, typename B, typename R, typename Q>
Turbo<L,B,R,Q>
::~Turbo()
{
}

template <class L, typename B, typename R, typename Q>
void Turbo<L,B,R,Q>
::build_args()
{
	factory::Codec_turbo::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",  "F"});
	this->opt_args.erase({"enc-seed", "S"});
	this->opt_args.erase({"itl-seed", "S"});

	L::build_args();
}

template <class L, typename B, typename R, typename Q>
void Turbo<L,B,R,Q>
::store_args()
{
	factory::Codec_turbo::store_args(this->ar.get_args(), this->params.cdc);

	if (this->params.cdc.dec.sub1.simd_strategy == "INTER")
		this->params.src.n_frames = mipp::N<Q>();
	if (this->params.cdc.dec.sub1.simd_strategy == "INTRA")
		this->params.src.n_frames = (int)std::ceil(mipp::N<Q>() / 8.f);

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

	this->params.cdc.enc.sub1.n_frames = this->params.cdc.enc.n_frames;
	this->params.cdc.enc.sub2.n_frames = this->params.cdc.enc.n_frames;
	this->params.cdc.dec.sub1.n_frames = this->params.cdc.dec.n_frames;
	this->params.cdc.dec.sub2.n_frames = this->params.cdc.dec.n_frames;

	if (this->params.err_track_revert)
	{
		this->params.cdc.enc.itl.core.type = "USER";
		this->params.cdc.dec.itl.core.type = "USER";
		this->params.cdc.enc.itl.core.path = this->params.err_track_path + std::string("_$snr.itl");
		this->params.cdc.dec.itl.core.path = this->params.err_track_path + std::string("_$snr.itl");
	}
}

template <class L, typename B, typename R, typename Q>
void Turbo<L,B,R,Q>
::print_header()
{
	factory::params_list trash;
	auto &pl_enc = (this->params.cdc.enc.type != "NO") ? this->pl_enc : trash;
	auto &pl_pct = (this->params.cdc.pct.type != "NO") ? this->pl_pct : trash;

	factory::Codec_turbo::make_header(pl_enc, this->pl_dec, pl_pct, this->pl_itl, this->params.cdc, false);

	L::print_header();
}
}
}
