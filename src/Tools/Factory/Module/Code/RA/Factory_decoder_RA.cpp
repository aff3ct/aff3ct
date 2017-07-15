#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/RA/Decoder_RA.hpp"

#include "Factory_decoder_RA.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_RA
::build(const parameters &params, const Interleaver<int> &itl)
{
	if (params.type == "RA")
		if (params.implem == "STD" ) return new Decoder_RA<B,R>(params.K, params.N_cw, itl, params.n_ite, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_decoder_RA
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder::build_args(req_args, opt_args);
	Factory_interleaver::build_args(req_args, opt_args);

	opt_args[{"dec-type", "D"}].push_back("RA");
	opt_args[{"dec-implem"}].push_back("STD");

	opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the decoder."};
}

void Factory_decoder_RA
::store_args(const Arguments_reader& ar, parameters &params)
{
	params.type   = "RA";
	params.implem = "STD";

	Factory_decoder::store_args(ar, params);

	params.itl.size = params.N_cw;
	params.itl.n_frames = params.n_frames;
	Factory_interleaver::store_args(ar, params.itl);

	if(ar.exist_arg({"dec-ite", "i"})) params.n_ite = ar.get_arg_int({"dec-ite", "i"});
}

void Factory_decoder_RA
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder::group_args(ar);
	Factory_interleaver::group_args(ar);
}

void Factory_decoder_RA
::header(params_list& head_dec, params_list& head_itl, const parameters& params)
{
	Factory_decoder::header(head_dec, params);
	Factory_interleaver::header(head_itl, params.itl);

	head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder<B_8 ,Q_8 >* aff3ct::tools::Factory_decoder_RA::build<B_8 ,Q_8 >(const aff3ct::tools::Factory_decoder_RA::parameters&, const Interleaver<int>&);
template aff3ct::module::Decoder<B_16,Q_16>* aff3ct::tools::Factory_decoder_RA::build<B_16,Q_16>(const aff3ct::tools::Factory_decoder_RA::parameters&, const Interleaver<int>&);
template aff3ct::module::Decoder<B_32,Q_32>* aff3ct::tools::Factory_decoder_RA::build<B_32,Q_32>(const aff3ct::tools::Factory_decoder_RA::parameters&, const Interleaver<int>&);
template aff3ct::module::Decoder<B_64,Q_64>* aff3ct::tools::Factory_decoder_RA::build<B_64,Q_64>(const aff3ct::tools::Factory_decoder_RA::parameters&, const Interleaver<int>&);
#else
template aff3ct::module::Decoder<B,Q>* aff3ct::tools::Factory_decoder_RA::build<B,Q>(const aff3ct::tools::Factory_decoder_RA::parameters&, const Interleaver<int>&);
#endif
// ==================================================================================== explicit template instantiation
