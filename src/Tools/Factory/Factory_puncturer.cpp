#include <Tools/Factory/Factory_puncturer.hpp>

using namespace aff3ct;
using namespace tools;

void Factory_puncturer::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ----------------------------------------------------------------------------------------------------- poncturer
}

void Factory_puncturer::store_args(const Arguments_reader& ar, puncturer_parameters &params)
{
	// ----------------------------------------------------------------------------------------------------- poncturer
}

void Factory_puncturer::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"pct","Puncturer parameter(s)"});
}

void Factory_puncturer::header(Header::params_list& head_pct, const puncturer_parameters& params)
{
	// ----------------------------------------------------------------------------------------------------- poncturer
}
