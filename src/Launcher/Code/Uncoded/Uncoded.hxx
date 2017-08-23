#include <iostream>

#include "Factory/Module/Codec/Uncoded/Codec_uncoded.hpp"

#include "Uncoded.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B, typename R, typename Q>
Uncoded<L,B,R,Q>
::Uncoded(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream)
{
}

template <class L, typename B, typename R, typename Q>
Uncoded<L,B,R,Q>
::~Uncoded()
{
}

template <class L, typename B, typename R, typename Q>
void Uncoded<L,B,R,Q>
::build_args()
{
	factory::Codec_uncoded::build_args(this->req_args, this->opt_args);

	this->req_args.erase({"enc-info-bits", "K"});
	this->opt_args.erase({"enc-fra",       "F"});

	L::build_args();
}

template <class L, typename B, typename R, typename Q>
void Uncoded<L,B,R,Q>
::store_args()
{
	L::store_args();

	this->params.cdc.enc.K = this->params.src.K;

	factory::Codec_uncoded::store_args(this->ar.get_args(), this->params.cdc);

	this->params.cdc.enc.n_frames = this->params.src.n_frames;
	this->params.cdc.dec.n_frames = this->params.src.n_frames;
}

template <class L, typename B, typename R, typename Q>
void Uncoded<L,B,R,Q>
::print_header()
{
	factory::params_list trash;
	auto &pl_enc = (this->params.cdc.enc.type != "NO") ? this->pl_enc : trash;

	factory::Codec_uncoded::make_header(pl_enc, this->pl_dec, this->params.cdc, false);

	L::print_header();
}
}
}
