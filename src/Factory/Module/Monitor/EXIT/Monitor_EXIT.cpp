#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"
#include "Factory/Module/Monitor/EXIT/Monitor_EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor_EXIT_name   = "Monitor EXIT";
const std::string aff3ct::factory::Monitor_EXIT_prefix = "mnt";

Monitor_EXIT
::Monitor_EXIT(const std::string &prefix)
: Monitor(Monitor_EXIT_name, prefix)
{
}

Monitor_EXIT* Monitor_EXIT
::clone() const
{
	return new Monitor_EXIT(*this);
}

void Monitor_EXIT
::get_description(cli::Argument_map_info &args) const
{
	Monitor::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Monitor_EXIT::";

	tools::add_arg(args, p, class_name+"p+size,K",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+fra,F",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+trials,n",
		cli::Integer(cli::Positive(), cli::Non_zero()));
}

void Monitor_EXIT
::store(const cli::Argument_map_value &vals)
{
	Monitor::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-size",   "K"})) this->size     = vals.to_int({p+"-size",   "K"});
	if(vals.exist({p+"-fra",    "F"})) this->n_frames = vals.to_int({p+"-fra",    "F"});
	if(vals.exist({p+"-trials", "n"})) this->n_trials = vals.to_int({p+"-trials", "n"});
}

void Monitor_EXIT
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Monitor::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Number of trials", std::to_string(this->n_trials)));
	if (full) headers[p].push_back(std::make_pair("Size (K)", std::to_string(this->size)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B, typename R>
module::Monitor_EXIT<B,R>* Monitor_EXIT
::build() const
{
	if (this->type == "STD") return new module::Monitor_EXIT<B,R>(this->size, this->n_trials, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Monitor_EXIT<B_32,R_32>* aff3ct::factory::Monitor_EXIT::build<B_32,R_32>() const;
template aff3ct::module::Monitor_EXIT<B_64,R_64>* aff3ct::factory::Monitor_EXIT::build<B_64,R_64>() const;
#else
template aff3ct::module::Monitor_EXIT<B,R>* aff3ct::factory::Monitor_EXIT::build<B,R>() const;
#endif
// ==================================================================================== explicit template instantiation
