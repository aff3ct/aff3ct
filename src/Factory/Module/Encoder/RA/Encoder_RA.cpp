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
	return new Encoder_RA::parameters(*this);
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

void Encoder_RA::parameters
::get_description(tools::Argument_map_info &args) const
{
	Encoder::parameters::get_description(args);

	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}

	auto p = this->get_prefix();

	tools::add_options(args.at({p+"-type"}), 0, "RA");
}

void Encoder_RA::parameters
::store(const tools::Argument_map_value &vals)
{
	Encoder::parameters::store(vals);

	if (itl != nullptr)
	{
		this->itl->core->size     = this->N_cw;
		this->itl->core->n_frames = this->n_frames;

		this->itl->store(vals);
	}
}

void Encoder_RA::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	if (itl != nullptr)
		itl->get_headers(headers, full);
}

template <typename B>
module::Encoder_RA<B>* Encoder_RA::parameters
::build(const module::Interleaver<B> &itl) const
{
	if (this->type == "RA") return new module::Encoder_RA<B>(this->K, this->N_cw, itl, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_RA<B>* Encoder_RA
::build(const parameters& params, const module::Interleaver<B> &itl)
{
	return params.template build<B>(itl);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_RA<B_8 >* aff3ct::factory::Encoder_RA::parameters::build<B_8 >(const aff3ct::module::Interleaver<B_8 >&) const;
template aff3ct::module::Encoder_RA<B_16>* aff3ct::factory::Encoder_RA::parameters::build<B_16>(const aff3ct::module::Interleaver<B_16>&) const;
template aff3ct::module::Encoder_RA<B_32>* aff3ct::factory::Encoder_RA::parameters::build<B_32>(const aff3ct::module::Interleaver<B_32>&) const;
template aff3ct::module::Encoder_RA<B_64>* aff3ct::factory::Encoder_RA::parameters::build<B_64>(const aff3ct::module::Interleaver<B_64>&) const;
template aff3ct::module::Encoder_RA<B_8 >* aff3ct::factory::Encoder_RA::build<B_8 >(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<B_8 >&);
template aff3ct::module::Encoder_RA<B_16>* aff3ct::factory::Encoder_RA::build<B_16>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<B_16>&);
template aff3ct::module::Encoder_RA<B_32>* aff3ct::factory::Encoder_RA::build<B_32>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<B_32>&);
template aff3ct::module::Encoder_RA<B_64>* aff3ct::factory::Encoder_RA::build<B_64>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<B_64>&);
#else
template aff3ct::module::Encoder_RA<B>* aff3ct::factory::Encoder_RA::parameters::build<B>(const aff3ct::module::Interleaver<B>&) const;
template aff3ct::module::Encoder_RA<B>* aff3ct::factory::Encoder_RA::build<B>(const aff3ct::factory::Encoder_RA::parameters&, const aff3ct::module::Interleaver<B>&);
#endif
// ==================================================================================== explicit template instantiation
