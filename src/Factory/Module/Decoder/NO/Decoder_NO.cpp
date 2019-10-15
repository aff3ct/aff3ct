#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/NO/Decoder_NO.hpp"
#include "Factory/Module/Decoder/NO/Decoder_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_NO_name   = "Decoder NO";
const std::string aff3ct::factory::Decoder_NO_prefix = "dec";

Decoder_NO::parameters
::parameters(const std::string &prefix)
: Decoder::parameters(Decoder_NO_name, prefix)
{
	this->type   = "NONE";
	this->implem = "HARD_DECISION";
}

Decoder_NO::parameters* Decoder_NO::parameters
::clone() const
{
	return new Decoder_NO::parameters(*this);
}

void Decoder_NO::parameters
::get_description(cli::Argument_map_info &args) const
{
	Decoder::parameters::get_description(args);

	auto p = this->get_prefix();

	cli::add_options(args.at({p+"-type", "D"}), 0, "NONE");
	cli::add_options(args.at({p+"-implem"   }), 0, "HARD_DECISION");
}

void Decoder_NO::parameters
::store(const cli::Argument_map_value &vals)
{
	Decoder::parameters::store(vals);
}

void Decoder_NO::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_NO::parameters
::build_siso(const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	if (this->type == "NONE" && this->implem == "HARD_DECISION") return new module::Decoder_NO<B,Q>(this->K, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_NO::parameters
::build(const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	try
	{
		return Decoder::parameters::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		if (this->type == "NONE" && this->implem == "HARD_DECISION") return new module::Decoder_NO<B,Q>(this->K, this->n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_NO
::build_siso(const parameters &params, const std::unique_ptr<module::Encoder<B>>& encoder)
{
	return params.template build_siso<B,Q>(encoder);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_NO
::build(const parameters &params, const std::unique_ptr<module::Encoder<B>>& encoder)
{
	return params.template build<B,Q>(encoder);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_NO::parameters::build<B_8 ,Q_8 >(const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_NO::parameters::build<B_16,Q_16>(const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_NO::parameters::build<B_32,Q_32>(const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_NO::parameters::build<B_64,Q_64>(const std::unique_ptr<module::Encoder<B_64>>&) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_NO::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_NO::parameters&, const std::unique_ptr<module::Encoder<B_8 >>&);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_NO::build<B_16,Q_16>(const aff3ct::factory::Decoder_NO::parameters&, const std::unique_ptr<module::Encoder<B_16>>&);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_NO::build<B_32,Q_32>(const aff3ct::factory::Decoder_NO::parameters&, const std::unique_ptr<module::Encoder<B_32>>&);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_NO::build<B_64,Q_64>(const aff3ct::factory::Decoder_NO::parameters&, const std::unique_ptr<module::Encoder<B_64>>&);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_NO::parameters::build<B,Q>(const std::unique_ptr<module::Encoder<B>>& ) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_NO::build<B,Q>(const aff3ct::factory::Decoder_NO::parameters&, const std::unique_ptr<module::Encoder<B>>& );
#endif

#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_NO::parameters::build_siso<B_8 ,Q_8 >(const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_NO::parameters::build_siso<B_16,Q_16>(const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_NO::parameters::build_siso<B_32,Q_32>(const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_NO::parameters::build_siso<B_64,Q_64>(const std::unique_ptr<module::Encoder<B_64>>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_NO::build_siso<B_8 ,Q_8 >(const aff3ct::factory::Decoder_NO::parameters&, const std::unique_ptr<module::Encoder<B_8 >>&);
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_NO::build_siso<B_16,Q_16>(const aff3ct::factory::Decoder_NO::parameters&, const std::unique_ptr<module::Encoder<B_16>>&);
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_NO::build_siso<B_32,Q_32>(const aff3ct::factory::Decoder_NO::parameters&, const std::unique_ptr<module::Encoder<B_32>>&);
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_NO::build_siso<B_64,Q_64>(const aff3ct::factory::Decoder_NO::parameters&, const std::unique_ptr<module::Encoder<B_64>>&);
#else
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_NO::parameters::build_siso<B,Q>(const std::unique_ptr<module::Encoder<B>>& ) const;
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_NO::build_siso<B,Q>(const aff3ct::factory::Decoder_NO::parameters&, const std::unique_ptr<module::Encoder<B>>& );
#endif
// ==================================================================================== explicit template instantiation
