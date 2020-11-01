#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Tools/Interleaver/Random_column/Interleaver_core_random_column.hpp"
#include "Tools/Interleaver/Row_column/Interleaver_core_row_column.hpp"
#include "Tools/Interleaver/Column_row/Interleaver_core_column_row.hpp"
#include "Tools/Interleaver/LTE/Interleaver_core_LTE.hpp"
#include "Tools/Interleaver/CCSDS/Interleaver_core_CCSDS.hpp"
#include "Tools/Interleaver/ARP/Interleaver_core_ARP_DVB_RCS1.hpp"
#include "Tools/Interleaver/ARP/Interleaver_core_ARP_DVB_RCS2.hpp"
#include "Tools/Interleaver/NO/Interleaver_core_NO.hpp"
#include "Tools/Interleaver/Golden/Interleaver_core_golden.hpp"
#include "Tools/Interleaver/Random/Interleaver_core_random.hpp"
#include "Tools/Interleaver/User/Interleaver_core_user.hpp"
#include "Factory/Tools/Interleaver/Interleaver_core.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Interleaver_core_name   = "Interleaver";
const std::string aff3ct::factory::Interleaver_core_prefix = "itl";

Interleaver_core
::Interleaver_core(const std::string &prefix)
: Factory(Interleaver_core_name, Interleaver_core_name, prefix)
{
}

Interleaver_core* Interleaver_core
::clone() const
{
	return new Interleaver_core(*this);
}

void Interleaver_core
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Interleaver_core::";

	tools::add_arg(args, p, class_name+"p+size",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("LTE", "CCSDS", "DVB-RCS1", "DVB-RCS2", "RANDOM", "GOLDEN", "USER", "RAND_COL",
		                             "ROW_COL", "COL_ROW", "NO")));

	tools::add_arg(args, p, class_name+"p+path",
		cli::File(cli::openmode::read));

	tools::add_arg(args, p, class_name+"p+cols",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+uni",
		cli::None());

	tools::add_arg(args, p, class_name+"p+seed",
		cli::Integer(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+read-order",
		cli::Text(cli::Including_set("TOP_LEFT", "TOP_RIGHT", "BOTTOM_LEFT", "BOTTOM_RIGHT")));

}

void Interleaver_core
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-size"      })) this->size       = vals.to_int ({p+"-size"      });
	if(vals.exist({p+"-type"      })) this->type       = vals.at     ({p+"-type"      });
	if(vals.exist({p+"-path"      })) this->path       = vals.to_file({p+"-path"      });
	if(vals.exist({p+"-cols"      })) this->n_cols     = vals.to_int ({p+"-cols"      });
	if(vals.exist({p+"-seed"      })) this->seed       = vals.to_int ({p+"-seed"      });
	if(vals.exist({p+"-uni"       })) this->uniform    = true;
	if(vals.exist({p+"-read-order"})) this->read_order = vals.at     ({p+"-read-order"});
}

void Interleaver_core
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Size", std::to_string(this->size)));
	if (this->type == "USER")
		headers[p].push_back(std::make_pair("Path", this->path));
	if (this->type == "RAND_COL" || this->type == "ROW_COL" || this->type == "COL_ROW")
		headers[p].push_back(std::make_pair("Number of columns", std::to_string(this->n_cols)));
	if (this->type == "RANDOM" || this->type == "GOLDEN" || this->type == "RAND_COL")
	{
		headers[p].push_back(std::make_pair("Seed", std::to_string(this->seed)));
		headers[p].push_back(std::make_pair("Uniform", (this->uniform ? "yes" : "no")));
	}
}

template <typename T>
tools::Interleaver_core<T>* Interleaver_core
::build() const
{
	if (this->type == "LTE"     ) return new tools::Interleaver_core_LTE          <T>(this->size                                         );
	if (this->type == "CCSDS"   ) return new tools::Interleaver_core_CCSDS        <T>(this->size                                         );
	if (this->type == "DVB-RCS1") return new tools::Interleaver_core_ARP_DVB_RCS1 <T>(this->size                                         );
	if (this->type == "DVB-RCS2") return new tools::Interleaver_core_ARP_DVB_RCS2 <T>(this->size                                         );
	if (this->type == "RANDOM"  ) return new tools::Interleaver_core_random       <T>(this->size,               this->seed, this->uniform);
	if (this->type == "RAND_COL") return new tools::Interleaver_core_random_column<T>(this->size, this->n_cols, this->seed, this->uniform);
	if (this->type == "ROW_COL" ) return new tools::Interleaver_core_row_column   <T>(this->size, this->n_cols, this->read_order         );
	if (this->type == "COL_ROW" ) return new tools::Interleaver_core_column_row   <T>(this->size, this->n_cols, this->read_order         );
	if (this->type == "GOLDEN"  ) return new tools::Interleaver_core_golden       <T>(this->size,               this->seed, this->uniform);
	if (this->type == "USER"    ) return new tools::Interleaver_core_user         <T>(this->size, this->path                             );
	if (this->type == "NO"      ) return new tools::Interleaver_core_NO           <T>(this->size                                         );

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
template aff3ct::tools::Interleaver_core<uint32_t>* aff3ct::factory::Interleaver_core::build<uint32_t>() const;
// ==================================================================================== explicit template instantiation
