#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah.hpp"
#include "Module/Decoder/ML/Decoder_maximum_likelihood.hpp"

#include "Decoder_turbo_product.hpp"

const std::string aff3ct::factory::Decoder_turbo_product_name   = "Decoder Turbo Product Code";
const std::string aff3ct::factory::Decoder_turbo_product_prefix = "dec";

using namespace aff3ct;
using namespace aff3ct::factory;

Decoder_turbo_product::parameters
::parameters(const std::string &prefix)
: Decoder::parameters(Decoder_turbo_product_name, prefix),
  sub(new Decoder_BCH::parameters(prefix+"-sub")),
  itl(new Interleaver::parameters("itl"))
{
	this->type   = "CP";
	this->implem = "STD";
}

Decoder_turbo_product::parameters* Decoder_turbo_product::parameters
::clone() const
{
	auto clone = new Decoder_turbo_product::parameters(*this);

	if (sub != nullptr) { clone->sub = sub->clone(); }
	if (itl != nullptr) { clone->itl = itl->clone(); }

	return clone;
}

std::vector<std::string> Decoder_turbo_product::parameters
::get_names() const
{
	auto n = Decoder::parameters::get_names();
	if (sub != nullptr) { auto nn = sub->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> Decoder_turbo_product::parameters
::get_short_names() const
{
	auto sn = Decoder::parameters::get_short_names();
	if (sub != nullptr) { auto nn = sub->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Decoder_turbo_product::parameters
::get_prefixes() const
{
	auto p = Decoder::parameters::get_prefixes();
	if (sub != nullptr) { auto nn = sub->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

Decoder_turbo_product::parameters
::~parameters()
{
	if (sub != nullptr) { delete sub; sub = nullptr; }
	if (itl != nullptr) { delete itl; itl = nullptr; }
}

void Decoder_turbo_product::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Decoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.erase({p+"-info-bits", "K"});
	req_args.erase({p+"-cw-size",   "N"});

	itl->get_description(req_args, opt_args);

	auto pi = this->itl->get_prefix();

	req_args.erase({pi+"-size"    });
	opt_args.erase({pi+"-fra", "F"});

	opt_args[{p+"-type", "D"}][2] += ", CP";

	opt_args[{p+"-ite", "i"}] =
		{"strictly_positive_int",
		 "maximal number of iterations in the turbo."};

	opt_args[{p+"-alpha"}] =
		{"float",
		 "extrinsic coefficient."};

	opt_args[{p+"-p"}] =
		{"strictly_positive_int",
		 "number of least reliable positions."};

	opt_args[{p+"-c"}] =
		{"positive_int",
		 "number of competitors (0 means equal to number of test vectors)."};

	opt_args[{p+"-ext"}] =
		{"",
		 "extends decoder with a parity bits."};

	sub->get_description(req_args, opt_args);

	auto ps = sub->get_prefix();

	opt_args.erase({ps+"-fra", "F"});
}

void Decoder_turbo_product::parameters
::store(const arg_val_map &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-ite", "i"})) this->n_ite                      = std::stoi(vals.at({p+"-ite", "i"}));
	if(exist(vals, {p+"-p"       })) this->n_least_reliable_positions = std::stoi(vals.at({p+"-p"       }));
	if(exist(vals, {p+"-c"       })) this->n_competitors              = std::stoi(vals.at({p+"-c"       }));
	if(exist(vals, {p+"-alpha"   })) this->alpha                      = std::stof(vals.at({p+"-alpha"   }));
	if(exist(vals, {p+"-ext"     })) this->parity_extended            = true;

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

void Decoder_turbo_product::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	if (this->type != "ML")
	{
		auto p = this->get_prefix();

		itl->get_headers(headers, full);

		headers[p].push_back(std::make_pair("Num. of iterations (i)",      std::to_string(n_ite                     )));
		headers[p].push_back(std::make_pair("alpha",                       std::to_string(alpha                     )));
		headers[p].push_back(std::make_pair("Num. least reliable pos (p)", std::to_string(n_least_reliable_positions)));

		if (n_competitors != 0)
			headers[p].push_back(std::make_pair("Num. competitors (c)", std::to_string(n_competitors)));

		headers[p].push_back(std::make_pair("Parity extended", (this->parity_extended ? "yes" : "no")));

		sub->get_headers(headers, full);
	}
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_turbo_product::parameters
::build(const module::Interleaver <Q> &itl,
              module::Decoder_HIHO<B> &hiho_r,
              module::Decoder_HIHO<B> &hiho_c,
              module::Encoder     <B> &enc_r,
              module::Encoder     <B> &enc_c,
              module::Encoder     <B> *encoder) const
{
	try
	{
		return Decoder::parameters::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		if (this->type == "CP")
		{
			if (this->implem == "STD")
				return new module::Decoder_chase_pyndiah<B,Q>(this->n_ite, itl, hiho_r, hiho_c, enc_r.get_info_bits_pos(), enc_r.get_info_bits_pos(),
				                                              alpha, n_least_reliable_positions, n_competitors);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_turbo_product::parameters
::build_siso(const module::Interleaver <Q> &itl,
                   module::Decoder_HIHO<B> &hiho_r,
                   module::Decoder_HIHO<B> &hiho_c,
                   module::Encoder     <B> &enc_r,
                   module::Encoder     <B> &enc_c) const
{
	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_turbo_product
::build(const parameters              &params,
        const module::Interleaver <Q> &itl,
              module::Decoder_HIHO<B> &hiho_r,
              module::Decoder_HIHO<B> &hiho_c,
              module::Encoder     <B> &enc_r,
              module::Encoder     <B> &enc_c,
              module::Encoder     <B> *encoder)
{
	return params.template build<B,Q>(itl, hiho_r, hiho_c, enc_r, enc_c, encoder);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_turbo_product
::build_siso(const parameters              &params,
             const module::Interleaver <Q> &itl,
                   module::Decoder_HIHO<B> &hiho_r,
                   module::Decoder_HIHO<B> &hiho_c,
                   module::Encoder     <B> &enc_r,
                   module::Encoder     <B> &enc_c)
{
	return params.template build_siso<B,Q>(itl, hiho_r, hiho_c, enc_r, enc_c);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_product::parameters::build<B_8 ,Q_8 >(const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_HIHO<B_8 >&, aff3ct::module::Decoder_HIHO<B_8 >&, module::Encoder<B_8 >&, module::Encoder<B_8 >&, module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_turbo_product::parameters::build<B_16,Q_16>(const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_HIHO<B_16>&, aff3ct::module::Decoder_HIHO<B_16>&, module::Encoder<B_16>&, module::Encoder<B_16>&, module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_turbo_product::parameters::build<B_32,Q_32>(const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_HIHO<B_32>&, aff3ct::module::Decoder_HIHO<B_32>&, module::Encoder<B_32>&, module::Encoder<B_32>&, module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_turbo_product::parameters::build<B_64,Q_64>(const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_HIHO<B_64>&, aff3ct::module::Decoder_HIHO<B_64>&, module::Encoder<B_64>&, module::Encoder<B_64>&, module::Encoder<B_64>*) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_product::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_HIHO<B_8 >&, aff3ct::module::Decoder_HIHO<B_8 >&, module::Encoder<B_8 >&, module::Encoder<B_8 >&, module::Encoder<B_8 >*);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_turbo_product::build<B_16,Q_16>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_HIHO<B_16>&, aff3ct::module::Decoder_HIHO<B_16>&, module::Encoder<B_16>&, module::Encoder<B_16>&, module::Encoder<B_16>*);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_turbo_product::build<B_32,Q_32>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_HIHO<B_32>&, aff3ct::module::Decoder_HIHO<B_32>&, module::Encoder<B_32>&, module::Encoder<B_32>&, module::Encoder<B_32>*);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_turbo_product::build<B_64,Q_64>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_HIHO<B_64>&, aff3ct::module::Decoder_HIHO<B_64>&, module::Encoder<B_64>&, module::Encoder<B_64>&, module::Encoder<B_64>*);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_turbo_product::parameters::build<B,Q>(const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_HIHO<B>&, aff3ct::module::Decoder_HIHO<B>&, module::Encoder<B>&, module::Encoder<B>&, module::Encoder<B>*) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_turbo_product::build<B,Q>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_HIHO<B>&, aff3ct::module::Decoder_HIHO<B>&, module::Encoder<B>&, module::Encoder<B>&, module::Encoder<B>*);
#endif

#ifdef MULTI_PREC
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_product::parameters::build_siso<B_8 ,Q_8 >(const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_HIHO<B_8 >&, aff3ct::module::Decoder_HIHO<B_8 >&, module::Encoder<B_8 >&, module::Encoder<B_8 >&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_turbo_product::parameters::build_siso<B_16,Q_16>(const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_HIHO<B_16>&, aff3ct::module::Decoder_HIHO<B_16>&, module::Encoder<B_16>&, module::Encoder<B_16>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_turbo_product::parameters::build_siso<B_32,Q_32>(const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_HIHO<B_32>&, aff3ct::module::Decoder_HIHO<B_32>&, module::Encoder<B_32>&, module::Encoder<B_32>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_turbo_product::parameters::build_siso<B_64,Q_64>(const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_HIHO<B_64>&, aff3ct::module::Decoder_HIHO<B_64>&, module::Encoder<B_64>&, module::Encoder<B_64>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_product::build_siso<B_8 ,Q_8 >(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_HIHO<B_8 >&, aff3ct::module::Decoder_HIHO<B_8 >&, module::Encoder<B_8 >&, module::Encoder<B_8 >&);
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_turbo_product::build_siso<B_16,Q_16>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_HIHO<B_16>&, aff3ct::module::Decoder_HIHO<B_16>&, module::Encoder<B_16>&, module::Encoder<B_16>&);
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_turbo_product::build_siso<B_32,Q_32>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_HIHO<B_32>&, aff3ct::module::Decoder_HIHO<B_32>&, module::Encoder<B_32>&, module::Encoder<B_32>&);
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_turbo_product::build_siso<B_64,Q_64>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_HIHO<B_64>&, aff3ct::module::Decoder_HIHO<B_64>&, module::Encoder<B_64>&, module::Encoder<B_64>&);
#else
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_turbo_product::parameters::build_siso<B,Q>(const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_HIHO<B>&, aff3ct::module::Decoder_HIHO<B>&, module::Encoder<B>&, module::Encoder<B>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_turbo_product::build_siso<B,Q>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_HIHO<B>&, aff3ct::module::Decoder_HIHO<B>&, module::Encoder<B>&, module::Encoder<B>&);
#endif
// ==================================================================================== explicit template instantiation
