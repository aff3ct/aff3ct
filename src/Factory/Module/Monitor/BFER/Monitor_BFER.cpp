#include "Tools/Exception/exception.hpp"

#include "Module/Monitor/BFER/Monitor_BFER.hpp"

#include "Monitor_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor_BFER_name   = "Monitor BER/FER";
const std::string aff3ct::factory::Monitor_BFER_prefix = "mnt";

Monitor_BFER::parameters
::parameters(const std::string &prefix)
: Monitor::parameters(Monitor_name, prefix)
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
::get_description(tools::Argument_map_info &args) const
{
	Monitor::parameters::get_description(args);

	auto p = this->get_prefix();

	args.add(
		{p+"-cw-size", "N"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"the codeword size for the mutual information computation.",
		tools::arg_rank::REQ);

	args.add(
		{p+"-info-bits", "K"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"number of bits to check.",
		tools::arg_rank::REQ);

	args.add(
		{p+"-fra", "F"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"set the number of inter frame level to process.");

	args.add(
		{p+"-max-fe", "e"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"max number of frame errors for each SNR simulation.");
}

void Monitor_BFER::parameters
::store(const tools::Argument_map_value &vals)
{
	Monitor::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-cw-size",   "N"})) this->N              = vals.to_int({p+"-cw-size",   "N"});
	if(vals.exist({p+"-info-bits", "K"})) this->K              = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-fra",       "F"})) this->n_frames       = vals.to_int({p+"-fra",       "F"});
	if(vals.exist({p+"-max-fe",    "e"})) this->n_frame_errors = vals.to_int({p+"-max-fe",    "e"});
}

void Monitor_BFER::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Monitor::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Frame error count (e)", std::to_string(this->n_frame_errors)));
	if (full) headers[p].push_back(std::make_pair("K",                 std::to_string(this->K       )));
	if (full) headers[p].push_back(std::make_pair("N",                 std::to_string(this->N       )));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B, typename R>
module::Monitor_BFER<B,R>* Monitor_BFER::parameters
::build() const
{
	if (this->type == "STD") return new module::Monitor_BFER<B,R>(this->K, this->N, this->n_frame_errors, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
module::Monitor_BFER<B,R>* Monitor_BFER
::build(const parameters& params)
{
	return params.template build<B,R>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Monitor_BFER<B_8 ,Q_8 >* aff3ct::factory::Monitor_BFER::parameters::build<B_8, Q_8 >() const;
template aff3ct::module::Monitor_BFER<B_16,Q_16>* aff3ct::factory::Monitor_BFER::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Monitor_BFER<B_32,Q_32>* aff3ct::factory::Monitor_BFER::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Monitor_BFER<B_64,Q_64>* aff3ct::factory::Monitor_BFER::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Monitor_BFER<B_8 ,Q_8 >* aff3ct::factory::Monitor_BFER::build<B_8, Q_8 >(const aff3ct::factory::Monitor_BFER::parameters&);
template aff3ct::module::Monitor_BFER<B_16,Q_16>* aff3ct::factory::Monitor_BFER::build<B_16,Q_16>(const aff3ct::factory::Monitor_BFER::parameters&);
template aff3ct::module::Monitor_BFER<B_32,Q_32>* aff3ct::factory::Monitor_BFER::build<B_32,Q_32>(const aff3ct::factory::Monitor_BFER::parameters&);
template aff3ct::module::Monitor_BFER<B_64,Q_64>* aff3ct::factory::Monitor_BFER::build<B_64,Q_64>(const aff3ct::factory::Monitor_BFER::parameters&);
#else
template aff3ct::module::Monitor_BFER<B,Q>* aff3ct::factory::Monitor_BFER::parameters::build<B,Q>() const;
template aff3ct::module::Monitor_BFER<B,Q>* aff3ct::factory::Monitor_BFER::build<B,Q>(const aff3ct::factory::Monitor_BFER::parameters&);
#endif
// ==================================================================================== explicit template instantiation
