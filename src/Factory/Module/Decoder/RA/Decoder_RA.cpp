#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/RA/Decoder_RA.hpp"

#include "Decoder_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_RA::name   = "Decoder RA";
const std::string aff3ct::factory::Decoder_RA::prefix = "dec";

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_RA::parameters
::build(const module::Interleaver<Q> &itl) const
{
	if (this->type == "RA")
		if (this->implem == "STD" ) return new module::Decoder_RA<B,Q>(this->K, this->N_cw, itl, this->n_ite, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_RA
::build(const parameters &params, const module::Interleaver<Q> &itl)
{
	return params.template build<B,Q>(itl);
}

void Decoder_RA
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args, p);
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

	Decoder::store_args(vals, params, p);

	params.itl.core.size     = params.N_cw;
	params.itl.core.n_frames = params.n_frames;
	Interleaver::store_args(vals, params.itl, "itl");

	if(exist(vals, {p+"-ite", "i"})) params.n_ite = std::stoi(vals.at({p+"-ite", "i"}));
}

void Decoder_RA
::make_header(params_list& head_dec, params_list& head_itl, const parameters& params, const bool full)
{
	Decoder    ::make_header(head_dec, params,     full);
	Interleaver::make_header(head_itl, params.itl, full);

	head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RA::parameters::build<B_8 ,Q_8 >(const aff3ct::module::Interleaver<Q_8 >&) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_RA::parameters::build<B_16,Q_16>(const aff3ct::module::Interleaver<Q_16>&) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_RA::parameters::build<B_32,Q_32>(const aff3ct::module::Interleaver<Q_32>&) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_RA::parameters::build<B_64,Q_64>(const aff3ct::module::Interleaver<Q_64>&) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RA::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<Q_8 >&);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_RA::build<B_16,Q_16>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<Q_16>&);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_RA::build<B_32,Q_32>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<Q_32>&);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_RA::build<B_64,Q_64>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<Q_64>&);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_RA::parameters::build<B,Q>(const aff3ct::module::Interleaver<Q>&) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_RA::build<B,Q>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<Q>&);
#endif
// ==================================================================================== explicit template instantiation
