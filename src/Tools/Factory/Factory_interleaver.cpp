#include "Tools/Exception/exception.hpp"

#include "Module/Interleaver/Random_column/Interleaver_random_column.hpp"
#include "Module/Interleaver/Row_column/Interleaver_row_column.hpp"
#include "Module/Interleaver/LTE/Interleaver_LTE.hpp"
#include "Module/Interleaver/CCSDS/Interleaver_CCSDS.hpp"
#include "Module/Interleaver/NO/Interleaver_NO.hpp"
#include "Module/Interleaver/Golden/Interleaver_golden.hpp"
#include "Module/Interleaver/Random/Interleaver_random.hpp"
#include "Module/Interleaver/User/Interleaver_user.hpp"

#include "Factory_interleaver.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename T>
Interleaver<T>* Factory_interleaver<T>
::build(const std::string type,
        const int         size,
        const std::string path,
        const bool        uniform,
        const int         n_cols,
        const int         seed,
        const int         n_frames)
{
	     if (type == "LTE"     ) return new Interleaver_LTE          <T>(size,                        n_frames);
	else if (type == "CCSDS"   ) return new Interleaver_CCSDS        <T>(size,                        n_frames);
	else if (type == "RANDOM"  ) return new Interleaver_random       <T>(size,         seed, uniform, n_frames);
	else if (type == "RAND_COL") return new Interleaver_random_column<T>(size, n_cols, seed, uniform, n_frames);
	else if (type == "ROW_COL" ) return new Interleaver_row_column   <T>(size, n_cols,                n_frames);
	else if (type == "GOLDEN"  ) return new Interleaver_golden       <T>(size,         seed, uniform, n_frames);
	else if (type == "USER"    ) return new Interleaver_user         <T>(size, path,                  n_frames);
	else if (type == "NO"      ) return new Interleaver_NO           <T>(size,                        n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_interleaver<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// --------------------------------------------------------------------------------------------------- interleaver
	opt_args[{"itl-type"}] =
		{"string",
		 "specify the type of the interleaver.",
		 "LTE, CCSDS, RANDOM, GOLDEN, USER, RAND_COL, ROW_COL, NO"};

	opt_args[{"itl-path"}] =
		{"string",
		 "specify the path to the interleaver file (to use with \"--itl-type USER\")."};

	opt_args[{"itl-cols"}] =
		{"positive_int",
		 "specify the number of columns used for the RAND_COL or ROW_COL interleaver."};

	opt_args[{"itl-uni"}] =
		{"",
		 "enable the regeneration of the interleaver at each new frame."};
}

template <typename B>
void Factory_interleaver<B>
::store_args(const Arguments_reader& ar, Interleaver_parameters &params, const int seed)
{
	// --------------------------------------------------------------------------------------------------- interleaver
	params.seed = seed;

	if(ar.exist_arg({"itl-type"})) params.type    = ar.get_arg    ({"itl-type"});
	if(ar.exist_arg({"itl-path"})) params.path    = ar.get_arg    ({"itl-path"});
	if(ar.exist_arg({"itl-cols"})) params.n_cols  = ar.get_arg_int({"itl-cols"});
	if(ar.exist_arg({"itl-uni" })) params.uniform = true;

//	if(params.monitor.err_track_revert)
//	{
//		params.monitor.err_track_enable = false;
//		if (params.interleaver.uniform)
//		{
//			params.interleaver.type = "USER";
//			params.interleaver.path = params.monitor.err_track_path + std::string("_$snr.itl");
//		}
//	}
}

template <typename B>
void Factory_interleaver<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"crc", "interleaver parameter(s)"});
}

template <typename B>
void Factory_interleaver<B>
::header(Header::params_list& head_itl, const Interleaver_parameters& params)
{
	// --------------------------------------------------------------------------------------------------- interleaver
	head_itl.push_back(std::make_pair("Type", params.type));

	if (params.type == "USER")
		head_itl.push_back(std::make_pair("Path", params.path));

	if (params.type == "RAND_COL" || params.type == "ROW_COL")
		head_itl.push_back(std::make_pair("Number of columns", std::to_string(params.n_cols)));

	head_itl.push_back(std::make_pair("Uniform", (params.uniform ? "on" : "off")));
}

//==================================================================================== explicit template instantiation
template struct aff3ct::tools::Factory_interleaver<short>;
template struct aff3ct::tools::Factory_interleaver<int>;
template struct aff3ct::tools::Factory_interleaver<long long>;
// ==================================================================================== explicit template instantiation
