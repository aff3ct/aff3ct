#include "BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER_std::name   = "Simulation BFER standard";
const std::string aff3ct::factory::BFER_std::prefix = "sim";

void BFER_std::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	BFER::build_args(req_args, opt_args, p);
}
