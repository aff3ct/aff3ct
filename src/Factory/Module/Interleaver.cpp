#include "Tools/Exception/exception.hpp"

#include "Module/Interleaver/Random_column/Interleaver_random_column.hpp"
#include "Module/Interleaver/Row_column/Interleaver_row_column.hpp"
#include "Module/Interleaver/LTE/Interleaver_LTE.hpp"
#include "Module/Interleaver/CCSDS/Interleaver_CCSDS.hpp"
#include "Module/Interleaver/NO/Interleaver_NO.hpp"
#include "Module/Interleaver/Golden/Interleaver_golden.hpp"
#include "Module/Interleaver/Random/Interleaver_random.hpp"
#include "Module/Interleaver/User/Interleaver_user.hpp"

#include "Interleaver.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Interleaver::name   = "Interleaver";
const std::string aff3ct::factory::Interleaver::prefix = "itl";

template <typename T>
module::Interleaver<T>* Interleaver
::build(const parameters &params)
{
	     if (params.type == "LTE"     ) return new module::Interleaver_LTE          <T>(params.size,                                             params.n_frames);
	else if (params.type == "CCSDS"   ) return new module::Interleaver_CCSDS        <T>(params.size,                                             params.n_frames);
	else if (params.type == "RANDOM"  ) return new module::Interleaver_random       <T>(params.size,                params.seed, params.uniform, params.n_frames);
	else if (params.type == "RAND_COL") return new module::Interleaver_random_column<T>(params.size, params.n_cols, params.seed, params.uniform, params.n_frames);
	else if (params.type == "ROW_COL" ) return new module::Interleaver_row_column   <T>(params.size, params.n_cols,                              params.n_frames);
	else if (params.type == "GOLDEN"  ) return new module::Interleaver_golden       <T>(params.size,                params.seed, params.uniform, params.n_frames);
	else if (params.type == "USER"    ) return new module::Interleaver_user         <T>(params.size, params.path,                                params.n_frames);
	else if (params.type == "NO"      ) return new module::Interleaver_NO           <T>(params.size,                                             params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Interleaver
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
		 "LTE, CCSDS, RANDOM, GOLDEN, USER, RAND_COL, ROW_COL, NO"};

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

void Interleaver
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

void Interleaver
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
template aff3ct::module::Interleaver<int16_t>* aff3ct::factory::Interleaver::build<int16_t>(const aff3ct::factory::Interleaver::parameters&);
template aff3ct::module::Interleaver<int32_t>* aff3ct::factory::Interleaver::build<int32_t>(const aff3ct::factory::Interleaver::parameters&);
template aff3ct::module::Interleaver<int64_t>* aff3ct::factory::Interleaver::build<int64_t>(const aff3ct::factory::Interleaver::parameters&);
// ==================================================================================== explicit template instantiation
