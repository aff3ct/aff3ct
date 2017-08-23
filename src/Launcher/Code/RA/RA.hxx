#include <iostream>

#include "Factory/Module/Codec/RA/Codec_RA.hpp"

#include "RA.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B, typename R, typename Q>
RA<L,B,R,Q>
::RA(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream)
{
}

template <class L, typename B, typename R, typename Q>
RA<L,B,R,Q>
::~RA()
{
}

template <class L, typename B, typename R, typename Q>
void RA<L,B,R,Q>
::build_args()
{
	factory::Codec_RA::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",   "F"});
	this->opt_args.erase({"enc-seed",  "S"});
	this->opt_args.erase({"dec-implem"    });
	this->opt_args.erase({"dec-type",  "D"});
	this->opt_args.erase({"itl-seed",  "S"});

	L::build_args();
}

template <class L, typename B, typename R, typename Q>
void RA<L,B,R,Q>
::store_args()
{
	factory::Codec_RA::store_args(this->ar.get_args(), this->params.cdc);

	L::store_args();

	this->params.cdc.enc.n_frames = this->params.src.n_frames;
	this->params.cdc.dec.n_frames = this->params.src.n_frames;

	this->params.cdc.enc.itl.core.seed     = this->params.global_seed;
	this->params.cdc.dec.itl.core.seed     = this->params.global_seed;
	this->params.cdc.enc.itl.core.n_frames = this->params.src.n_frames;
	this->params.cdc.dec.itl.core.n_frames = this->params.src.n_frames;

	if (this->params.err_track_revert)
	{
		this->params.cdc.enc.itl.core.type = "USER";
		this->params.cdc.dec.itl.core.type = "USER";
		this->params.cdc.enc.itl.core.path = this->params.err_track_path + std::string("_$snr.itl");
		this->params.cdc.dec.itl.core.path = this->params.err_track_path + std::string("_$snr.itl");
	}
}

template <class L, typename B, typename R, typename Q>
void RA<L,B,R,Q>
::print_header()
{
	factory::params_list trash;
	auto &pl_enc = (this->params.cdc.enc.type != "NO") ? this->pl_enc : trash;

	factory::Codec_RA::make_header(pl_enc, this->pl_dec, this->pl_itl, this->params.cdc, false);

	L::print_header();
}
}
}
