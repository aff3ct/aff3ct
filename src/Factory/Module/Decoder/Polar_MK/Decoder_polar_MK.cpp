#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Polar_MK/SC/Decoder_polar_MK_SC_naive.hpp"

#include "Decoder_polar_MK.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_polar_MK_name   = "Decoder Polar MK";
const std::string aff3ct::factory::Decoder_polar_MK_prefix = "dec";

Decoder_polar_MK::parameters
::parameters(const std::string &prefix)
: Decoder::parameters(Decoder_polar_MK_name, prefix)
{
	this->type   = "SC";
	this->implem = "NAIVE";
}

Decoder_polar_MK::parameters
::parameters(const std::string &name, const std::string &prefix)
: Decoder::parameters(name, prefix)
{
	this->type   = "SC";
	this->implem = "NAIVE";
}

Decoder_polar_MK::parameters* Decoder_polar_MK::parameters
::clone() const
{
	return new Decoder_polar_MK::parameters(*this);
}

void Decoder_polar_MK::parameters
::get_description(tools::Argument_map_info &args) const
{
	Decoder::parameters::get_description(args);

	auto p = this->get_prefix();

	tools::add_options(args.at({p+"-type", "D"}), 0, "SC");

	args.at({p+"-implem"})->change_type(tools::Text(tools::Example_set("NAIVE")));
}

void Decoder_polar_MK::parameters
::store(const tools::Argument_map_value &vals)
{
	Decoder::parameters::store(vals);
}

void Decoder_polar_MK::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_polar_MK::parameters
::build(const tools::Polar_code &code, const std::vector<bool> &frozen_bits, module::CRC<B> *crc,
        const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	try
	{
		return Decoder::parameters::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		if (!this->systematic) // non-systematic encoding
		{
			if (this->implem == "NAIVE")
			{
				if (crc == nullptr || crc->get_size() == 0)
				{
					if (this->type == "SC") return new module::Decoder_polar_MK_SC_naive<B,Q>(this->K, this->N_cw, code, frozen_bits, this->n_frames);
				}
			}
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_polar_MK
::build(const parameters& params, const tools::Polar_code &code, const std::vector<bool> &frozen_bits,
        module::CRC<B> *crc, const std::unique_ptr<module::Encoder<B>>& encoder)
{
	return params.template build<B,Q>(code, frozen_bits, crc, encoder);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_polar_MK::parameters::build<B_8 ,Q_8 >(const tools::Polar_code&, const std::vector<bool>&, module::CRC<B_8 >*, const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_polar_MK::parameters::build<B_16,Q_16>(const tools::Polar_code&, const std::vector<bool>&, module::CRC<B_16>*, const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_polar_MK::parameters::build<B_32,Q_32>(const tools::Polar_code&, const std::vector<bool>&, module::CRC<B_32>*, const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_polar_MK::parameters::build<B_64,Q_64>(const tools::Polar_code&, const std::vector<bool>&, module::CRC<B_64>*, const std::unique_ptr<module::Encoder<B_64>>&) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_polar_MK::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_polar_MK::parameters&, const tools::Polar_code&, const std::vector<bool>&, module::CRC<B_8 >*, const std::unique_ptr<module::Encoder<B_8 >>&);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_polar_MK::build<B_16,Q_16>(const aff3ct::factory::Decoder_polar_MK::parameters&, const tools::Polar_code&, const std::vector<bool>&, module::CRC<B_16>*, const std::unique_ptr<module::Encoder<B_16>>&);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_polar_MK::build<B_32,Q_32>(const aff3ct::factory::Decoder_polar_MK::parameters&, const tools::Polar_code&, const std::vector<bool>&, module::CRC<B_32>*, const std::unique_ptr<module::Encoder<B_32>>&);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_polar_MK::build<B_64,Q_64>(const aff3ct::factory::Decoder_polar_MK::parameters&, const tools::Polar_code&, const std::vector<bool>&, module::CRC<B_64>*, const std::unique_ptr<module::Encoder<B_64>>&);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_polar_MK::parameters::build<B,Q>(const tools::Polar_code&, const std::vector<bool>&, module::CRC<B>*, const std::unique_ptr<module::Encoder<B>>&) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_polar_MK::build<B,Q>(const aff3ct::factory::Decoder_polar_MK::parameters&, const tools::Polar_code&, const std::vector<bool>&, module::CRC<B>*, const std::unique_ptr<module::Encoder<B>>&);
#endif
// ==================================================================================== explicit template instantiation
