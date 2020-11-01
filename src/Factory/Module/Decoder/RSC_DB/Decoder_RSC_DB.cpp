#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_generic.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_DVB_RCS1.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_DVB_RCS2.hpp"
#include "Factory/Module/Decoder/RSC_DB/Decoder_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_RSC_DB_name   = "Decoder RSC DB";
const std::string aff3ct::factory::Decoder_RSC_DB_prefix = "dec";

Decoder_RSC_DB
::Decoder_RSC_DB(const std::string &prefix)
: Decoder(Decoder_RSC_DB_name, prefix)
{
	this->type   = "BCJR";
	this->implem = "GENERIC";
}

Decoder_RSC_DB* Decoder_RSC_DB
::clone() const
{
	return new Decoder_RSC_DB(*this);
}

void Decoder_RSC_DB
::get_description(cli::Argument_map_info &args) const
{
	Decoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder_RSC_DB::";

	args.erase({p+"-cw-size", "N"});

	cli::add_options(args.at({p+"-type", "D"}), 0, "BCJR");
	cli::add_options(args.at({p+"-implem"   }), 0, "GENERIC", "DVB-RCS1", "DVB-RCS2");

	tools::add_arg(args, p, class_name+"p+max",
		cli::Text(cli::Including_set("MAX", "MAXL", "MAXS")));

	tools::add_arg(args, p, class_name+"p+no-buff",
		cli::None());
}

void Decoder_RSC_DB
::store(const cli::Argument_map_value &vals)
{
	Decoder::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-max"    })) this->max      = vals.at({p+"-max"});
	if(vals.exist({p+"-no-buff"})) this->buffered = false;

	this->N_cw = 2 * this->K;
	this->R    = (float)this->K / (float)this->N_cw;
}

void Decoder_RSC_DB
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Decoder::get_headers(headers, full);

	if (this->type != "ML" && this->type != "CHASE")
	{
		auto p = this->get_prefix();

		if (this->tail_length && full)
			headers[p].push_back(std::make_pair("Tail length", std::to_string(this->tail_length)));

		if (full) headers[p].push_back(std::make_pair("Buffered", (this->buffered ? "on" : "off")));

		headers[p].push_back(std::make_pair(std::string("Max type"), this->max));
	}
}

template <typename B, typename Q, tools::proto_max<Q> MAX>
module::Decoder_RSC_DB_BCJR<B,Q>* Decoder_RSC_DB
::_build_siso(const std::vector<std::vector<int>> &trellis, module::Encoder<B> *encoder) const
{
	if (this->type == "BCJR")
	{
		if (this->implem == "GENERIC" ) return new module::Decoder_RSC_DB_BCJR_generic <B,Q,MAX>(this->K, trellis, this->buffered);
		if (this->implem == "DVB-RCS1") return new module::Decoder_RSC_DB_BCJR_DVB_RCS1<B,Q,MAX>(this->K, trellis, this->buffered);
		if (this->implem == "DVB-RCS2") return new module::Decoder_RSC_DB_BCJR_DVB_RCS2<B,Q,MAX>(this->K, trellis, this->buffered);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_RSC_DB_BCJR<B,Q>* Decoder_RSC_DB
::build_siso(const std::vector<std::vector<int>> &trellis, module::Encoder<B> *encoder) const
{
	if (this->max == "MAX" ) return _build_siso<B,Q,tools::max       <Q>>(trellis, encoder);
	if (this->max == "MAXS") return _build_siso<B,Q,tools::max_star  <Q>>(trellis, encoder);
	if (this->max == "MAXL") return _build_siso<B,Q,tools::max_linear<Q>>(trellis, encoder);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_RSC_DB
::build(const std::vector<std::vector<int>> &trellis, module::Encoder<B> *encoder) const
{
	try
	{
		return Decoder::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		return build_siso<B,Q>(trellis);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >* aff3ct::factory::Decoder_RSC_DB::build_siso<B_8 ,Q_8 >(const std::vector<std::vector<int>>&, module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>* aff3ct::factory::Decoder_RSC_DB::build_siso<B_16,Q_16>(const std::vector<std::vector<int>>&, module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>* aff3ct::factory::Decoder_RSC_DB::build_siso<B_32,Q_32>(const std::vector<std::vector<int>>&, module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>* aff3ct::factory::Decoder_RSC_DB::build_siso<B_64,Q_64>(const std::vector<std::vector<int>>&, module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>* aff3ct::factory::Decoder_RSC_DB::build_siso<B,Q>(const std::vector<std::vector<int>>&, module::Encoder<B>*) const;
#endif

#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RSC_DB::build<B_8 ,Q_8 >(const std::vector<std::vector<int>>&, module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_RSC_DB::build<B_16,Q_16>(const std::vector<std::vector<int>>&, module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_RSC_DB::build<B_32,Q_32>(const std::vector<std::vector<int>>&, module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_RSC_DB::build<B_64,Q_64>(const std::vector<std::vector<int>>&, module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_RSC_DB::build<B,Q>(const std::vector<std::vector<int>>&, module::Encoder<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
