#include "Tools/Exception/exception.hpp"

#include "Interleaver.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Interleaver_name   = "Interleaver";
const std::string aff3ct::factory::Interleaver_prefix = "itl";

Interleaver::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Interleaver_name, Interleaver_name, prefix),
  core(new factory::Interleaver_core::parameters(prefix))
{
}

Interleaver::parameters* Interleaver::parameters
::clone() const
{
	return new Interleaver::parameters(*this);
}

void Interleaver::parameters
::get_description(tools::Argument_map_info &args) const
{
	core->get_description(args);
}

void Interleaver::parameters
::store(const tools::Argument_map_value &vals)
{
	core->store(vals);
}

void Interleaver::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	core->get_headers(headers, full);
}

template <typename D>
module::Interleaver<D>* Interleaver::parameters
::build(const tools::Interleaver_core<>& itl_core) const
{
	return new module::Interleaver<D>(itl_core);
}

template <typename D>
module::Interleaver<D>* Interleaver
::build(const tools::Interleaver_core<>& itl_core)
{
	return new module::Interleaver<D>(itl_core);
}

// ==================================================================================== explicit template instantiation
template aff3ct::module::Interleaver<int8_t >* aff3ct::factory::Interleaver::parameters::build<int8_t >(const aff3ct::tools::Interleaver_core<>&) const;
template aff3ct::module::Interleaver<int16_t>* aff3ct::factory::Interleaver::parameters::build<int16_t>(const aff3ct::tools::Interleaver_core<>&) const;
template aff3ct::module::Interleaver<int32_t>* aff3ct::factory::Interleaver::parameters::build<int32_t>(const aff3ct::tools::Interleaver_core<>&) const;
template aff3ct::module::Interleaver<int64_t>* aff3ct::factory::Interleaver::parameters::build<int64_t>(const aff3ct::tools::Interleaver_core<>&) const;
template aff3ct::module::Interleaver<float  >* aff3ct::factory::Interleaver::parameters::build<float  >(const aff3ct::tools::Interleaver_core<>&) const;
template aff3ct::module::Interleaver<double >* aff3ct::factory::Interleaver::parameters::build<double >(const aff3ct::tools::Interleaver_core<>&) const;
template aff3ct::module::Interleaver<int8_t >* aff3ct::factory::Interleaver::build<int8_t >(const aff3ct::tools::Interleaver_core<>&);
template aff3ct::module::Interleaver<int16_t>* aff3ct::factory::Interleaver::build<int16_t>(const aff3ct::tools::Interleaver_core<>&);
template aff3ct::module::Interleaver<int32_t>* aff3ct::factory::Interleaver::build<int32_t>(const aff3ct::tools::Interleaver_core<>&);
template aff3ct::module::Interleaver<int64_t>* aff3ct::factory::Interleaver::build<int64_t>(const aff3ct::tools::Interleaver_core<>&);
template aff3ct::module::Interleaver<float  >* aff3ct::factory::Interleaver::build<float  >(const aff3ct::tools::Interleaver_core<>&);
template aff3ct::module::Interleaver<double >* aff3ct::factory::Interleaver::build<double >(const aff3ct::tools::Interleaver_core<>&);
// ==================================================================================== explicit template instantiation
