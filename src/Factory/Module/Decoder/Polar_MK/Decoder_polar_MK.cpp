#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Module/Decoder/Polar_MK/SC/Decoder_polar_MK_SC_naive.hpp"
#include "Module/Decoder/Polar_MK/SC/Decoder_polar_MK_SC_naive_sys.hpp"
#include "Module/Decoder/Polar_MK/SCL/Decoder_polar_MK_SCL_naive.hpp"
#include "Module/Decoder/Polar_MK/SCL/Decoder_polar_MK_SCL_naive_sys.hpp"
#include "Module/Decoder/Polar_MK/SCL/CRC/Decoder_polar_MK_SCL_naive_CA.hpp"
#include "Module/Decoder/Polar_MK/SCL/CRC/Decoder_polar_MK_SCL_naive_CA_sys.hpp"
#include "Module/Decoder/Polar_MK/ASCL/Decoder_polar_MK_ASCL_naive_CA.hpp"
#include "Module/Decoder/Polar_MK/ASCL/Decoder_polar_MK_ASCL_naive_CA_sys.hpp"
#include "Factory/Module/Decoder/Polar_MK/Decoder_polar_MK.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_polar_MK_name   = "Decoder Polar MK";
const std::string aff3ct::factory::Decoder_polar_MK_prefix = "dec";

Decoder_polar_MK
::Decoder_polar_MK(const std::string &prefix)
: Decoder(Decoder_polar_MK_name, prefix)
{
	this->type   = "SC";
	this->implem = "NAIVE";
}

Decoder_polar_MK
::Decoder_polar_MK(const std::string &name, const std::string &prefix)
: Decoder(name, prefix)
{
	this->type   = "SC";
	this->implem = "NAIVE";
}

Decoder_polar_MK* Decoder_polar_MK
::clone() const
{
	return new Decoder_polar_MK(*this);
}

void Decoder_polar_MK
::get_description(cli::Argument_map_info &args) const
{
	Decoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder_polar_MK::";

	cli::add_options(args.at({p+"-type", "D"}), 0, "SC", "SCL", "ASCL");

	args.at({p+"-implem"})->change_type(cli::Text(cli::Example_set("NAIVE")));

	tools::add_arg(args, p, class_name+"p+lists,L",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+node-type",
		cli::Text(cli::Including_set("MS", "SPA")));
}

void Decoder_polar_MK
::store(const cli::Argument_map_value &vals)
{
	Decoder::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-lists", "L"})) this->L         = vals.to_int({p+"-lists", "L"});
	if(vals.exist({p+"-node-type" })) this->node_type = vals.at    ({p+"-node-type" });
}

void Decoder_polar_MK
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Decoder::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Node type", this->node_type));
	if (this->type == "SCL")
		headers[p].push_back(std::make_pair("Num. of lists (L)", std::to_string(this->L)));
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_polar_MK
::build(const tools::Polar_code &code, const std::vector<bool> &frozen_bits, const module::CRC<B> *crc,
        module::Encoder<B>* encoder) const
{
	try
	{
		return Decoder::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		std::vector<std::vector<std::function<Q(const std::vector<Q> &LLRs,
		                                        const std::vector<B> &bits)>>> lambdas(code.get_kernel_matrices().size());

		for (size_t l = 0; l < lambdas.size(); l++)
		{
			if (tools::Polar_lambdas<B,Q>::functions.find(code.get_kernel_matrices()[l]) ==
			    tools::Polar_lambdas<B,Q>::functions.end())
				throw tools::runtime_error(__FILE__, __LINE__, __func__, "Unsupported polar kernel.");

			if (this->node_type == "MS")
				lambdas[l] = tools::Polar_lambdas<B,Q,tools::square_plus_MS<Q>>::functions[code.get_kernel_matrices()[l]];
			else if (this->node_type == "SPA")
				lambdas[l] = tools::Polar_lambdas<B,Q,tools::square_plus_SPA<Q>>::functions[code.get_kernel_matrices()[l]];
		}

		if (!this->systematic) // non-systematic encoding
		{
			if (this->implem == "NAIVE")
			{
				if (crc != nullptr && std::unique_ptr<module::CRC<B>>(crc->clone())->get_size() > 0)
				{
					if (this->type == "SCL" ) return new module::Decoder_polar_MK_SCL_naive_CA <B,Q>(this->K, this->N_cw, this->L, code, frozen_bits, lambdas, *crc);
					if (this->type == "ASCL") return new module::Decoder_polar_MK_ASCL_naive_CA<B,Q>(this->K, this->N_cw, this->L, code, frozen_bits, lambdas, *crc);
				}
				if (this->type == "SC" ) return new module::Decoder_polar_MK_SC_naive <B,Q>(this->K, this->N_cw,          code, frozen_bits, lambdas);
				if (this->type == "SCL") return new module::Decoder_polar_MK_SCL_naive<B,Q>(this->K, this->N_cw, this->L, code, frozen_bits, lambdas);
			}
		}
		else // systematic encoding
		{
			if (this->implem == "NAIVE")
			{
				if (crc != nullptr && std::unique_ptr<module::CRC<B>>(crc->clone())->get_size() > 0)
				{
					if (this->type == "SCL" ) return new module::Decoder_polar_MK_SCL_naive_CA_sys <B,Q>(this->K, this->N_cw, this->L, code, frozen_bits, lambdas, *crc);
					if (this->type == "ASCL") return new module::Decoder_polar_MK_ASCL_naive_CA_sys<B,Q>(this->K, this->N_cw, this->L, code, frozen_bits, lambdas, *crc);
				}
				if (this->type == "SC" ) return new module::Decoder_polar_MK_SC_naive_sys <B,Q>(this->K, this->N_cw,          code, frozen_bits, lambdas);
				if (this->type == "SCL") return new module::Decoder_polar_MK_SCL_naive_sys<B,Q>(this->K, this->N_cw, this->L, code, frozen_bits, lambdas);
			}
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_polar_MK::build<B_8 ,Q_8 >(const tools::Polar_code&, const std::vector<bool>&, const module::CRC<B_8 >*, module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_polar_MK::build<B_16,Q_16>(const tools::Polar_code&, const std::vector<bool>&, const module::CRC<B_16>*, module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_polar_MK::build<B_32,Q_32>(const tools::Polar_code&, const std::vector<bool>&, const module::CRC<B_32>*, module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_polar_MK::build<B_64,Q_64>(const tools::Polar_code&, const std::vector<bool>&, const module::CRC<B_64>*, module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_polar_MK::build<B,Q>(const tools::Polar_code&, const std::vector<bool>&, const module::CRC<B>*, module::Encoder<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
