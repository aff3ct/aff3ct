#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Encoder/Turbo_product/Encoder_turbo_product.hpp"
#include "Factory/Module/Encoder/Turbo_product/Encoder_turbo_product.hpp"

const std::string aff3ct::factory::Encoder_turbo_product_name   = "Encoder Turbo Product Code";
const std::string aff3ct::factory::Encoder_turbo_product_prefix = "enc";

using namespace aff3ct;
using namespace aff3ct::factory;

Encoder_turbo_product
::Encoder_turbo_product(const std::string &prefix)
: Encoder(Encoder_turbo_product_name, prefix),
  sub(new Encoder_BCH(prefix+"-sub")),
  itl(new Interleaver("itl"))
{
	this->type = "TPC";
}

Encoder_turbo_product* Encoder_turbo_product
::clone() const
{
	return new Encoder_turbo_product(*this);
}

std::vector<std::string> Encoder_turbo_product
::get_names() const
{
	auto n = Encoder::get_names();
	if (sub != nullptr) { auto nn = sub->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> Encoder_turbo_product
::get_short_names() const
{
	auto sn = Encoder::get_short_names();
	if (sub != nullptr) { auto nn = sub->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Encoder_turbo_product
::get_prefixes() const
{
	auto p = Encoder::get_prefixes();
	if (sub != nullptr) { auto nn = sub->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void Encoder_turbo_product
::get_description(cli::Argument_map_info &args) const
{
	Encoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_turbo_product::";

	args.erase({p+"-info-bits", "K"});
	args.erase({p+"-cw-size",   "N"});

	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = this->itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}

	cli::add_options(args.at({p+"-type"}), 0, "TPC");

	tools::add_arg(args, p, class_name+"p+ext",
		cli::None());

	sub->get_description(args);

	auto ps = sub->get_prefix();

	args.erase({ps+"-fra",  "F"});
	args.erase({ps+"-seed", "S"});
}

void Encoder_turbo_product
::store(const cli::Argument_map_value &vals)
{
	Encoder::store(vals);

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

void Encoder_turbo_product
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::get_headers(headers, full);

	if (itl != nullptr)
		itl->get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Parity extended", (this->parity_extended ? "yes" : "no")));

	sub->get_headers(headers, full);
}

template <typename B>
module::Encoder_turbo_product<B>* Encoder_turbo_product
::build(const module::Interleaver<B> &itl,
              module::Encoder_BCH<B> &enc_r,
              module::Encoder_BCH<B> &enc_c) const
{
	if (this->type == "TPC") return new module::Encoder_turbo_product<B>(itl, enc_r, enc_c, n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_turbo_product<B_8 >* aff3ct::factory::Encoder_turbo_product::build<B_8 >(const aff3ct::module::Interleaver<B_8 >&, aff3ct::module::Encoder_BCH<B_8 >&, aff3ct::module::Encoder_BCH<B_8 >&) const;
template aff3ct::module::Encoder_turbo_product<B_16>* aff3ct::factory::Encoder_turbo_product::build<B_16>(const aff3ct::module::Interleaver<B_16>&, aff3ct::module::Encoder_BCH<B_16>&, aff3ct::module::Encoder_BCH<B_16>&) const;
template aff3ct::module::Encoder_turbo_product<B_32>* aff3ct::factory::Encoder_turbo_product::build<B_32>(const aff3ct::module::Interleaver<B_32>&, aff3ct::module::Encoder_BCH<B_32>&, aff3ct::module::Encoder_BCH<B_32>&) const;
template aff3ct::module::Encoder_turbo_product<B_64>* aff3ct::factory::Encoder_turbo_product::build<B_64>(const aff3ct::module::Interleaver<B_64>&, aff3ct::module::Encoder_BCH<B_64>&, aff3ct::module::Encoder_BCH<B_64>&) const;
#else
template aff3ct::module::Encoder_turbo_product<B>* aff3ct::factory::Encoder_turbo_product::build<B>(const aff3ct::module::Interleaver<B>&, aff3ct::module::Encoder_BCH<B>&, aff3ct::module::Encoder_BCH<B>&) const;
#endif
// ==================================================================================== explicit template instantiation
