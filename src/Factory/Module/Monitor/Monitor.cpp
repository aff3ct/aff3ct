#include "Tools/Exception/exception.hpp"

#include "Module/Monitor/Monitor.hpp"

#include "Monitor.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor::name   = "Monitor";
const std::string aff3ct::factory::Monitor::prefix = "mnt";

void Monitor
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
}

void Monitor
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
}

void Monitor
::make_header(params_list& head_mnt, const parameters& params, const bool full)
{
}
