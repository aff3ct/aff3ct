#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"

#include "Flip_and_check.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Flip_and_check_name   = "Flip and check";
const std::string aff3ct::factory::Flip_and_check_prefix = "fnc";

Flip_and_check::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Flip_and_check_name, Flip_and_check_name, prefix)
{
}

Flip_and_check::parameters
::parameters(const std::string &name, const std::string &prefix)
: Factory::parameters(name, Flip_and_check_name, prefix)
{
}

Flip_and_check::parameters* Flip_and_check::parameters
::clone() const
{
	return new Flip_and_check::parameters(*this);
}

void Flip_and_check::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Flip_and_check::parameters::";

	tools::add_arg(args, p, class_name+"p+size",
		tools::Integer(tools::Positive(), tools::Non_zero()),
		tools::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+fra,F",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+",
		tools::None());

	tools::add_arg(args, p, class_name+"p+q",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+ite-m",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+ite-M",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+ite-s",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+ite",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+crc-start",
		tools::Integer(tools::Positive(), tools::Non_zero()));
}

void Flip_and_check::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p             })) this->enable        = true;
	if(vals.exist({p+"-size"     })) this->size          = vals.to_int({p+"-size"     });
	if(vals.exist({p+"-q"        })) this->q             = vals.to_int({p+"-q"        });
	if(vals.exist({p+"-crc-start"})) this->crc_start_ite = vals.to_int({p+"-crc-ite"  });
	if(vals.exist({p+"-fra",  "F"})) this->n_frames      = vals.to_int({p+"-fra", "F" });
	if(vals.exist({p+"-ite-s"    })) this->ite_step      = vals.to_int({p+"-ite-s"    });
	if(vals.exist({p+"-ite",  "i"})) this->n_ite         = vals.to_int({p+"-ite",  "i"});
	if(vals.exist({p+"-ite-m"    })) this->ite_min       = vals.to_int({p+"-ite-m"    });
	if(vals.exist({p+"-ite-M"    })) this->ite_max       = vals.to_int({p+"-ite-M"    });
	else                             this->ite_max       = this->n_ite;
}

void Flip_and_check::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Enabled", ((this->enable) ? "yes" : "no")));
	if (this->enable)
	{
		headers[p].push_back(std::make_pair("FNC q",        std::to_string(this->q       )));
		headers[p].push_back(std::make_pair("FNC ite min",  std::to_string(this->ite_min )));
		headers[p].push_back(std::make_pair("FNC ite max",  std::to_string(this->ite_max )));
		headers[p].push_back(std::make_pair("FNC ite step", std::to_string(this->ite_step)));
	}
}

template<typename B, typename Q>
tools::Flip_and_check<B,Q>* Flip_and_check::parameters
::build(module::CRC<B> &crc) const
{
	if (!this->enable)
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	return new tools::Flip_and_check<B,Q>(this->size, this->n_ite, crc, this->crc_start_ite, this->q, this->ite_min, this->ite_max, this->ite_step, this->n_frames);
}

template<typename B, typename Q>
tools::Flip_and_check<B,Q>* Flip_and_check
::build(const parameters& params, module::CRC<B> &crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
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
