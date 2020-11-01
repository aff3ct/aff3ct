#include "Tools/Exception/exception.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Interleaver_name   = "Interleaver";
const std::string aff3ct::factory::Interleaver_prefix = "itl";

Interleaver
::Interleaver(const std::string &prefix)
: Factory(Interleaver_name, Interleaver_name, prefix),
  core(new factory::Interleaver_core(prefix))
{
}

Interleaver* Interleaver
::clone() const
{
	return new Interleaver(*this);
}

void Interleaver
::get_description(cli::Argument_map_info &args) const
{
	core->get_description(args);
}

void Interleaver
::store(const cli::Argument_map_value &vals)
{
	core->store(vals);
}

void Interleaver
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	core->get_headers(headers, full);
}

template <typename D>
module::Interleaver<D>* Interleaver
::build(tools::Interleaver_core<>& itl_core)
{
	return new module::Interleaver<D>(itl_core);
}

// ==================================================================================== explicit template instantiation
template aff3ct::module::Interleaver<int8_t >* aff3ct::factory::Interleaver::build<int8_t >(aff3ct::tools::Interleaver_core<>&);
template aff3ct::module::Interleaver<int16_t>* aff3ct::factory::Interleaver::build<int16_t>(aff3ct::tools::Interleaver_core<>&);
template aff3ct::module::Interleaver<int32_t>* aff3ct::factory::Interleaver::build<int32_t>(aff3ct::tools::Interleaver_core<>&);
template aff3ct::module::Interleaver<int64_t>* aff3ct::factory::Interleaver::build<int64_t>(aff3ct::tools::Interleaver_core<>&);
template aff3ct::module::Interleaver<float  >* aff3ct::factory::Interleaver::build<float  >(aff3ct::tools::Interleaver_core<>&);
template aff3ct::module::Interleaver<double >* aff3ct::factory::Interleaver::build<double >(aff3ct::tools::Interleaver_core<>&);
// ==================================================================================== explicit template instantiation
