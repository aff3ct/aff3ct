#include "Tools/Exception/exception.hpp"

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

#include "Interleaver_core.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Interleaver_core::name   = "Interleaver";
const std::string aff3ct::factory::Interleaver_core::prefix = "itl";

Interleaver_core::parameters
::parameters(const std::string prefix)
: Factory::parameters(Interleaver_core::name, Interleaver_core::name, prefix)
{
}

Interleaver_core::parameters
::~parameters()
{
}

Interleaver_core::parameters* Interleaver_core::parameters
::clone() const
{
	return new Interleaver_core::parameters(*this);
}

void Interleaver_core::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	auto p = this->get_prefix();

	req_args[{p+"-size"}] =
		{"positive_int",
		 "number of symbols to interleave."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-type"}] =
		{"string",
		 "specify the type of the interleaver.",
		 "LTE, CCSDS, DVB-RCS1, DVB-RCS2, RANDOM, GOLDEN, USER, RAND_COL, ROW_COL, COL_ROW, NO"};

	opt_args[{p+"-path"}] =
		{"string",
		 "specify the path to the interleaver file (to use with \"--itl-type USER\")."};

	opt_args[{p+"-cols"}] =
		{"positive_int",
		 "specify the number of columns used for the RAND_COL, ROW_COL or COL_ROW interleaver."};

	opt_args[{p+"-uni"}] =
		{"",
		 "enable the regeneration of the interleaver at each new frame."};

	opt_args[{p+"-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};
}

void Interleaver_core::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-size"     })) this->size     = std::stoi(vals.at({p+"-size"     }));
	if(exist(vals, {p+"-fra",  "F"})) this->n_frames = std::stoi(vals.at({p+"-fra",  "F"}));
	if(exist(vals, {p+"-type"     })) this->type     =           vals.at({p+"-type"     });
	if(exist(vals, {p+"-path"     })) this->path     =           vals.at({p+"-path"     });
	if(exist(vals, {p+"-cols"     })) this->n_cols   = std::stoi(vals.at({p+"-cols"     }));
	if(exist(vals, {p+"-seed", "S"})) this->seed     = std::stoi(vals.at({p+"-seed", "S"}));
	if(exist(vals, {p+"-uni"      })) this->uniform  = true;
}

void Interleaver_core::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Size", std::to_string(this->size)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
	if (this->type == "USER")
		headers[p].push_back(std::make_pair("Path", this->path));
	if (this->type == "RAND_COL" || this->type == "ROW_COL" || this->type == "COL_ROW")
		headers[p].push_back(std::make_pair("Number of columns", std::to_string(this->n_cols)));
	if (this->type == "RANDOM" || this->type == "GOLDEN" || this->type == "RAND_COL")
	{
		if (full) headers[p].push_back(std::make_pair("Seed", std::to_string(this->seed)));
		headers[p].push_back(std::make_pair("Uniform", (this->uniform ? "yes" : "no")));
	}
}

template <typename T>
tools::Interleaver_core<T>* Interleaver_core::parameters
::build() const
{
	     if (this->type == "LTE"     ) return new tools::Interleaver_core_LTE          <T>(this->size,                                          this->n_frames);
	else if (this->type == "CCSDS"   ) return new tools::Interleaver_core_CCSDS        <T>(this->size,                                          this->n_frames);
	else if (this->type == "DVB-RCS1") return new tools::Interleaver_core_ARP_DVB_RCS1 <T>(this->size,                                          this->n_frames);
	else if (this->type == "DVB-RCS2") return new tools::Interleaver_core_ARP_DVB_RCS2 <T>(this->size,                                          this->n_frames);
	else if (this->type == "RANDOM"  ) return new tools::Interleaver_core_random       <T>(this->size,               this->seed, this->uniform, this->n_frames);
	else if (this->type == "RAND_COL") return new tools::Interleaver_core_random_column<T>(this->size, this->n_cols, this->seed, this->uniform, this->n_frames);
	else if (this->type == "ROW_COL" ) return new tools::Interleaver_core_row_column   <T>(this->size, this->n_cols,                            this->n_frames);
	else if (this->type == "COL_ROW" ) return new tools::Interleaver_core_column_row   <T>(this->size, this->n_cols,                            this->n_frames);
	else if (this->type == "GOLDEN"  ) return new tools::Interleaver_core_golden       <T>(this->size,               this->seed, this->uniform, this->n_frames);
	else if (this->type == "USER"    ) return new tools::Interleaver_core_user         <T>(this->size, this->path,                              this->n_frames);
	else if (this->type == "NO"      ) return new tools::Interleaver_core_NO           <T>(this->size,                                          this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename T>
tools::Interleaver_core<T>* Interleaver_core
::build(const parameters &params)
{
	return params.template build<T>();
}

// ==================================================================================== explicit template instantiation 
template aff3ct::tools::Interleaver_core<uint32_t>* aff3ct::factory::Interleaver_core::parameters::build<uint32_t>() const;
template aff3ct::tools::Interleaver_core<uint32_t>* aff3ct::factory::Interleaver_core::build<uint32_t>(const aff3ct::factory::Interleaver_core::parameters&);
// ==================================================================================== explicit template instantiation
