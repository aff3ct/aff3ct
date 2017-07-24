#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/RA/Decoder_RA.hpp"

#include "Decoder_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_RA::name   = "Decoder RA";
const std::string aff3ct::factory::Decoder_RA::prefix = "dec";

template <typename B, typename R>
module::Decoder<B,R>* Decoder_RA
::build(const parameters &params, const module::Interleaver<int> &itl)
{
	if (params.type == "RA")
		if (params.implem == "STD" ) return new module::Decoder_RA<B,R>(params.K, params.N_cw, itl, params.n_ite, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Decoder_RA
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args);
	Interleaver::build_args(req_args, opt_args, "itl");
	req_args.erase({"itl-size"    });
	opt_args.erase({"itl-fra", "F"});

	opt_args[{p+"-type", "D"}].push_back("RA");
	opt_args[{p+"-implem"   }].push_back("STD");

	opt_args[{p+"-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the decoder."};
}

void Decoder_RA
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type   = "RA";
	params.implem = "STD";

	Decoder::store_args(vals, params);

	params.itl.size     = params.N_cw;
	params.itl.n_frames = params.n_frames;
	Interleaver::store_args(vals, params.itl, "itl");

	if(exist(vals, {p+"-ite", "i"})) params.n_ite = std::stoi(vals.at({p+"-ite", "i"}));
}

void Decoder_RA
::header(params_list& head_dec, params_list& head_itl, const parameters& params)
{
	Decoder::header(head_dec, params);
	Interleaver::header(head_itl, params.itl);

	head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder<B_8 ,Q_8 >* aff3ct::factory::Decoder_RA::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<int>&);
template aff3ct::module::Decoder<B_16,Q_16>* aff3ct::factory::Decoder_RA::build<B_16,Q_16>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<int>&);
template aff3ct::module::Decoder<B_32,Q_32>* aff3ct::factory::Decoder_RA::build<B_32,Q_32>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<int>&);
template aff3ct::module::Decoder<B_64,Q_64>* aff3ct::factory::Decoder_RA::build<B_64,Q_64>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<int>&);
#else
template aff3ct::module::Decoder<B,Q>* aff3ct::factory::Decoder_RA::build<B,Q>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<int>&);
#endif
// ==================================================================================== explicit template instantiation
