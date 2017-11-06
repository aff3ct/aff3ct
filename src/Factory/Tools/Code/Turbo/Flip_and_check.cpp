#include "Tools/Exception/exception.hpp"

#include "Flip_and_check.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Flip_and_check::name   = "Flip and check";
const std::string aff3ct::factory::Flip_and_check::prefix = "fnc";

Flip_and_check::parameters
::parameters(const std::string prefix)
: Factory::parameters(Flip_and_check::name, Flip_and_check::name, prefix)
{
}

Flip_and_check::parameters
::parameters(const std::string name, const std::string prefix)
: Factory::parameters(name, Flip_and_check::name, prefix)
{
}

Flip_and_check::parameters
::~parameters()
{
}

Flip_and_check::parameters* Flip_and_check::parameters
::clone() const
{
	return new Flip_and_check::parameters(*this);
}

void Flip_and_check::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	auto p = this->get_prefix();

	req_args[{p+"-size", "K"}] =
		{"strictly_positive_int",
		 "size (in bit) of the extrinsic for the fnc processing."};

	opt_args[{p}] =
		{"",
		 "enables the flip and check decoder (requires \"--crc-type\")."};

	opt_args[{p+"-q"}] =
		{"strictly_positive_int",
		 "set the search's space for the fnc algorithm."};

	opt_args[{p+"-ite-m"}] =
		{"positive_int",
		 "set first iteration at which the fnc is used."};

	opt_args[{p+"-ite-M"}] =
		{"positive_int",
		 "set last iteration at which the fnc is used."};

	opt_args[{p+"-ite-s"}] =
		{"strictly_positive_int",
		 "set iteration step for the fnc algorithm."};

	opt_args[{p+"-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo."};

	opt_args[{p+"-crc-ite"}] =
		{"positive_int",
		 "set the iteration to start the CRC checking."};

	opt_args[{p+"-fra", "F"}] =
		{"strictly_positive_int",
		 "set the number of inter frame level to process."};
}

void Flip_and_check::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p             })) this->enable              = true;
	if(exist(vals, {p+"-size"     })) this->size                = std::stoi(vals.at({p+"-size"     }));
	if(exist(vals, {p+"-q"        })) this->q                   = std::stoi(vals.at({p+"-q"        }));
	if(exist(vals, {p+"-ite-s"    })) this->ite_step            = std::stoi(vals.at({p+"-ite-s"    }));
	if(exist(vals, {p+"-ite",  "i"})) this->n_ite               = std::stoi(vals.at({p+"-ite",  "i"}));
	if(exist(vals, {p+"-ite-m"    })) this->ite_min             = std::stoi(vals.at({p+"-ite-m"    }));
	if(exist(vals, {p+"-ite-M"    })) this->ite_max             = std::stoi(vals.at({p+"-ite-M"    }));
	else                              this->ite_max             = this->n_ite;
	if(exist(vals, {p+"-crc-ite"  })) this->start_crc_check_ite = std::stoi(vals.at({p+"-crc-ite"  }));
	if(exist(vals, {p+"-fra",  "F"})) this->n_frames            = std::stoi(vals.at({p+"-fra", "F" }));
}

void Flip_and_check::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Enabled", ((this->enable) ? "yes" : "no")));
	if (this->enable)
	{
		headers[p].push_back(std::make_pair("FNC q", std::to_string(this->q)));
		headers[p].push_back(std::make_pair("FNC ite min", std::to_string(this->ite_min)));
		headers[p].push_back(std::make_pair("FNC ite max", std::to_string(this->ite_max)));
		headers[p].push_back(std::make_pair("FNC ite step", std::to_string(this->ite_step)));
	}
}

template<typename B, typename Q>
tools::Flip_and_check<B,Q>* Flip_and_check::parameters
::build(module::CRC<B> &crc) const
{
	if (!this->enable)
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	return new tools::Flip_and_check<B,Q>(this->size, this->n_ite, crc, this->start_crc_check_ite, this->q, this->ite_min, this->ite_max, this->ite_step, this->n_frames);
}

template<typename B, typename Q>
tools::Flip_and_check<B,Q>* Flip_and_check
::build(const parameters& params, module::CRC<B> &crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Flip_and_check<B_8 ,Q_8 >* aff3ct::factory::Flip_and_check::parameters::build<B_8 ,Q_8 >(module::CRC<B_8 >&) const;
template aff3ct::tools::Flip_and_check<B_16,Q_16>* aff3ct::factory::Flip_and_check::parameters::build<B_16,Q_16>(module::CRC<B_16>&) const;
template aff3ct::tools::Flip_and_check<B_32,Q_32>* aff3ct::factory::Flip_and_check::parameters::build<B_32,Q_32>(module::CRC<B_32>&) const;
template aff3ct::tools::Flip_and_check<B_64,Q_64>* aff3ct::factory::Flip_and_check::parameters::build<B_64,Q_64>(module::CRC<B_64>&) const;
template aff3ct::tools::Flip_and_check<B_8 ,Q_8 >* aff3ct::factory::Flip_and_check::build<B_8 ,Q_8 >(const aff3ct::factory::Flip_and_check::parameters&, module::CRC<B_8 >&);
template aff3ct::tools::Flip_and_check<B_16,Q_16>* aff3ct::factory::Flip_and_check::build<B_16,Q_16>(const aff3ct::factory::Flip_and_check::parameters&, module::CRC<B_16>&);
template aff3ct::tools::Flip_and_check<B_32,Q_32>* aff3ct::factory::Flip_and_check::build<B_32,Q_32>(const aff3ct::factory::Flip_and_check::parameters&, module::CRC<B_32>&);
template aff3ct::tools::Flip_and_check<B_64,Q_64>* aff3ct::factory::Flip_and_check::build<B_64,Q_64>(const aff3ct::factory::Flip_and_check::parameters&, module::CRC<B_64>&);
#else
template aff3ct::tools::Flip_and_check<B,Q>* aff3ct::factory::Flip_and_check::parameters::build<B,Q>(module::CRC<B>&) const;
template aff3ct::tools::Flip_and_check<B,Q>* aff3ct::factory::Flip_and_check::build<B,Q>(const aff3ct::factory::Flip_and_check::parameters&, module::CRC<B>&);
#endif
// ==================================================================================== explicit template instantiation
