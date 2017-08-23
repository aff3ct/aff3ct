#include "BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER::name   = "Simulation BFER";
const std::string aff3ct::factory::BFER::prefix = "sim";

void BFER::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Simulation::build_args(req_args, opt_args, p);

	opt_args[{p+"-debug", "d"}] =
		{"",
		 "enable debug mode: print array values after each step."};

	opt_args[{p+"-debug-prec"}] =
		{"positive_int",
		 "set the precision of real elements when displayed in debug mode."};

	opt_args[{p+"-debug-limit"}] =
		{"positive_int",
		 "set the max number of elements to display in the debug mode."};

	opt_args[{p+"-stats"}] =
		{"",
		 "display statistics module by module."};

	opt_args[{p+"-snr-type", "E"}] =
		{"string",
		 "select the type of SNR: symbol energy or information bit energy.",
		 "ES, EB"};

	opt_args[{p+"-coset", "c"}] =
		{"",
		 "enable the coset approach."};

	opt_args[{p+"-err-trk"}] =
		{"",
		 "enable the tracking of the bad frames (by default the frames are stored in the current folder)."};

	opt_args[{p+"-err-trk-rev"}] =
		{"",
		 "automatically replay the saved frames."};

	opt_args[{p+"-err-trk-path"}] =
		{"string",
		 "base path for the files where the bad frames will be stored or read."};

	opt_args[{p+"-coded"}] =
		{"",
		 "enable the coded monitoring (extends the monitored bits to the entire codeword)."};
}
