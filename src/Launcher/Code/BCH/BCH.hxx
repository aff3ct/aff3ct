#include <iostream>

#include "Factory/Module/Codec/BCH/Codec_BCH.hpp"

#include "BCH.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B, typename R, typename Q>
BCH<L,B,R,Q>
::BCH(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream)
{
}

template <class L, typename B, typename R, typename Q>
BCH<L,B,R,Q>
::~BCH()
{
}

template <class L, typename B, typename R, typename Q>
void BCH<L,B,R,Q>
::build_args()
{
	factory::Codec_BCH::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",   "F"});
	this->opt_args.erase({"enc-seed",  "S"});
	this->opt_args.erase({"dec-implem"    });
	this->opt_args.erase({"dec-type",  "D"});

	L::build_args();
}

template <class L, typename B, typename R, typename Q>
void BCH<L,B,R,Q>
::store_args()
{
	factory::Codec_BCH::store_args(this->ar.get_args(), this->params.cdc);

	L::store_args();

	this->params.cdc.enc.n_frames = this->params.src.n_frames;
	this->params.cdc.dec.n_frames = this->params.src.n_frames;
}

template <class L, typename B, typename R, typename Q>
void BCH<L,B,R,Q>
::print_header()
{
	factory::params_list trash;
	auto &pl_enc = (this->params.cdc.enc.type != "NO") ? this->pl_enc : trash;

	factory::Codec_BCH::make_header(pl_enc, this->pl_dec, this->params.cdc, false);

	L::print_header();
}
}
}
