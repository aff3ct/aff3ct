#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Turbo_product_code/Encoder_turbo_product_code.hpp"

#include "Encoder_turbo_product_code.hpp"

const std::string aff3ct::factory::Encoder_turbo_product_code_name   = "Encoder Turbo Product Code";
const std::string aff3ct::factory::Encoder_turbo_product_code_prefix = "enc";

using namespace aff3ct;
using namespace aff3ct::factory;

Encoder_turbo_product_code::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_turbo_product_code_name, prefix),
  sub(new Encoder_BCH::parameters(prefix+"-sub")),
  itl(new Interleaver::parameters("itl"))
{
	this->type = "TPC";
}

Encoder_turbo_product_code::parameters* Encoder_turbo_product_code::parameters
::clone() const
{
	auto clone = new Encoder_turbo_product_code::parameters(*this);

	if (sub != nullptr) { clone->sub = sub->clone(); }
	if (itl != nullptr) { clone->itl = itl->clone(); }

	return clone;
}

std::vector<std::string> Encoder_turbo_product_code::parameters
::get_names() const
{
	auto n = Encoder::parameters::get_names();
	if (sub != nullptr) { auto nn = sub->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> Encoder_turbo_product_code::parameters
::get_short_names() const
{
	auto sn = Encoder::parameters::get_short_names();
	if (sub != nullptr) { auto nn = sub->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Encoder_turbo_product_code::parameters
::get_prefixes() const
{
	auto p = Encoder::parameters::get_prefixes();
	if (sub != nullptr) { auto nn = sub->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

Encoder_turbo_product_code::parameters
::~parameters()
{
	if (sub != nullptr) { delete sub; sub = nullptr; }
	if (itl != nullptr) { delete itl; itl = nullptr; }
}

void Encoder_turbo_product_code::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Encoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.erase({p+"-info-bits", "K"});
	req_args.erase({p+"-cw-size",   "N"});

	itl->get_description(req_args, opt_args);

	auto pi = this->itl->get_prefix();

	req_args.erase({pi+"-size"    });
	opt_args.erase({pi+"-fra", "F"});

	opt_args[{p+"-type"}][2] += ", TPC";

	opt_args[{p+"-ext"}] =
		{"",
		 "extends decoder with a parity bits."};

	sub->get_description(req_args, opt_args);

	auto ps = sub->get_prefix();

	opt_args.erase({ps+"-fra", "F"});
}

void Encoder_turbo_product_code::parameters
::store(const arg_val_map &vals)
{
	Encoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-ext"})) this->parity_extended = true;

	this->sub->n_frames = this->n_frames;

	sub->store(vals);

	this->K = this->sub->K * this->sub->K;

	this->R = (float)this->K / (float)this->N_cw;

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

void Encoder_turbo_product_code::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	itl->get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Parity extended", (this->parity_extended ? "yes" : "no")));

	sub->get_headers(headers, full);
}

template <typename B>
module::Encoder_turbo_product_code<B>* Encoder_turbo_product_code::parameters
::build(const module::Interleaver<B> &itl,
              module::Encoder_BCH<B> &enc_n,
              module::Encoder_BCH<B> &enc_i) const
{
	if (this->type == "TPC")
		return new module::Encoder_turbo_product_code<B>(itl, enc_n, enc_i);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_turbo_product_code<B>* Encoder_turbo_product_code
::build(const parameters              &params,
        const module::Interleaver<B> &itl,
              module::Encoder_BCH<B> &enc_n,
              module::Encoder_BCH<B> &enc_i)
{
	return params.template build<B>(itl, enc_n, enc_i);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_turbo_product_code<B_8 >* aff3ct::factory::Encoder_turbo_product_code::parameters::build<B_8 >(const aff3ct::module::Interleaver<B_8 >&, aff3ct::module::Encoder_BCH<B_8 >&, aff3ct::module::Encoder_BCH<B_8 >&) const;
template aff3ct::module::Encoder_turbo_product_code<B_16>* aff3ct::factory::Encoder_turbo_product_code::parameters::build<B_16>(const aff3ct::module::Interleaver<B_16>&, aff3ct::module::Encoder_BCH<B_16>&, aff3ct::module::Encoder_BCH<B_16>&) const;
template aff3ct::module::Encoder_turbo_product_code<B_32>* aff3ct::factory::Encoder_turbo_product_code::parameters::build<B_32>(const aff3ct::module::Interleaver<B_32>&, aff3ct::module::Encoder_BCH<B_32>&, aff3ct::module::Encoder_BCH<B_32>&) const;
template aff3ct::module::Encoder_turbo_product_code<B_64>* aff3ct::factory::Encoder_turbo_product_code::parameters::build<B_64>(const aff3ct::module::Interleaver<B_64>&, aff3ct::module::Encoder_BCH<B_64>&, aff3ct::module::Encoder_BCH<B_64>&) const;
template aff3ct::module::Encoder_turbo_product_code<B_8 >* aff3ct::factory::Encoder_turbo_product_code::build<B_8 >(const aff3ct::factory::Encoder_turbo_product_code::parameters&, const aff3ct::module::Interleaver<B_8 >&, aff3ct::module::Encoder_BCH<B_8 >&, aff3ct::module::Encoder_BCH<B_8 >&);
template aff3ct::module::Encoder_turbo_product_code<B_16>* aff3ct::factory::Encoder_turbo_product_code::build<B_16>(const aff3ct::factory::Encoder_turbo_product_code::parameters&, const aff3ct::module::Interleaver<B_16>&, aff3ct::module::Encoder_BCH<B_16>&, aff3ct::module::Encoder_BCH<B_16>&);
template aff3ct::module::Encoder_turbo_product_code<B_32>* aff3ct::factory::Encoder_turbo_product_code::build<B_32>(const aff3ct::factory::Encoder_turbo_product_code::parameters&, const aff3ct::module::Interleaver<B_32>&, aff3ct::module::Encoder_BCH<B_32>&, aff3ct::module::Encoder_BCH<B_32>&);
template aff3ct::module::Encoder_turbo_product_code<B_64>* aff3ct::factory::Encoder_turbo_product_code::build<B_64>(const aff3ct::factory::Encoder_turbo_product_code::parameters&, const aff3ct::module::Interleaver<B_64>&, aff3ct::module::Encoder_BCH<B_64>&, aff3ct::module::Encoder_BCH<B_64>&);
#else
template aff3ct::module::Encoder_turbo_product_code<B>* aff3ct::factory::Encoder_turbo_product_code::parameters::build<B>(const aff3ct::module::Interleaver<B>&, aff3ct::module::Encoder_BCH<B>&, aff3ct::module::Encoder_BCH<B>&) const;
template aff3ct::module::Encoder_turbo_product_code<B>* aff3ct::factory::Encoder_turbo_product_code::build<B>(const aff3ct::factory::Encoder_turbo_product_code::parameters&, const aff3ct::module::Interleaver<B>&, aff3ct::module::Encoder_BCH<B>&, aff3ct::module::Encoder_BCH<B>&);
#endif
// ==================================================================================== explicit template instantiation
