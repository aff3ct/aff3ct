#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RA/Encoder_RA.hpp"

#include "Encoder_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_RA_name   = "Encoder RA";
const std::string aff3ct::factory::Encoder_RA_prefix = "enc";

Encoder_RA::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_RA_name, prefix),
  itl(new Interleaver::parameters("itl"))
{
	this->type = "RA";
}

Encoder_RA::parameters* Encoder_RA::parameters
::clone() const
{
	auto clone = new Encoder_RA::parameters(*this);

	if (itl != nullptr) { clone->itl = itl->clone(); }

	return clone;
}

std::vector<std::string> Encoder_RA::parameters
::get_names() const
{
	auto n = Encoder::parameters::get_names();
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}
std::vector<std::string> Encoder_RA::parameters
::get_short_names() const
{
	auto sn = Encoder::parameters::get_short_names();
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Encoder_RA::parameters
::get_prefixes() const
{
	auto p = Encoder::parameters::get_prefixes();
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

Encoder_RA::parameters
::~parameters()
{
	if (itl != nullptr) { delete itl; itl = nullptr; }
}

void Encoder_RA::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Encoder::parameters::get_description(req_args, opt_args);

	this->itl->get_description(req_args, opt_args);

	req_args.erase({"itl-size"    });
	opt_args.erase({"itl-fra", "F"});

	auto p = this->get_prefix();

	tools::add_options(opt_args.at({p+"-type"}), 0, "RA");
}

void Encoder_RA::parameters
::store(const tools::Argument_map_value &vals)
{
	Encoder::parameters::store(vals);

	this->itl->core->size     = this->N_cw;
	this->itl->core->n_frames = this->n_frames;

	this->itl->store(vals);
}

void Encoder_RA::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	this->itl->get_headers(headers, full);
}

template <typename B>
module::Encoder<B>* Encoder_RA::parameters
::build(const module::Interleaver<B> &itl) const
{
	if (this->type == "RA") return new module::Encoder_RA<B>(this->K, this->N_cw, itl, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder<B>* Encoder_RA
::build(const parameters& params, const module::Interleaver<B> &itl)
{
	return params.template build<B>(itl);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_RA::parameters::build<B_8 >(const aff3ct::module::Interleaver<B_8 >&) const;
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_RA::parameters::build<B_16>(const aff3ct::module::Interleaver<B_16>&) const;
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_RA::parameters::build<B_32>(const aff3ct::module::Interleaver<B_32>&) const;
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_RA::parameters::build<B_64>(const aff3ct::module::Interleaver<B_64>&) const;
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_RA::build<B_8 >(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<B_8 >&);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_RA::build<B_16>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<B_16>&);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_RA::build<B_32>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<B_32>&);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_RA::build<B_64>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<B_64>&);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_RA::parameters::build<B>(const aff3ct::module::Interleaver<B>&) const;
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_RA::build<B>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<B>&);
#endif
// ==================================================================================== explicit template instantiation
