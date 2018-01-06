#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_generic.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_DVB_RCS1.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_DVB_RCS2.hpp"

#include "Decoder_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_RSC_DB::name   = "Decoder RSC DB";
const std::string aff3ct::factory::Decoder_RSC_DB::prefix = "dec";

Decoder_RSC_DB::parameters
::parameters(const std::string prefix)
: Decoder::parameters(Decoder_RSC_DB::name, prefix)
{
	this->type   = "BCJR";
	this->implem = "GENERIC";
}

Decoder_RSC_DB::parameters
::~parameters()
{
}

Decoder_RSC_DB::parameters* Decoder_RSC_DB::parameters
::clone() const
{
	return new Decoder_RSC_DB::parameters(*this);
}

void Decoder_RSC_DB::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Decoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.erase({p+"-cw-size", "N"});

	tools::add_options<std::string>(opt_args.at({p+"-type", "D"}), 0, "BCJR");
	tools::add_options<std::string>(opt_args.at({p+"-implem"}), 0, "GENERIC", "DVB-RCS1", "DVB-RCS2");

	opt_args.add(
		{p+"-max"},
		tools::Text(tools::Including_set("MAX", "MAXL", "MAXS")),
		"the MAX implementation for the nodes.");

	opt_args.add(
		{p+"-no-buff"},
		new tools::None(),
		"does not suppose a buffered encoding.");
}

void Decoder_RSC_DB::parameters
::store(const tools::Argument_map_value &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-max"    })) this->max      = vals.at({p+"-max"});
	if(vals.exist({p+"-no-buff"})) this->buffered = false;

	this->N_cw = 2 * this->K;
	this->R    = (float)this->K / (float)this->N_cw;
}

void Decoder_RSC_DB::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->tail_length && full)
		headers[p].push_back(std::make_pair("Tail length", std::to_string(this->tail_length)));

	if (full) headers[p].push_back(std::make_pair("Buffered", (this->buffered ? "on" : "off")));

	headers[p].push_back(std::make_pair(std::string("Max type"), this->max));
}

template <typename B, typename Q, tools::proto_max<Q> MAX>
module::Decoder_RSC_DB_BCJR<B,Q>* Decoder_RSC_DB::parameters
::_build(const std::vector<std::vector<int>> &trellis) const
{
	if (this->type == "BCJR")
	{
		if (this->implem == "GENERIC" ) return new module::Decoder_RSC_DB_BCJR_generic <B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames);
		if (this->implem == "DVB-RCS1") return new module::Decoder_RSC_DB_BCJR_DVB_RCS1<B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames);
		if (this->implem == "DVB-RCS2") return new module::Decoder_RSC_DB_BCJR_DVB_RCS2<B,Q,MAX>(this->K, trellis, this->buffered, this->n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_RSC_DB_BCJR<B,Q>* Decoder_RSC_DB::parameters
::build(const std::vector<std::vector<int>> &trellis) const
{
	     if (this->max == "MAX" ) return _build<B,Q,tools::max       <Q>>(trellis);
	else if (this->max == "MAXS") return _build<B,Q,tools::max_star  <Q>>(trellis);
	else if (this->max == "MAXL") return _build<B,Q,tools::max_linear<Q>>(trellis);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_RSC_DB_BCJR<B,Q>* Decoder_RSC_DB
::build(const parameters &params, const std::vector<std::vector<int>> &trellis)
{
	return params.template build<B,Q>(trellis);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >* aff3ct::factory::Decoder_RSC_DB::parameters::build<B_8 ,Q_8 >(const std::vector<std::vector<int>>&) const;
template aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>* aff3ct::factory::Decoder_RSC_DB::parameters::build<B_16,Q_16>(const std::vector<std::vector<int>>&) const;
template aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>* aff3ct::factory::Decoder_RSC_DB::parameters::build<B_32,Q_32>(const std::vector<std::vector<int>>&) const;
template aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>* aff3ct::factory::Decoder_RSC_DB::parameters::build<B_64,Q_64>(const std::vector<std::vector<int>>&) const;
template aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >* aff3ct::factory::Decoder_RSC_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_RSC_DB::parameters&, const std::vector<std::vector<int>>&);
template aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>* aff3ct::factory::Decoder_RSC_DB::build<B_16,Q_16>(const aff3ct::factory::Decoder_RSC_DB::parameters&, const std::vector<std::vector<int>>&);
template aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>* aff3ct::factory::Decoder_RSC_DB::build<B_32,Q_32>(const aff3ct::factory::Decoder_RSC_DB::parameters&, const std::vector<std::vector<int>>&);
template aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>* aff3ct::factory::Decoder_RSC_DB::build<B_64,Q_64>(const aff3ct::factory::Decoder_RSC_DB::parameters&, const std::vector<std::vector<int>>&);
#else
template aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>* aff3ct::factory::Decoder_RSC_DB::parameters::build<B,Q>(const std::vector<std::vector<int>>&) const;
template aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>* aff3ct::factory::Decoder_RSC_DB::build<B,Q>(const aff3ct::factory::Decoder_RSC_DB::parameters&, const std::vector<std::vector<int>>&);
#endif
// ==================================================================================== explicit template instantiation
