#include "EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::EXIT::name   = "Simulation EXIT";
const std::string aff3ct::factory::EXIT::prefix = "sim";

void EXIT::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Simulation::build_args(req_args, opt_args, p);

	req_args[{p+"-siga-min", "a"}] =
		{"positive_float",
		 "sigma min value used in EXIT charts."};

	req_args[{p+"-siga-max", "A"}] =
		{"positive_float",
		 "sigma max value used in EXIT charts."};

	opt_args[{p+"-siga-step"}] =
		{"positive_float",
		 "sigma step value used in EXIT charts."};

	opt_args[{p+"-trials"}] =
		{"positive_int",
		 "number of frames to simulate per sigma A value."};

	opt_args[{p+"-inter-lvl"}].push_back("1");
}
