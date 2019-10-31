#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"
#include "Module/Decoder/Repetition/Decoder_repetition_fast.hpp"
#include "Factory/Module/Decoder/Repetition/Decoder_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_repetition_name   = "Decoder Repetition";
const std::string aff3ct::factory::Decoder_repetition_prefix = "dec";

Decoder_repetition
::Decoder_repetition(const std::string &prefix)
: Decoder(Decoder_repetition_name, prefix)
{
	this->type   = "REPETITION";
	this->implem = "STD";
}

Decoder_repetition* Decoder_repetition
::clone() const
{
	return new Decoder_repetition(*this);
}

void Decoder_repetition
::get_description(cli::Argument_map_info &args) const
{
	Decoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder_repetition::";

	cli::add_options(args.at({p+"-type", "D"}), 0, "REPETITION" );
	cli::add_options(args.at({p+"-implem"   }), 0, "STD", "FAST");

	tools::add_arg(args, p, class_name+"p+no-buff",
		cli::None());
}

void Decoder_repetition
::store(const cli::Argument_map_value &vals)
{
	Decoder::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-no-buff"})) this->buffered = false;
}

void Decoder_repetition
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Decoder::get_headers(headers, full);

	if (this->type != "ML" && this->type != "CHASE")
	{
		auto p = this->get_prefix();

		if (full) headers[p].push_back(std::make_pair("Buffered", (this->buffered ? "on" : "off")));
	}
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_repetition
::build(const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	try
	{
		return Decoder::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		if (this->type == "REPETITION")
		{
			if (this->implem == "STD" ) return new module::Decoder_repetition_std <B,Q>(this->K, this->N_cw, this->buffered, this->n_frames);
			if (this->implem == "FAST") return new module::Decoder_repetition_fast<B,Q>(this->K, this->N_cw, this->buffered, this->n_frames);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_repetition::build<B_8 ,Q_8 >(const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_repetition::build<B_16,Q_16>(const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_repetition::build<B_32,Q_32>(const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_repetition::build<B_64,Q_64>(const std::unique_ptr<module::Encoder<B_64>>&) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_repetition::build<B,Q>(const std::unique_ptr<module::Encoder<B>>& ) const;
#endif
// ==================================================================================== explicit template instantiation
