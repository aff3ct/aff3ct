#include <iostream>

#include "Factory/Module/Codec/Repetition/Codec_repetition.hpp"

#include "Repetition.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B, typename R, typename Q>
Repetition<L,B,R,Q>
::Repetition(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream)
{
}

template <class L, typename B, typename R, typename Q>
Repetition<L,B,R,Q>
::~Repetition()
{
}

template <class L, typename B, typename R, typename Q>
void Repetition<L,B,R,Q>
::build_args()
{
	factory::Codec_repetition::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",       "F"});
	this->opt_args.erase({"enc-seed",      "S"});

	L::build_args();
}

template <class L, typename B, typename R, typename Q>
void Repetition<L,B,R,Q>
::store_args()
{
	factory::Codec_repetition::store_args(this->ar.get_args(), this->params.cdc);

	if (std::is_same<Q,int8_t>() || std::is_same<Q,int16_t>())
	{
		this->params.qnt.n_bits     = 6;
		this->params.qnt.n_decimals = 2;
	}

	L::store_args();

	this->params.cdc.enc.n_frames = this->params.src.n_frames;
	this->params.cdc.dec.n_frames = this->params.src.n_frames;
}

template <class L, typename B, typename R, typename Q>
void Repetition<L,B,R,Q>
::print_header()
{
	factory::params_list trash;
	auto &pl_enc = (this->params.cdc.enc.type != "NO") ? this->pl_enc : trash;

	factory::Codec_repetition::make_header(pl_enc, this->pl_dec, this->params.cdc, false);

	L::print_header();
}
}
}
