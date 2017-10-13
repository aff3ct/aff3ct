#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"

#include "Decoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_BCH::name   = "Decoder BCH";
const std::string aff3ct::factory::Decoder_BCH::prefix = "dec";

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_BCH
::build(const parameters &params, const tools::BCH_Polynomial_Generator &GF_poly)
{
	if (params.type == "ALGEBRAIC")
	{
		if (params.implem == "STD") return new module::Decoder_BCH<B,Q>(params.K, params.N_cw, GF_poly, params.n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Decoder_BCH
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args, p);

	opt_args[{p+"-corr-pow", "T"}] =
		{"positive_int",
		 "correction power of the BCH code."};
}

void Decoder_BCH
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type   = "ALGEBRAIC";
	params.implem = "STD";

	Decoder::store_args(vals, params, p);

	params.m = (int)std::ceil(std::log2(params.N_cw));
	if (params.m == 0)
	{
		std::stringstream message;
		message << "The Gallois Field order is null (because N_cw = " << params.N_cw << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (exist(vals, {p+"-corr-pow", "T"}))
		params.t = std::stoi(vals.at({p+"-corr-pow", "T"}));
	else
		params.t = (params.N_cw - params.K) / params.m;
}

void Decoder_BCH
::make_header(params_list& head_dec, const parameters& params, const bool full)
{
	Decoder::make_header(head_dec, params, full);

	head_dec.push_back(std::make_pair("Galois field order (m)", std::to_string(params.m)));
	head_dec.push_back(std::make_pair("Correction power (T)",   std::to_string(params.t)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_BCH::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_Polynomial_Generator&);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_BCH::build<B_16,Q_16>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_Polynomial_Generator&);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_BCH::build<B_32,Q_32>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_Polynomial_Generator&);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_BCH::build<B_64,Q_64>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_Polynomial_Generator&);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_BCH::build<B,Q>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_Polynomial_Generator&);
#endif
// ==================================================================================== explicit template instantiation

