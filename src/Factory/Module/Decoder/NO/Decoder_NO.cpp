#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/NO/Decoder_NO.hpp"
#include "Factory/Module/Decoder/NO/Decoder_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_NO_name   = "Decoder NO";
const std::string aff3ct::factory::Decoder_NO_prefix = "dec";

Decoder_NO
::Decoder_NO(const std::string &prefix)
: Decoder(Decoder_NO_name, prefix)
{
	this->type   = "NONE";
	this->implem = "HARD_DECISION";
}

Decoder_NO* Decoder_NO
::clone() const
{
	return new Decoder_NO(*this);
}

void Decoder_NO
::get_description(cli::Argument_map_info &args) const
{
	Decoder::get_description(args);

	auto p = this->get_prefix();

	cli::add_options(args.at({p+"-type", "D"}), 0, "NONE");
	cli::add_options(args.at({p+"-implem"   }), 0, "HARD_DECISION");
}

void Decoder_NO
::store(const cli::Argument_map_value &vals)
{
	Decoder::store(vals);
}

void Decoder_NO
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Decoder::get_headers(headers, full);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_NO
::build_siso(module::Encoder<B> *encoder) const
{
	if (this->type == "NONE" && this->implem == "HARD_DECISION") return new module::Decoder_NO<B,Q>(this->K, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_NO
::build(module::Encoder<B> *encoder) const
{
	try
	{
		return Decoder::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		if (this->type == "NONE" && this->implem == "HARD_DECISION") return new module::Decoder_NO<B,Q>(this->K, this->n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_NO::build<B_8 ,Q_8 >(module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_NO::build<B_16,Q_16>(module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_NO::build<B_32,Q_32>(module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_NO::build<B_64,Q_64>(module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_NO::build<B,Q>(module::Encoder<B>*) const;
#endif

#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_NO::build_siso<B_8 ,Q_8 >(module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_NO::build_siso<B_16,Q_16>(module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_NO::build_siso<B_32,Q_32>(module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_NO::build_siso<B_64,Q_64>(module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_NO::build_siso<B,Q>(module::Encoder<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
