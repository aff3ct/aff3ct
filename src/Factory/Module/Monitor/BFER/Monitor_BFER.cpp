#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Factory/Module/Monitor/BFER/Monitor_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor_BFER_name   = "Monitor BER/FER";
const std::string aff3ct::factory::Monitor_BFER_prefix = "mnt";

Monitor_BFER
::Monitor_BFER(const std::string &prefix)
: Monitor(Monitor_name, prefix)
{
}

Monitor_BFER* Monitor_BFER
::clone() const
{
	return new Monitor_BFER(*this);
}

void Monitor_BFER
::get_description(cli::Argument_map_info &args) const
{
	Monitor::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Monitor_BFER::";

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+max-fe,e",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+max-fra,n",
		cli::Integer(cli::Positive()),
		cli::arg_rank::ADV);

	tools::add_arg(args, p, class_name+"p+err-hist",
		cli::Integer(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+err-hist-path",
		cli::File(cli::openmode::write));
}

void Monitor_BFER
::store(const cli::Argument_map_value &vals)
{
	Monitor::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K              = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-max-fe",    "e"})) this->n_frame_errors = vals.to_int({p+"-max-fe",    "e"});
	if(vals.exist({p+"-err-hist"      })) this->err_hist       = vals.to_int({p+"-err-hist"      });
	if(vals.exist({p+"-err-hist-path" })) this->err_hist_path  = vals.at    ({p+"-err-hist-path" });
	if(vals.exist({p+"-max-fra",   "n"})) this->max_frame      = vals.to_int({p+"-max-fra",   "n"});
}

void Monitor_BFER
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Monitor::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Frame error count (e)", std::to_string(this->n_frame_errors)));
	if (full) headers[p].push_back(std::make_pair("Size (K)",          std::to_string(this->K       )));

	if (this->err_hist >= 0)
		headers[p].push_back(std::make_pair("Error histogram path", this->err_hist_path));
}

template <typename B>
module::Monitor_BFER<B>* Monitor_BFER
::build(bool count_unknown_values) const
{
	if (this->type == "STD") return new module::Monitor_BFER<B>(this->K, this->n_frame_errors, this->max_frame, count_unknown_values);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Monitor_BFER<B_8 >* aff3ct::factory::Monitor_BFER::build<B_8 >(bool) const;
template aff3ct::module::Monitor_BFER<B_16>* aff3ct::factory::Monitor_BFER::build<B_16>(bool) const;
template aff3ct::module::Monitor_BFER<B_32>* aff3ct::factory::Monitor_BFER::build<B_32>(bool) const;
template aff3ct::module::Monitor_BFER<B_64>* aff3ct::factory::Monitor_BFER::build<B_64>(bool) const;
#else
template aff3ct::module::Monitor_BFER<B>* aff3ct::factory::Monitor_BFER::build<B>(bool) const;
#endif