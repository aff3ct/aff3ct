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
		"max number of frame errors for each noise point simulation.");

	args.add(
		{p+"-max-frame", "n"},
		tools::Integer(tools::Positive()),
		"maximum number of frames for each noise point simulation.",
		tools::arg_rank::ADV);

	args.add(
		{p+"-err-hist"},
		tools::Integer(tools::Positive()),
		"activate the histogram of the number of errors per frame. Set the max number of bit error per frame included in the histogram (0 is no limit).");

	args.add(
		{p+"-err-hist-path"},
		tools::File(tools::openmode::write),
		"path to the output histogram (default is './hist', add automatically the current noise value and the extension '.txt')");
}

void Monitor_BFER::parameters
::store(const tools::Argument_map_value &vals)
{
	Monitor::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K              = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-fra",       "F"})) this->n_frames       = vals.to_int({p+"-fra",       "F"});
	if(vals.exist({p+"-max-fe",    "e"})) this->n_frame_errors = vals.to_int({p+"-max-fe",    "e"});
	if(vals.exist({p+"-err-hist"      })) this->err_hist       = vals.to_int({p+"-err-hist"      });
	if(vals.exist({p+"-err-hist-path" })) this->err_hist_path  = vals.at    ({p+"-err-hist-path" });
	if(vals.exist({p+"-max-frame", "n"})) this->max_frame      = vals.to_int({p+"-max-frame", "n"});
}

void Monitor_BFER::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Monitor::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Frame error count (e)", std::to_string(this->n_frame_errors)));
	if (full) headers[p].push_back(std::make_pair("Size (K)",          std::to_string(this->K       )));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));

	if (this->err_hist >= 0)
		headers[p].push_back(std::make_pair("Error histogram path", this->err_hist_path));
}

template <typename B>
module::Monitor_BFER<B>* Monitor_BFER::parameters
::build(bool count_unknown_values) const
{
	if (this->type == "STD") return new module::Monitor_BFER<B>(this->K, this->n_frame_errors, this->max_frame, count_unknown_values, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Monitor_BFER<B>* Monitor_BFER
::build(const parameters& params, bool count_unknown_values)
{
	return params.template build<B>(count_unknown_values);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Monitor_BFER<B_8 >* aff3ct::factory::Monitor_BFER::parameters::build<B_8 >(bool) const;
template aff3ct::module::Monitor_BFER<B_16>* aff3ct::factory::Monitor_BFER::parameters::build<B_16>(bool) const;
template aff3ct::module::Monitor_BFER<B_32>* aff3ct::factory::Monitor_BFER::parameters::build<B_32>(bool) const;
template aff3ct::module::Monitor_BFER<B_64>* aff3ct::factory::Monitor_BFER::parameters::build<B_64>(bool) const;
template aff3ct::module::Monitor_BFER<B_8 >* aff3ct::factory::Monitor_BFER::build<B_8 >(const aff3ct::factory::Monitor_BFER::parameters&, bool);
template aff3ct::module::Monitor_BFER<B_16>* aff3ct::factory::Monitor_BFER::build<B_16>(const aff3ct::factory::Monitor_BFER::parameters&, bool);
template aff3ct::module::Monitor_BFER<B_32>* aff3ct::factory::Monitor_BFER::build<B_32>(const aff3ct::factory::Monitor_BFER::parameters&, bool);
template aff3ct::module::Monitor_BFER<B_64>* aff3ct::factory::Monitor_BFER::build<B_64>(const aff3ct::factory::Monitor_BFER::parameters&, bool);
#else
template aff3ct::module::Monitor_BFER<B>* aff3ct::factory::Monitor_BFER::parameters::build<B>(bool) const;
template aff3ct::module::Monitor_BFER<B>* aff3ct::factory::Monitor_BFER::build<B>(const aff3ct::factory::Monitor_BFER::parameters&, bool);
#endif