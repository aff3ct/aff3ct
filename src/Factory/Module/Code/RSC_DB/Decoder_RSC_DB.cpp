#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_DVB1.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_DVB2.hpp"

#include "Decoder_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_RSC_DB::name   = "Decoder RSC DB";
const std::string aff3ct::factory::Decoder_RSC_DB::prefix = "dec";

template <typename B, typename Q, tools::proto_max<Q> MAX>
module::Decoder_SISO<B,Q>* Decoder_RSC_DB
::_build(const parameters &dec_par, const std::vector<std::vector<int>> &trellis)
{
	if (dec_par.type == "BCJR")
	{
		if (dec_par.implem == "GENERIC" ) return new module::Decoder_RSC_DB_BCJR     <B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames);
		if (dec_par.implem == "DVB-RCS1") return new module::Decoder_RSC_DB_BCJR_DVB1<B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames);
		if (dec_par.implem == "DVB-RCS2") return new module::Decoder_RSC_DB_BCJR_DVB2<B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SISO<B,Q>* Decoder_RSC_DB
::build(const parameters &dec_par, const std::vector<std::vector<int>> &trellis)
{
	     if (dec_par.max == "MAX" ) return _build<B,Q,tools::max       <Q>>(dec_par, trellis);
	else if (dec_par.max == "MAXS") return _build<B,Q,tools::max_star  <Q>>(dec_par, trellis);
	else if (dec_par.max == "MAXL") return _build<B,Q,tools::max_linear<Q>>(dec_par, trellis);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Decoder_RSC_DB
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args, p);
	req_args.erase({p+"-cw-size", "N"});

	opt_args[{p+"-type", "D"}].push_back("BCJR");

	opt_args[{p+"-implem"}].push_back("GENERIC, DVB-RCS1, DVB-RCS2");

	opt_args[{p+"-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXL, MAXS"};

	opt_args[{p+"-no-buff"}] =
		{"",
		 "does not suppose a buffered encoding."};
}

void Decoder_RSC_DB
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type   = "BCJR";
	params.implem = "GENERIC";

	Decoder::store_args(vals, params, p);

	if(exist(vals, {p+"-max"    })) params.max      = vals.at({p+"-max"});
	if(exist(vals, {p+"-no-buff"})) params.buffered = false;

	params.N_cw = 2 * params.K;
	params.R    = (float)params.K / (float)params.N_cw;
}

void Decoder_RSC_DB
::make_header(params_list& head_dec, const parameters& params, const bool full)
{
	Decoder::make_header(head_dec, params, full);

	if (params.tail_length && full)
		head_dec.push_back(std::make_pair("Tail length", std::to_string(params.tail_length)));

	if (full) head_dec.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));

	head_dec.push_back(std::make_pair(std::string("Max type"), params.max));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SISO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RSC_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_RSC_DB::parameters&, const std::vector<std::vector<int>>&);
template aff3ct::module::Decoder_SISO<B_16,Q_16>* aff3ct::factory::Decoder_RSC_DB::build<B_16,Q_16>(const aff3ct::factory::Decoder_RSC_DB::parameters&, const std::vector<std::vector<int>>&);
template aff3ct::module::Decoder_SISO<B_32,Q_32>* aff3ct::factory::Decoder_RSC_DB::build<B_32,Q_32>(const aff3ct::factory::Decoder_RSC_DB::parameters&, const std::vector<std::vector<int>>&);
template aff3ct::module::Decoder_SISO<B_64,Q_64>* aff3ct::factory::Decoder_RSC_DB::build<B_64,Q_64>(const aff3ct::factory::Decoder_RSC_DB::parameters&, const std::vector<std::vector<int>>&);
#else
template aff3ct::module::Decoder_SISO<B,Q>* aff3ct::factory::Decoder_RSC_DB::build<B,Q>(const aff3ct::factory::Decoder_RSC_DB::parameters&, const std::vector<std::vector<int>>&);
#endif
// ==================================================================================== explicit template instantiation
