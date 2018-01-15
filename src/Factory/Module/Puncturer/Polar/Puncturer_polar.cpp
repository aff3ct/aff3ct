#include <cmath>

#include "Tools/Exception/exception.hpp"

#include "Puncturer_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_polar_name   = "Puncturer Polar";
const std::string aff3ct::factory::Puncturer_polar_prefix = "pct";

Puncturer_polar::parameters
::parameters(const std::string &prefix)
: Puncturer::parameters(Puncturer_polar_name, prefix)
{
	this->type = "WANGLIU";
}

Puncturer_polar::parameters
::~parameters()
{
}

Puncturer_polar::parameters* Puncturer_polar::parameters
::clone() const
{
	return new Puncturer_polar::parameters(*this);
}

void Puncturer_polar::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Puncturer::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	tools::add_options(opt_args.at({p+"-type"}), 0, "WANGLIU");
}

void Puncturer_polar::parameters
::store(const tools::Argument_map_value &vals)
{
	Puncturer::parameters::store(vals);
	this->N_cw = (int)std::exp2((int)std::ceil(std::log2(this->N)));

	if (this->N == this->N_cw)
		this->type = "NO";
}

void Puncturer_polar::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Puncturer::parameters::get_headers(headers, full);
}

template <typename B, typename Q>
module::Puncturer_polar_wangliu<B,Q>* Puncturer_polar::parameters
::build(const tools::Frozenbits_generator &fb_generator) const
{
	if (this->type == "WANGLIU") return new module::Puncturer_polar_wangliu<B,Q>(this->K, this->N, fb_generator, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Puncturer_polar_wangliu<B,Q>* Puncturer_polar
::build(const parameters                  &params,
        const tools::Frozenbits_generator &fb_generator)
{
	return params.template build<B,Q>(fb_generator);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer_polar_wangliu<B_8 ,Q_8 >* aff3ct::factory::Puncturer_polar::parameters::build<B_8 ,Q_8 >(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_wangliu<B_16,Q_16>* aff3ct::factory::Puncturer_polar::parameters::build<B_16,Q_16>(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_wangliu<B_32,Q_32>* aff3ct::factory::Puncturer_polar::parameters::build<B_32,Q_32>(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_wangliu<B_64,Q_64>* aff3ct::factory::Puncturer_polar::parameters::build<B_64,Q_64>(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_wangliu<B_8 ,Q_8 >* aff3ct::factory::Puncturer_polar::build<B_8 ,Q_8 >(const aff3ct::factory::Puncturer_polar::parameters&, const aff3ct::tools::Frozenbits_generator&);
template aff3ct::module::Puncturer_polar_wangliu<B_16,Q_16>* aff3ct::factory::Puncturer_polar::build<B_16,Q_16>(const aff3ct::factory::Puncturer_polar::parameters&, const aff3ct::tools::Frozenbits_generator&);
template aff3ct::module::Puncturer_polar_wangliu<B_32,Q_32>* aff3ct::factory::Puncturer_polar::build<B_32,Q_32>(const aff3ct::factory::Puncturer_polar::parameters&, const aff3ct::tools::Frozenbits_generator&);
template aff3ct::module::Puncturer_polar_wangliu<B_64,Q_64>* aff3ct::factory::Puncturer_polar::build<B_64,Q_64>(const aff3ct::factory::Puncturer_polar::parameters&, const aff3ct::tools::Frozenbits_generator&);
#else
template aff3ct::module::Puncturer_polar_wangliu<B,Q>* aff3ct::factory::Puncturer_polar::parameters::build<B,Q>(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_wangliu<B,Q>* aff3ct::factory::Puncturer_polar::build<B,Q>(const aff3ct::factory::Puncturer_polar::parameters&, const aff3ct::tools::Frozenbits_generator&);
#endif
// ==================================================================================== explicit template instantiation
