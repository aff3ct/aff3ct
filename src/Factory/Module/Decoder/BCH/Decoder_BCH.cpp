#include <utility>
#include <sstream>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Decoder/BCH/Standard/Decoder_BCH_std.hpp"
#include "Module/Decoder/BCH/Genius/Decoder_BCH_genius.hpp"
#include "Module/Decoder/BCH/Fast/Decoder_BCH_fast.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_BCH_name   = "Decoder BCH";
const std::string aff3ct::factory::Decoder_BCH_prefix = "dec";

Decoder_BCH
::Decoder_BCH(const std::string &prefix)
: Decoder(Decoder_BCH_name, prefix)
{
	this->type   = "ALGEBRAIC";
	this->implem = "STD";
}

Decoder_BCH* Decoder_BCH
::clone() const
{
	return new Decoder_BCH(*this);
}

void Decoder_BCH
::get_description(cli::Argument_map_info &args) const
{
	Decoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder_BCH::";

	tools::add_arg(args, p, class_name+"p+corr-pow,T",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	args.add_link({p+"-corr-pow", "T"}, {p+"-info-bits", "K"});

	cli::add_options(args.at({p+"-type", "D"}), 0, "ALGEBRAIC");
	cli::add_options(args.at({p+"-implem"   }), 0, "GENIUS", "FAST");
}

void Decoder_BCH
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
			this->K = this->N_cw - tools::BCH_polynomial_generator<>(this->N_cw, this->t).get_n_rdncy();
			this->R = (float) this->K / (float) this->N_cw;
		}
	}
	else
		this->t = (this->N_cw - this->K) / this->m;
}

void Decoder_BCH
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
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
module::Decoder_SIHO<B,Q>* Decoder_BCH
::build(const tools::BCH_polynomial_generator<B> &GF, module::Encoder<B> *encoder) const
{
	try
	{
		return Decoder::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		if (this->type == "ALGEBRAIC")
		{
			if (this->implem == "STD" ) return new module::Decoder_BCH_std <B,Q>(this->K, this->N_cw, GF);
			if (this->implem == "FAST") return new module::Decoder_BCH_fast<B,Q>(this->K, this->N_cw, GF);

			if (encoder)
			{
				if (this->implem == "GENIUS") return new module::Decoder_BCH_genius<B,Q>(this->K, this->N_cw, this->t, *encoder);
			}
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_BCH::build<B_8 ,Q_8 >(const aff3ct::tools::BCH_polynomial_generator<B_8 >&, module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_BCH::build<B_16,Q_16>(const aff3ct::tools::BCH_polynomial_generator<B_16>&, module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_BCH::build<B_32,Q_32>(const aff3ct::tools::BCH_polynomial_generator<B_32>&, module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_BCH::build<B_64,Q_64>(const aff3ct::tools::BCH_polynomial_generator<B_64>&, module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_BCH::build<B,Q>(const aff3ct::tools::BCH_polynomial_generator<B>&, module::Encoder<B>*) const;
#endif
// ==================================================================================== explicit template instantiation