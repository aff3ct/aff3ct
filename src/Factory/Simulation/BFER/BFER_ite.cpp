#include "BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER_ite::name   = "Simulation BFER iterative";
const std::string aff3ct::factory::BFER_ite::prefix = "sim";

void BFER_ite::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	BFER::build_args(req_args, opt_args, p);

	opt_args[{p+"-ite", "I"}] =
		{"positive_int",
		 "number of global iterations between the demodulator and the decoder."};

	opt_args[{p+"-crc-start"}] =
		{"positive_int",
		 "iteration number to start the CRC checking in the turbo demodulation process."};
}
