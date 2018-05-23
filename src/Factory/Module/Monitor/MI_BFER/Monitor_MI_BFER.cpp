#include "Tools/Exception/exception.hpp"

#include "Module/Monitor/BFER/Monitor_MI_BFER.hpp"

#include "Monitor_MI_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor_MI_BFER_name   = "Monitor MI";
const std::string aff3ct::factory::Monitor_MI_BFER_prefix = "mnt";

Monitor_MI_BFER::parameters
::parameters(const std::string &prefix)
: Monitor_MI::parameters  (prefix),
  Monitor_BFER::parameters(prefix)
{
}

Monitor_MI_BFER::parameters
::~parameters()
{
}

Monitor_MI_BFER::parameters* Monitor_MI_BFER::parameters
::clone() const
{
	return new Monitor_MI_BFER::parameters(*this);
}

void Monitor_MI_BFER::parameters
::get_description(tools::Argument_map_info &args) const
{
	Monitor_MI  ::parameters::get_description(args);
	Monitor_BFER::parameters::get_description(args);

	auto p = this->get_prefix();

	args.erase({p+"-size", "N"})
	args.erase({p+"-size", "K"})


	args.add(
		{p+"-info-bits", "K"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"useful number of bit transmitted (information bits).",
		tools::arg_rank::REQ);

	args.add(
		{p+"-fra-size", "N"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"useful number of bit transmitted (information bits).",
		tools::arg_rank::REQ);

	args.add(
		{p+"-mutinfo"},
		tools::None(),
		"allow the computation of the mutual information.");
}

void Monitor_MI_BFER::parameters
::store(const tools::Argument_map_value &vals)
{
	Monitor_MI  ::parameters::store(vals);
	Monitor_BFER::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-fra-size",  "N"})) this->N = vals.to_int({p+"-fra-size",  "N"});
	if(vals.exist({p+"-mutinfo"       })) this->mutinfo = true;

	Monitor_BFER::parameters::size = this->K;
	Monitor_MI  ::parameters::size = this->N;

	this->n_frames = Monitor_MI::parameters::n_frames;
}

void Monitor_MI_BFER::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Monitor::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Compute mutual info",         this->mutinfo ? "yes" : "no"));
	headers[p].push_back(std::make_pair("Number of trials (n)",        std::to_string(this->n_trials)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B, typename R>
module::Monitor_MI_BFER<B,R>* Monitor_MI_BFER::parameters
::build(bool count_unknown_values) const
{
	if (this->type == "STD") return new module::Monitor_MI_BFER<B,R>(this->K, this->N, this->n_trials, this->n_frame_errors, count_unknown_values, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
module::Monitor_MI_BFER<B,R>* Monitor_MI_BFER
::build(const parameters& params, bool count_unknown_values)
{
	return params.template build<B,R>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Monitor_MI_BFER<B_8 ,R_8 >* aff3ct::factory::Monitor_MI_BFER::parameters::build<B_8, R_8 >(bool) const;
template aff3ct::module::Monitor_MI_BFER<B_16,R_16>* aff3ct::factory::Monitor_MI_BFER::parameters::build<B_16,R_16>(bool) const;
template aff3ct::module::Monitor_MI_BFER<B_32,R_32>* aff3ct::factory::Monitor_MI_BFER::parameters::build<B_32,R_32>(bool) const;
template aff3ct::module::Monitor_MI_BFER<B_64,R_64>* aff3ct::factory::Monitor_MI_BFER::parameters::build<B_64,R_64>(bool) const;
template aff3ct::module::Monitor_MI_BFER<B_8 ,R_8 >* aff3ct::factory::Monitor_MI_BFER::build<B_8, R_8 >(const aff3ct::factory::Monitor_MI_BFER::parameters&, bool);
template aff3ct::module::Monitor_MI_BFER<B_16,R_16>* aff3ct::factory::Monitor_MI_BFER::build<B_16,R_16>(const aff3ct::factory::Monitor_MI_BFER::parameters&, bool);
template aff3ct::module::Monitor_MI_BFER<B_32,R_32>* aff3ct::factory::Monitor_MI_BFER::build<B_32,R_32>(const aff3ct::factory::Monitor_MI_BFER::parameters&, bool);
template aff3ct::module::Monitor_MI_BFER<B_64,R_64>* aff3ct::factory::Monitor_MI_BFER::build<B_64,R_64>(const aff3ct::factory::Monitor_MI_BFER::parameters&, bool);
#else
template aff3ct::module::Monitor_MI_BFER<B,R>* aff3ct::factory::Monitor_MI_BFER::parameters::build<B,R>(bool) const;
template aff3ct::module::Monitor_MI_BFER<B,R>* aff3ct::factory::Monitor_MI_BFER::build<B,R>(const aff3ct::factory::Monitor_MI_BFER::parameters&, bool);
#endif
// ==================================================================================== explicit template instantiation
