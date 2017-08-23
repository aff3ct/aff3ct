#include "Codec.hpp"

using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec::name   = "Codec";
const std::string aff3ct::factory::Codec::prefix = "cdc";

void Codec::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
}

void Codec::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
}

void Codec::make_header(params_list& head_cdc, const parameters& params, const bool full)
{
}
