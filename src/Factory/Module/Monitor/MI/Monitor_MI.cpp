#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"

#include "Module/Monitor/MI/Monitor_MI.hpp"

#include "Monitor_MI.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor_MI_name   = "Monitor MI";
const std::string aff3ct::factory::Monitor_MI_prefix = "mnt";

Monitor_MI::parameters
::parameters(const std::string &prefix)
: Monitor::parameters(Monitor_name, prefix)
{
}

Monitor_MI::parameters* Monitor_MI::parameters
::clone() const
{
	return new Monitor_MI::parameters(*this);
}

void Monitor_MI::parameters
::get_description(tools::Argument_map_info &args) const
{
	Monitor::parameters::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Monitor_MI::parameters::";

	// args.add(
	// 	{p+"-fra-size", "N"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"the frame size for the mutual information computation.",
	// 	tools::arg_rank::REQ);
	tools::add_arg(args, p, class_name+"p+fra-size,N",
		tools::Integer(tools::Positive(), tools::Non_zero()),
		tools::arg_rank::REQ);

	// args.add(
	// 	{p+"-fra", "F"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"set the number of inter frame level to process.");
	tools::add_arg(args, p, class_name+"p+fra,F",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	// args.add(
	// 	{p+"-trials", "n"},
	// 	tools::Integer(tools::Positive(), tools::Non_zero()),
	// 	"number of frames to simulate.");
	tools::add_arg(args, p, class_name+"p+trials,n",
		tools::Integer(tools::Positive(), tools::Non_zero()));
}

void Monitor_MI::parameters
::store(const tools::Argument_map_value &vals)
{
	Monitor::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-fra-size", "N"})) this->N        = vals.to_int({p+"-fra-size", "N"});
	if(vals.exist({p+"-fra",      "F"})) this->n_frames = vals.to_int({p+"-fra",      "F"});
	if(vals.exist({p+"-trials",   "n"})) this->n_trials = vals.to_int({p+"-trials",   "n"});
}

void Monitor_MI::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Monitor::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Number of trials (n)",        std::to_string(this->n_trials)));
	if (full) headers[p].push_back(std::make_pair("Size (N)",          std::to_string(this->N       )));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B, typename R>
module::Monitor_MI<B,R>* Monitor_MI::parameters
::build() const
{
	if (this->type == "STD") return new module::Monitor_MI<B,R>(this->N, this->n_trials, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
module::Monitor_MI<B,R>* Monitor_MI
::build(const parameters& params)
{
	return params.template build<B,R>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Monitor_MI<B_8 ,R_8 >* aff3ct::factory::Monitor_MI::parameters::build<B_8, R_8 >() const;
template aff3ct::module::Monitor_MI<B_16,R_16>* aff3ct::factory::Monitor_MI::parameters::build<B_16,R_16>() const;
template aff3ct::module::Monitor_MI<B_32,R_32>* aff3ct::factory::Monitor_MI::parameters::build<B_32,R_32>() const;
template aff3ct::module::Monitor_MI<B_64,R_64>* aff3ct::factory::Monitor_MI::parameters::build<B_64,R_64>() const;
template aff3ct::module::Monitor_MI<B_8 ,R_8 >* aff3ct::factory::Monitor_MI::build<B_8, R_8 >(const aff3ct::factory::Monitor_MI::parameters&);
template aff3ct::module::Monitor_MI<B_16,R_16>* aff3ct::factory::Monitor_MI::build<B_16,R_16>(const aff3ct::factory::Monitor_MI::parameters&);
template aff3ct::module::Monitor_MI<B_32,R_32>* aff3ct::factory::Monitor_MI::build<B_32,R_32>(const aff3ct::factory::Monitor_MI::parameters&);
template aff3ct::module::Monitor_MI<B_64,R_64>* aff3ct::factory::Monitor_MI::build<B_64,R_64>(const aff3ct::factory::Monitor_MI::parameters&);
#else
template aff3ct::module::Monitor_MI<B,R>* aff3ct::factory::Monitor_MI::parameters::build<B,R>() const;
template aff3ct::module::Monitor_MI<B,R>* aff3ct::factory::Monitor_MI::build<B,R>(const aff3ct::factory::Monitor_MI::parameters&);
#endif
// ==================================================================================== explicit template instantiation
