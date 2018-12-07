#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Turbo_product/Encoder_turbo_product.hpp"

#include "Encoder_turbo_product.hpp"

const std::string aff3ct::factory::Encoder_turbo_product_name   = "Encoder Turbo Product Code";
const std::string aff3ct::factory::Encoder_turbo_product_prefix = "enc";

using namespace aff3ct;
using namespace aff3ct::factory;

Encoder_turbo_product::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_turbo_product_name, prefix),
  sub(new Encoder_BCH::parameters(prefix+"-sub")),
  itl(new Interleaver::parameters("itl"))
{
	this->type = "TPC";
}

Encoder_turbo_product::parameters* Encoder_turbo_product::parameters
::clone() const
{
	return new Encoder_turbo_product::parameters(*this);
}

std::vector<std::string> Encoder_turbo_product::parameters
::get_names() const
{
	auto n = Encoder::parameters::get_names();
	if (sub != nullptr) { auto nn = sub->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> Encoder_turbo_product::parameters
::get_short_names() const
{
	auto sn = Encoder::parameters::get_short_names();
	if (sub != nullptr) { auto nn = sub->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Encoder_turbo_product::parameters
::get_prefixes() const
{
	auto p = Encoder::parameters::get_prefixes();
	if (sub != nullptr) { auto nn = sub->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void Encoder_turbo_product::parameters
::get_description(tools::Argument_map_info &args) const
{
	Encoder::parameters::get_description(args);

	auto p = this->get_prefix();

	args.erase({p+"-info-bits", "K"});
	args.erase({p+"-cw-size",   "N"});

	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = this->itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}

	tools::add_options(args.at({p+"-type"}), 0, "TPC");

	args.add(
		{p+"-ext"},
		tools::None(),
		"extends code with a parity bits.");


	sub->get_description(args);

	auto ps = sub->get_prefix();

	args.erase({ps+"-fra", "F"});
}

void Encoder_turbo_product::parameters
::store(const tools::Argument_map_value &vals)
{
	Encoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-ext"})) this->parity_extended = true;

	// this->sub->n_frames = this->n_frames;

	sub->store(vals);

	this->K = this->sub->K * this->sub->K;

	this->R = (float)this->K / (float)this->N_cw;


	if (itl != nullptr)
	{
		this->itl->core->n_frames = this->n_frames;
		this->itl->core->type     = "ROW_COL";

		if (parity_extended)
			this->itl->core->n_cols = this->sub->N_cw +1;
		else
			this->itl->core->n_cols = this->sub->N_cw;

		this->itl->core->size = this->itl->core->n_cols * this->itl->core->n_cols;
		this->N_cw = this->itl->core->size;

		itl->store(vals);
	}
}

void Encoder_turbo_product::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	if (itl != nullptr)
		itl->get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Parity extended", (this->parity_extended ? "yes" : "no")));

	sub->get_headers(headers, full);
}

template <typename B>
module::Encoder_turbo_product<B>* Encoder_turbo_product::parameters
::build(const module::Interleaver<B> &itl,
              module::Encoder_BCH<B> &enc_r,
              module::Encoder_BCH<B> &enc_c) const
{
	if (this->type == "TPC") return new module::Encoder_turbo_product<B>(itl, enc_r, enc_c, n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_turbo_product<B>* Encoder_turbo_product
::build(const parameters              &params,
        const module::Interleaver<B> &itl,
              module::Encoder_BCH<B> &enc_r,
              module::Encoder_BCH<B> &enc_c)
{
	return params.template build<B>(itl, enc_r, enc_c);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_turbo_product<B_8 >* aff3ct::factory::Encoder_turbo_product::parameters::build<B_8 >(const aff3ct::module::Interleaver<B_8 >&, aff3ct::module::Encoder_BCH<B_8 >&, aff3ct::module::Encoder_BCH<B_8 >&) const;
template aff3ct::module::Encoder_turbo_product<B_16>* aff3ct::factory::Encoder_turbo_product::parameters::build<B_16>(const aff3ct::module::Interleaver<B_16>&, aff3ct::module::Encoder_BCH<B_16>&, aff3ct::module::Encoder_BCH<B_16>&) const;
template aff3ct::module::Encoder_turbo_product<B_32>* aff3ct::factory::Encoder_turbo_product::parameters::build<B_32>(const aff3ct::module::Interleaver<B_32>&, aff3ct::module::Encoder_BCH<B_32>&, aff3ct::module::Encoder_BCH<B_32>&) const;
template aff3ct::module::Encoder_turbo_product<B_64>* aff3ct::factory::Encoder_turbo_product::parameters::build<B_64>(const aff3ct::module::Interleaver<B_64>&, aff3ct::module::Encoder_BCH<B_64>&, aff3ct::module::Encoder_BCH<B_64>&) const;
template aff3ct::module::Encoder_turbo_product<B_8 >* aff3ct::factory::Encoder_turbo_product::build<B_8 >(const aff3ct::factory::Encoder_turbo_product::parameters&, const aff3ct::module::Interleaver<B_8 >&, aff3ct::module::Encoder_BCH<B_8 >&, aff3ct::module::Encoder_BCH<B_8 >&);
template aff3ct::module::Encoder_turbo_product<B_16>* aff3ct::factory::Encoder_turbo_product::build<B_16>(const aff3ct::factory::Encoder_turbo_product::parameters&, const aff3ct::module::Interleaver<B_16>&, aff3ct::module::Encoder_BCH<B_16>&, aff3ct::module::Encoder_BCH<B_16>&);
template aff3ct::module::Encoder_turbo_product<B_32>* aff3ct::factory::Encoder_turbo_product::build<B_32>(const aff3ct::factory::Encoder_turbo_product::parameters&, const aff3ct::module::Interleaver<B_32>&, aff3ct::module::Encoder_BCH<B_32>&, aff3ct::module::Encoder_BCH<B_32>&);
template aff3ct::module::Encoder_turbo_product<B_64>* aff3ct::factory::Encoder_turbo_product::build<B_64>(const aff3ct::factory::Encoder_turbo_product::parameters&, const aff3ct::module::Interleaver<B_64>&, aff3ct::module::Encoder_BCH<B_64>&, aff3ct::module::Encoder_BCH<B_64>&);
#else
template aff3ct::module::Encoder_turbo_product<B>* aff3ct::factory::Encoder_turbo_product::parameters::build<B>(const aff3ct::module::Interleaver<B>&, aff3ct::module::Encoder_BCH<B>&, aff3ct::module::Encoder_BCH<B>&) const;
template aff3ct::module::Encoder_turbo_product<B>* aff3ct::factory::Encoder_turbo_product::build<B>(const aff3ct::factory::Encoder_turbo_product::parameters&, const aff3ct::module::Interleaver<B>&, aff3ct::module::Encoder_BCH<B>&, aff3ct::module::Encoder_BCH<B>&);
#endif
// ==================================================================================== explicit template instantiation
