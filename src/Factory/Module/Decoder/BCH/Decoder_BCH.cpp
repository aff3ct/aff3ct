#include <sstream>

#include "Module/Decoder/BCH/Standard/Decoder_BCH_std.hpp"
#include "Module/Decoder/BCH/Genius/Decoder_BCH_genius.hpp"
#include "Module/Decoder/BCH/Fast/Decoder_BCH_fast.hpp"

#include "Tools/Exception/exception.hpp"

#include "Decoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_BCH_name   = "Decoder BCH";
const std::string aff3ct::factory::Decoder_BCH_prefix = "dec";

Decoder_BCH::parameters
::parameters(const std::string &prefix)
: Decoder::parameters(Decoder_BCH_name, prefix)
{
	this->type   = "ALGEBRAIC";
	this->implem = "STD";
}

Decoder_BCH::parameters* Decoder_BCH::parameters
::clone() const
{
	return new Decoder_BCH::parameters(*this);
}

void Decoder_BCH::parameters
::get_description(tools::Argument_map_info &args) const
{
	Decoder::parameters::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder_BCH::parameters::";

	// args.add(
	// 	{p+"-corr-pow", "T"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"correction power of the BCH code.");
	add_arg(args, p, class_name+"p+corr-pow,T",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	args.add_link({p+"-corr-pow", "T"}, {p+"-info-bits", "K"});

	tools::add_options(args.at({p+"-type", "D"}), 0, "ALGEBRAIC");
	tools::add_options(args.at({p+"-implem"   }), 0, "GENIUS", "FAST");
}

void Decoder_BCH::parameters
::store(const tools::Argument_map_value &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	this->m = (int)std::ceil(std::log2(this->N_cw));
	if (this->m == 0)
	{
		std::stringstream message;
		message << "The Galois Field order is null (because N_cw = " << this->N_cw << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (vals.exist({p+"-corr-pow", "T"}))
	{
		this->t = vals.to_int({p + "-corr-pow", "T"});
		if (K == 0)
		{
			this->K = this->N_cw - tools::BCH_polynomial_generator<>(this->N_cw, this->t).get_n_rdncy();
			this->R = (float) this->K / (float) this->N_cw;
		}
	}
	else
		this->t = (this->N_cw - this->K) / this->m;
}

void Decoder_BCH::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	if (this->type != "ML" && this->type != "CHASE")
	{
		auto p = this->get_prefix();

		headers[p].push_back(std::make_pair("Galois field order (m)", std::to_string(this->m)));
		headers[p].push_back(std::make_pair("Correction power (T)",   std::to_string(this->t)));
	}
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_BCH::parameters
::build(const tools::BCH_polynomial_generator<B> &GF, const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	try
	{
		return Decoder::parameters::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		return build_hiho<B,Q>(GF, encoder);
	}
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_BCH
::build(const parameters &params, const tools::BCH_polynomial_generator<B> &GF, const std::unique_ptr<module::Encoder<B>>& encoder)
{
	return params.template build<B,Q>(GF, encoder);
}

template <typename B, typename Q>
module::Decoder_SIHO_HIHO<B,Q>* Decoder_BCH::parameters
::build_hiho(const tools::BCH_polynomial_generator<B> &GF, const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	if (this->type == "ALGEBRAIC")
	{
		if (this->implem == "STD" ) return new module::Decoder_BCH_std <B,Q>(this->K, this->N_cw, GF, this->n_frames);
		if (this->implem == "FAST") return new module::Decoder_BCH_fast<B,Q>(this->K, this->N_cw, GF, this->n_frames);

		if (encoder)
		{
			if (this->implem == "GENIUS") return new module::Decoder_BCH_genius<B,Q>(this->K, this->N_cw, this->t, *encoder, this->n_frames);
		}
	}


	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO_HIHO<B,Q>* Decoder_BCH
::build_hiho(const parameters &params, const tools::BCH_polynomial_generator<B> &GF, const std::unique_ptr<module::Encoder<B>>& encoder)
{
	return params.template build_hiho<B,Q>(GF, encoder);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_BCH::parameters::build<B_8 ,Q_8 >(const aff3ct::tools::BCH_polynomial_generator<B_8 >&, const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_BCH::parameters::build<B_16,Q_16>(const aff3ct::tools::BCH_polynomial_generator<B_16>&, const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_BCH::parameters::build<B_32,Q_32>(const aff3ct::tools::BCH_polynomial_generator<B_32>&, const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_BCH::parameters::build<B_64,Q_64>(const aff3ct::tools::BCH_polynomial_generator<B_64>&, const std::unique_ptr<module::Encoder<B_64>>&) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_BCH::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_8 >&, const std::unique_ptr<module::Encoder<B_8 >>&);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_BCH::build<B_16,Q_16>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_16>&, const std::unique_ptr<module::Encoder<B_16>>&);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_BCH::build<B_32,Q_32>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_32>&, const std::unique_ptr<module::Encoder<B_32>>&);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_BCH::build<B_64,Q_64>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_64>&, const std::unique_ptr<module::Encoder<B_64>>&);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_BCH::parameters::build<B,Q>(const aff3ct::tools::BCH_polynomial_generator<B>&, const std::unique_ptr<module::Encoder<B>>& ) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_BCH::build<B,Q>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B>&, const std::unique_ptr<module::Encoder<B>>& );
#endif
// ==================================================================================== explicit template instantiation


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO_HIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_BCH::parameters::build_hiho<B_8 ,Q_8 >(const aff3ct::tools::BCH_polynomial_generator<B_8 >&, const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_16,Q_16>* aff3ct::factory::Decoder_BCH::parameters::build_hiho<B_16,Q_16>(const aff3ct::tools::BCH_polynomial_generator<B_16>&, const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_32,Q_32>* aff3ct::factory::Decoder_BCH::parameters::build_hiho<B_32,Q_32>(const aff3ct::tools::BCH_polynomial_generator<B_32>&, const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_64,Q_64>* aff3ct::factory::Decoder_BCH::parameters::build_hiho<B_64,Q_64>(const aff3ct::tools::BCH_polynomial_generator<B_64>&, const std::unique_ptr<module::Encoder<B_64>>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_BCH::build_hiho<B_8 ,Q_8 >(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_8 >&, const std::unique_ptr<module::Encoder<B_8 >>&);
template aff3ct::module::Decoder_SIHO_HIHO<B_16,Q_16>* aff3ct::factory::Decoder_BCH::build_hiho<B_16,Q_16>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_16>&, const std::unique_ptr<module::Encoder<B_16>>&);
template aff3ct::module::Decoder_SIHO_HIHO<B_32,Q_32>* aff3ct::factory::Decoder_BCH::build_hiho<B_32,Q_32>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_32>&, const std::unique_ptr<module::Encoder<B_32>>&);
template aff3ct::module::Decoder_SIHO_HIHO<B_64,Q_64>* aff3ct::factory::Decoder_BCH::build_hiho<B_64,Q_64>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_64>&, const std::unique_ptr<module::Encoder<B_64>>&);
#else
template aff3ct::module::Decoder_SIHO_HIHO<B,Q>* aff3ct::factory::Decoder_BCH::parameters::build_hiho<B>(const aff3ct::tools::BCH_polynomial_generator<B>&, const std::unique_ptr<module::Encoder<B>>& ) const;
template aff3ct::module::Decoder_SIHO_HIHO<B,Q>* aff3ct::factory::Decoder_BCH::build_hiho<B>(const aff3ct::factory::Decoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B>&, const std::unique_ptr<module::Encoder<B>>& );
#endif
// ==================================================================================== explicit template instantiation

