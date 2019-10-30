#include "Tools/Documentation/documentation.h"
#include "Factory/Module/Decoder/Turbo/Decoder_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_turbo_name   = "Decoder Turbo";
const std::string aff3ct::factory::Decoder_turbo_prefix = "dec";

void Decoder_turbo_common
::add_args_and_options(cli::Argument_map_info &args, const std::string &p, const std::string &class_name)
{
	cli::add_options(args.at({p+"-type", "D"}), 0, "TURBO"      );
	cli::add_options(args.at({p+"-implem"   }), 0, "STD", "FAST");

	tools::add_arg(args, p, class_name+"p+ite,i",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+sc",
		cli::None());

	tools::add_arg(args, p, class_name+"p+json",
		cli::None());

	tools::add_arg(args, p, class_name+"p+crc-start",
		cli::Integer(cli::Positive(), cli::Non_zero()));
}