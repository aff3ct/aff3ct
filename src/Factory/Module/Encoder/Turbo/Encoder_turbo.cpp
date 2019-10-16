#include "Tools/Documentation/documentation.h"
#include "Factory/Module/Encoder/Turbo/Encoder_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_turbo_name   = "Encoder Turbo";
const std::string aff3ct::factory::Encoder_turbo_prefix = "enc";

void Encoder_turbo
::add_args_and_options(cli::Argument_map_info &args, const std::string &p, const std::string &class_name)
{
	cli::add_options(args.at({p+"-type"}), 0, "TURBO");

	tools::add_arg(args, p, class_name+"p+json-path",
		cli::File(cli::openmode::write));
}