#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Factory/Tools/Code/Turbo/Flip_and_check.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Flip_and_check_name   = "Flip and check";
const std::string aff3ct::factory::Flip_and_check_prefix = "fnc";

Flip_and_check
::Flip_and_check(const std::string &prefix)
: Factory(Flip_and_check_name, Flip_and_check_name, prefix)
{
}

Flip_and_check
::Flip_and_check(const std::string &name, const std::string &prefix)
: Factory(name, Flip_and_check_name, prefix)
{
}

Flip_and_check* Flip_and_check
::clone() const
{
	return new Flip_and_check(*this);
}

void Flip_and_check
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Flip_and_check::";

	tools::add_arg(args, p, class_name+"p+size",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+fra,F",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+",
		cli::None());

	tools::add_arg(args, p, class_name+"p+q",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+ite-m",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+ite-M",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+ite-s",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+ite,i",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+crc-start",
		cli::Integer(cli::Positive(), cli::Non_zero()));
}

void Flip_and_check
::store(const cli::Argument_map_value &vals)
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

void Flip_and_check
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
tools::Flip_and_check<B,Q>* Flip_and_check
::build(module::CRC<B> &crc) const
{
	if (!this->enable)
		throw tools::cannot_allocate(__FILE__, __LINE__, __func__);

	return new tools::Flip_and_check<B,Q>(this->size, this->n_ite, crc, this->crc_start_ite, this->q, this->ite_min, this->ite_max, this->ite_step, this->n_frames);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Flip_and_check<B_8 ,Q_8 >* aff3ct::factory::Flip_and_check::build<B_8 ,Q_8 >(module::CRC<B_8 >&) const;
template aff3ct::tools::Flip_and_check<B_16,Q_16>* aff3ct::factory::Flip_and_check::build<B_16,Q_16>(module::CRC<B_16>&) const;
template aff3ct::tools::Flip_and_check<B_32,Q_32>* aff3ct::factory::Flip_and_check::build<B_32,Q_32>(module::CRC<B_32>&) const;
template aff3ct::tools::Flip_and_check<B_64,Q_64>* aff3ct::factory::Flip_and_check::build<B_64,Q_64>(module::CRC<B_64>&) const;
#else
template aff3ct::tools::Flip_and_check<B,Q>* aff3ct::factory::Flip_and_check::build<B,Q>(module::CRC<B>&) const;
#endif
// ==================================================================================== explicit template instantiation
