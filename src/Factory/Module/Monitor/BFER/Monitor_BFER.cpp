#include "Tools/Exception/exception.hpp"

#include "Module/Monitor/BFER/Monitor_BFER.hpp"

#include "Monitor_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor_BFER::name   = "Monitor BER/FER";
const std::string aff3ct::factory::Monitor_BFER::prefix = "mnt";

Monitor_BFER::parameters
::parameters(const std::string prefix)
: Monitor::parameters(Monitor::name, prefix)
{
}

Monitor_BFER::parameters
::~parameters()
{
}

Monitor_BFER::parameters* Monitor_BFER::parameters
::clone() const
{
	return new Monitor_BFER::parameters(*this);
}

void Monitor_BFER::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Monitor::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args[{p+"-size", "K"}] =
		{"strictly_positive_int",
		 "number of bits to check."};

	opt_args[{p+"-fra", "F"}] =
		{"strictly_positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-max-fe", "e"}] =
		{"positive_int",
		 "max number of frame errors for each SNR simulation."};
}

void Monitor_BFER::parameters
::store(const arg_val_map &vals)
{
	Monitor::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-size",   "K"})) this->size           = std::stoi(vals.at({p+"-size",   "K"}));
	if(exist(vals, {p+"-fra",    "F"})) this->n_frames       = std::stoi(vals.at({p+"-fra",    "F"}));
	if(exist(vals, {p+"-max-fe", "e"})) this->n_frame_errors = std::stoi(vals.at({p+"-max-fe", "e"}));
}

void Monitor_BFER::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Monitor::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Frame error count (e)", std::to_string(this->n_frame_errors)));
	if (full) headers[p].push_back(std::make_pair("Size (K)", std::to_string(this->size)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B>
module::Monitor_BFER<B>* Monitor_BFER::parameters
::build() const
{
	if (this->type == "STD") return new module::Monitor_BFER<B>(this->size, this->n_frame_errors, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Monitor_BFER<B>* Monitor_BFER
::build(const parameters& params)
{
	return params.template build<B>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Monitor_BFER<B_8 >* aff3ct::factory::Monitor_BFER::parameters::build<B_8 >() const;
template aff3ct::module::Monitor_BFER<B_16>* aff3ct::factory::Monitor_BFER::parameters::build<B_16>() const;
template aff3ct::module::Monitor_BFER<B_32>* aff3ct::factory::Monitor_BFER::parameters::build<B_32>() const;
template aff3ct::module::Monitor_BFER<B_64>* aff3ct::factory::Monitor_BFER::parameters::build<B_64>() const;
template aff3ct::module::Monitor_BFER<B_8 >* aff3ct::factory::Monitor_BFER::build<B_8 >(const aff3ct::factory::Monitor_BFER::parameters&);
template aff3ct::module::Monitor_BFER<B_16>* aff3ct::factory::Monitor_BFER::build<B_16>(const aff3ct::factory::Monitor_BFER::parameters&);
template aff3ct::module::Monitor_BFER<B_32>* aff3ct::factory::Monitor_BFER::build<B_32>(const aff3ct::factory::Monitor_BFER::parameters&);
template aff3ct::module::Monitor_BFER<B_64>* aff3ct::factory::Monitor_BFER::build<B_64>(const aff3ct::factory::Monitor_BFER::parameters&);
#else
template aff3ct::module::Monitor_BFER<B>* aff3ct::factory::Monitor_BFER::parameters::build<B>() const;
template aff3ct::module::Monitor_BFER<B>* aff3ct::factory::Monitor_BFER::build<B>(const aff3ct::factory::Monitor_BFER::parameters&);
#endif
// ==================================================================================== explicit template instantiation
