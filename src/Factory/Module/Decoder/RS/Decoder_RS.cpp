#include <utility>
#include <sstream>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Decoder/RS/Standard/Decoder_RS_std.hpp"
#include "Module/Decoder/RS/Genius/Decoder_RS_genius.hpp"
#include "Factory/Module/Decoder/RS/Decoder_RS.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_RS_name   = "Decoder RS";
const std::string aff3ct::factory::Decoder_RS_prefix = "dec";

Decoder_RS
::Decoder_RS(const std::string &prefix)
: Decoder(Decoder_RS_name, prefix)
{
	this->type   = "ALGEBRAIC";
	this->implem = "STD";
}

Decoder_RS* Decoder_RS
::clone() const
{
	return new Decoder_RS(*this);
}

void Decoder_RS
::get_description(cli::Argument_map_info &args) const
{
	Decoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder_RS::";

	tools::add_arg(args, p, class_name+"p+corr-pow,T",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	args.add_link({p+"-corr-pow", "T"}, {p+"-info-bits", "K"});

	cli::add_options(args.at({p+"-type", "D"}), 0, "ALGEBRAIC");
	cli::add_options(args.at({p+"-implem"   }), 0, "GENIUS");
}

void Decoder_RS
::store(const cli::Argument_map_value &vals)
{
	Decoder::store(vals);

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
			this->K = this->N_cw - this->t * 2;
			this->R = (float) this->K / (float) this->N_cw;
		}
	}
	else
		this->t = (this->N_cw - this->K) / 2;
}

void Decoder_RS
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::get_headers(headers, full);

	if (this->type != "ML" && this->type != "CHASE")
	{
		auto p = this->get_prefix();

		headers[p].push_back(std::make_pair("Galois field order (m)", std::to_string(this->m)));
		headers[p].push_back(std::make_pair("Correction power (T)",   std::to_string(this->t)));
	}
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_RS
::build(const tools::RS_polynomial_generator &GF, const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	try
	{
		return Decoder::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		return build_hiho<B,Q>(GF, encoder);
	}
}

template <typename B, typename Q>
module::Decoder_SIHO_HIHO<B,Q>* Decoder_RS
::build_hiho(const tools::RS_polynomial_generator &GF, const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	if (this->type == "ALGEBRAIC")
	{
		if (this->implem == "STD") return new module::Decoder_RS_std<B,Q>(this->K, this->N_cw, GF, this->n_frames);

		if (encoder)
		{
			if (this->implem == "GENIUS") return new module::Decoder_RS_genius<B,Q>(this->K, this->N_cw, GF, *encoder, this->n_frames);
		}
	}


	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RS::build<B_8 ,Q_8 >(const aff3ct::tools::RS_polynomial_generator&, const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_RS::build<B_16,Q_16>(const aff3ct::tools::RS_polynomial_generator&, const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_RS::build<B_32,Q_32>(const aff3ct::tools::RS_polynomial_generator&, const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_RS::build<B_64,Q_64>(const aff3ct::tools::RS_polynomial_generator&, const std::unique_ptr<module::Encoder<B_64>>&) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_RS::build<B,Q>(const aff3ct::tools::RS_polynomial_generator&, const std::unique_ptr<module::Encoder<B>>& ) const;
#endif
// ==================================================================================== explicit template instantiation


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO_HIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RS::build_hiho<B_8 ,Q_8 >(const aff3ct::tools::RS_polynomial_generator&, const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_16,Q_16>* aff3ct::factory::Decoder_RS::build_hiho<B_16,Q_16>(const aff3ct::tools::RS_polynomial_generator&, const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_32,Q_32>* aff3ct::factory::Decoder_RS::build_hiho<B_32,Q_32>(const aff3ct::tools::RS_polynomial_generator&, const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_64,Q_64>* aff3ct::factory::Decoder_RS::build_hiho<B_64,Q_64>(const aff3ct::tools::RS_polynomial_generator&, const std::unique_ptr<module::Encoder<B_64>>&) const;
#else
template aff3ct::module::Decoder_SIHO_HIHO<B,Q>* aff3ct::factory::Decoder_RS::build_hiho<B>(const aff3ct::tools::RS_polynomial_generator&, const std::unique_ptr<module::Encoder<B>>& ) const;
#endif
// ==================================================================================== explicit template instantiation

