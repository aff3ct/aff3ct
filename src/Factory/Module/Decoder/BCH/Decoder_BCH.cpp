#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"

#include "Decoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_BCH::name   = "Decoder BCH";
const std::string aff3ct::factory::Decoder_BCH::prefix = "dec";

Decoder_BCH::parameters
::parameters(const std::string prefix)
: Decoder::parameters(Decoder_BCH::name, prefix)
{
	this->type   = "ALGEBRAIC";
	this->implem = "STD";
}

Decoder_BCH::parameters
::~parameters()
{
}

Decoder_BCH::parameters* Decoder_BCH::parameters
::clone() const
{
	return new Decoder_BCH::parameters(*this);
}

void Decoder_BCH::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Decoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	opt_args[{p+"-corr-pow", "T"}] =
		{"strictly_positive_int",
		 "correction power of the BCH code."};
}

void Decoder_BCH::parameters
::store(const arg_val_map &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	this->m = (int)std::ceil(std::log2(this->N_cw));
	if (this->m == 0)
	{
		std::stringstream message;
		message << "The Gallois Field order is null (because N_cw = " << this->N_cw << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (exist(vals, {p+"-corr-pow", "T"}))
		this->t = std::stoi(vals.at({p+"-corr-pow", "T"}));
	else
		this->t = (this->N_cw - this->K) / this->m;
}

void Decoder_BCH::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Galois field order (m)", std::to_string(this->m)));
	headers[p].push_back(std::make_pair("Correction power (T)",   std::to_string(this->t)));
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_BCH::parameters
::build(const tools::BCH_Polynomial_Generator &GF) const
{
	if (this->type == "ALGEBRAIC")
	{
		if (this->implem == "STD") return new module::Decoder_BCH<B,Q>(this->K, this->N_cw, GF, this->n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_BCH
::build(const parameters &params, const tools::BCH_Polynomial_Generator &GF)
{
	return params.template build<B,Q>(GF);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_BCH::parameters::build<B_8 ,Q_8 >(const aff3ct::tools::BCH_Polynomial_Generator&) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_BCH::parameters::build<B_16,Q_16>(const aff3ct::tools::BCH_Polynomial_Generator&) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_BCH::parameters::build<B_32,Q_32>(const aff3ct::tools::BCH_Polynomial_Generator&) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_BCH::parameters::build<B_64,Q_64>(const aff3ct::tools::BCH_Polynomial_Generator&) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_BCH::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_Polynomial_Generator&);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_BCH::build<B_16,Q_16>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_Polynomial_Generator&);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_BCH::build<B_32,Q_32>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_Polynomial_Generator&);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_BCH::build<B_64,Q_64>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_Polynomial_Generator&);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_BCH::parameters::build<B,Q>(const aff3ct::tools::BCH_Polynomial_Generator&) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_BCH::build<B,Q>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_Polynomial_Generator&);
#endif
// ==================================================================================== explicit template instantiation

