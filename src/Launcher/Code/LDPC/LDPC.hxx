#include <iostream>
#include <mipp.h>

#include "Factory/Module/Codec/LDPC/Codec_LDPC.hpp"

#include "LDPC.hpp"

namespace aff3ct
{
namespace launcher
{
template <class L, typename B, typename R, typename Q>
LDPC<L,B,R,Q>
::LDPC(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream)
{
}

template <class L, typename B, typename R, typename Q>
LDPC<L,B,R,Q>
::~LDPC()
{
}

template <class L, typename B, typename R, typename Q>
void LDPC<L,B,R,Q>
::build_args()
{
	factory::Codec_LDPC::build_args(this->req_args, this->opt_args);

	this->opt_args.erase({"enc-fra",  "F"});
	this->opt_args.erase({"enc-seed", "S"});

	L::build_args();
}

template <class L, typename B, typename R, typename Q>
void LDPC<L,B,R,Q>
::store_args()
{
	factory::Codec_LDPC::store_args(this->ar.get_args(), this->params.cdc);

	if (this->params.cdc.dec.simd_strategy == "INTER")
		this->params.src.n_frames = mipp::N<Q>();

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
void LDPC<L,B,R,Q>
::print_header()
{
	factory::params_list trash;
	auto &pl_enc = (this->params.cdc.enc.type != "NO") ? this->pl_enc : trash;

	factory::Codec_LDPC::make_header(pl_enc, this->pl_dec, this->params.cdc, false);

	L::print_header();
}
}
}
