#include "Monitor_EXIT.hpp"

#include "Tools/Exception/exception.hpp"

#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"


using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor_EXIT::name   = "Monitor EXIT";
const std::string aff3ct::factory::Monitor_EXIT::prefix = "mnt";

Monitor_EXIT::parameters
::parameters(const std::string prefix)
: Monitor::parameters(Monitor_EXIT::name, prefix)
{
}

Monitor_EXIT::parameters
::~parameters()
{
}

Monitor_EXIT::parameters* Monitor_EXIT::parameters
::clone() const
{
	return new Monitor_EXIT::parameters(*this);
}

void Monitor_EXIT::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Monitor::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args[{p+"-size", "K"}] =
		{"positive_int",
		 "number of bits to check."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-trials", "n"}] =
		{"positive_int",
		 "number of frames to simulate per sigma A value."};
}

void Monitor_EXIT::parameters
::store(const arg_val_map &vals)
{
	Monitor::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-size",   "K"})) this->size     = std::stoi(vals.at({p+"-size",   "K"}));
	if(exist(vals, {p+"-fra",    "F"})) this->n_frames = std::stoi(vals.at({p+"-fra",    "F"}));
	if(exist(vals, {p+"-trials", "n"})) this->n_trials = std::stoi(vals.at({p+"-trials", "n"}));
}

void Monitor_EXIT::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Monitor::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Number of trials", std::to_string(this->n_trials)));
	if (full) headers[p].push_back(std::make_pair("Size (K)", std::to_string(this->size)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B, typename R>
module::Monitor_EXIT<B,R>* Monitor_EXIT::parameters
::build() const
{
	if (this->type == "STD") return new module::Monitor_EXIT<B,R>(this->size, this->n_trials, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
module::Monitor_EXIT<B,R>* Monitor_EXIT
::build(const parameters& params)
{
	return params.template build<B,R>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Monitor_EXIT<B_32,R_32>* aff3ct::factory::Monitor_EXIT::parameters::build<B_32,R_32>() const;
template aff3ct::module::Monitor_EXIT<B_64,R_64>* aff3ct::factory::Monitor_EXIT::parameters::build<B_64,R_64>() const;
template aff3ct::module::Monitor_EXIT<B_32,R_32>* aff3ct::factory::Monitor_EXIT::build<B_32,R_32>(const aff3ct::factory::Monitor_EXIT::parameters&);
template aff3ct::module::Monitor_EXIT<B_64,R_64>* aff3ct::factory::Monitor_EXIT::build<B_64,R_64>(const aff3ct::factory::Monitor_EXIT::parameters&);
#else
template aff3ct::module::Monitor_EXIT<B,R>* aff3ct::factory::Monitor_EXIT::parameters::build<B,R>() const;
template aff3ct::module::Monitor_EXIT<B,R>* aff3ct::factory::Monitor_EXIT::build<B,R>(const aff3ct::factory::Monitor_EXIT::parameters&);
#endif
// ==================================================================================== explicit template instantiation
