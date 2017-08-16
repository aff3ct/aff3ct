#include "Tools/Exception/exception.hpp"

#include "Tools/Interleaver/Random_column/Interleaver_core_random_column.hpp"
#include "Tools/Interleaver/Row_column/Interleaver_core_row_column.hpp"
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

template <typename T>
tools::Interleaver_core<T>* Interleaver_core
::build(const parameters &params)
{
	     if (params.type == "LTE"     ) return new tools::Interleaver_core_LTE          <T>(params.size,                                             params.n_frames);
	else if (params.type == "CCSDS"   ) return new tools::Interleaver_core_CCSDS        <T>(params.size,                                             params.n_frames);
	else if (params.type == "DVB-RCS1") return new tools::Interleaver_core_ARP_DVB_RCS1 <T>(params.size,                                             params.n_frames);
	else if (params.type == "DVB-RCS2") return new tools::Interleaver_core_ARP_DVB_RCS2 <T>(params.size,                                             params.n_frames);
	else if (params.type == "RANDOM"  ) return new tools::Interleaver_core_random       <T>(params.size,                params.seed, params.uniform, params.n_frames);
	else if (params.type == "RAND_COL") return new tools::Interleaver_core_random_column<T>(params.size, params.n_cols, params.seed, params.uniform, params.n_frames);
	else if (params.type == "ROW_COL" ) return new tools::Interleaver_core_row_column   <T>(params.size, params.n_cols,                              params.n_frames);
	else if (params.type == "GOLDEN"  ) return new tools::Interleaver_core_golden       <T>(params.size,                params.seed, params.uniform, params.n_frames);
	else if (params.type == "USER"    ) return new tools::Interleaver_core_user         <T>(params.size, params.path,                                params.n_frames);
	else if (params.type == "NO"      ) return new tools::Interleaver_core_NO           <T>(params.size,                                             params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Interleaver_core
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	req_args[{p+"-size"}] =
		{"positive_int",
		 "number of symbols to interleave."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-type"}] =
		{"string",
		 "specify the type of the interleaver.",
		 "LTE, CCSDS, DVB-RCS1, DVB-RCS2, RANDOM, GOLDEN, USER, RAND_COL, ROW_COL, NO"};

	opt_args[{p+"-path"}] =
		{"string",
		 "specify the path to the interleaver file (to use with \"--itl-type USER\")."};

	opt_args[{p+"-cols"}] =
		{"positive_int",
		 "specify the number of columns used for the RAND_COL or ROW_COL interleaver."};

	opt_args[{p+"-uni"}] =
		{"",
		 "enable the regeneration of the interleaver at each new frame."};

	opt_args[{p+"-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};
}

void Interleaver_core
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	if(exist(vals, {p+"-size"     })) params.size     = std::stoi(vals.at({p+"-size"     }));
	if(exist(vals, {p+"-fra",  "F"})) params.n_frames = std::stoi(vals.at({p+"-fra",  "F"}));
	if(exist(vals, {p+"-type"     })) params.type     =           vals.at({p+"-type"     });
	if(exist(vals, {p+"-path"     })) params.path     =           vals.at({p+"-path"     });
	if(exist(vals, {p+"-cols"     })) params.n_cols   = std::stoi(vals.at({p+"-cols"     }));
	if(exist(vals, {p+"-seed", "S"})) params.seed     = std::stoi(vals.at({p+"-seed", "S"}));
	if(exist(vals, {p+"-uni"      })) params.uniform  = true;
}

void Interleaver_core
::make_header(params_list& head_itl, const parameters& params, const bool full)
{
	head_itl.push_back(std::make_pair("Type", params.type));
	if (full) head_itl.push_back(std::make_pair("Size", std::to_string(params.size)));
	if (full) head_itl.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
	if (params.type == "USER")
		head_itl.push_back(std::make_pair("Path", params.path));
	if (params.type == "RAND_COL" || params.type == "ROW_COL")
		head_itl.push_back(std::make_pair("Number of columns", std::to_string(params.n_cols)));
	if (params.type == "RANDOM" || params.type == "GOLDEN" || params.type == "RAND_COL")
	{
		if (full) head_itl.push_back(std::make_pair("Seed", std::to_string(params.seed)));
		head_itl.push_back(std::make_pair("Uniform", (params.uniform ? "yes" : "no")));
	}
}

// ==================================================================================== explicit template instantiation 
template aff3ct::tools::Interleaver_core<uint32_t>* aff3ct::factory::Interleaver_core::build<uint32_t>(const aff3ct::factory::Interleaver_core::parameters&);
// ==================================================================================== explicit template instantiation
