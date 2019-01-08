#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"

#include "Tools/general_utils.h"
#include "Tools/Arguments/Splitter/Splitter.hpp"

#include "Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah.hpp"
#include "Module/Decoder/Turbo_product/Decoder_turbo_product.hpp"

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
	return new Decoder_turbo_product::parameters(*this);
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

struct Real_splitter
{
	static std::vector<std::string> split(const std::string& val)
	{
		const std::string head      = "{([";
		const std::string queue     = "})]";
		const std::string separator = ",";

		return tools::Splitter::split(val, head, queue, separator);
	}
};

void Decoder_turbo_product::parameters
::get_description(tools::Argument_map_info &args) const
{
	Decoder::parameters::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder_turbo_product::parameters::";

	args.erase({p+"-info-bits", "K"});
	args.erase({p+"-cw-size",   "N"});

	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = this->itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}

	tools::add_options(args.at({p+"-type", "D"}), 0, "CP");
	tools::add_options(args.at({p+"-implem"   }), 0, "FAST");

	tools::add_arg(args, p, class_name+"p+ite,i",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+alpha",
		tools::List<float,Real_splitter>(tools::Real(), tools::Length(1)));

	tools::add_arg(args, p, class_name+"p+beta",
		tools::List<float,Real_splitter>(tools::Real(tools::Positive()), tools::Length(1)));

	tools::add_arg(args, p, class_name+"p+p",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+t",
		tools::Integer(tools::Positive()));

	tools::add_arg(args, p, class_name+"p+c",
		tools::Integer(tools::Positive()));

	tools::add_arg(args, p, class_name+"p+ext",
		tools::None());

	tools::add_arg(args, p, class_name+"p+cp-coef",
		tools::List<float,Real_splitter>(tools::Real(), tools::Length(5,5)));

	sub->get_description(args);

	auto ps = sub->get_prefix();

	args.erase({ps+"-fra", "F"});
}

void Decoder_turbo_product::parameters
::store(const tools::Argument_map_value &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-ite", "i"})) this->n_ite                      = vals.to_int({p+"-ite", "i"});
	if(vals.exist({p+"-p"       })) this->n_least_reliable_positions = vals.to_int({p+"-p"       });

	if(vals.exist({p+"-t"}))
		this->n_test_vectors = vals.to_int({p+"-t"});
	else
		this->n_test_vectors = 1<<this->n_least_reliable_positions;

	if(vals.exist({p+"-c"}))
		this->n_competitors = vals.to_int({p+"-c"});
	else
		this->n_competitors = this->n_test_vectors;

	if(vals.exist({p+"-ext"})) this->parity_extended = true;


	if(vals.exist({p+"-alpha"}))
	{
		this->alpha = vals.to_list<float>({p+"-alpha"});
		this->alpha.resize(this->n_ite*2, alpha.back());
	}
	else
	{
		this->alpha.clear();
		this->alpha.resize(this->n_ite*2, 0.5f);
	}

	if(vals.exist({p+"-beta"}))
	{
		this->beta = vals.to_list<float>({p+"-beta"});
		this->beta.resize(this->n_ite*2, beta.back());
	}
	else
	{
		this->beta.clear();
	}


	if(vals.exist({p+"-cp-coef"}))
		this->cp_coef = vals.to_list<float>({p+"-cp-coef"});
	else
	{
		this->cp_coef.clear();
		this->cp_coef.resize(5, 1.f);
		this->cp_coef[4] = 0;
	}


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

void Decoder_turbo_product::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	if (this->type != "ML")
	{
		auto p = this->get_prefix();

		if (itl != nullptr)
			itl->get_headers(headers, full);

		headers[p].push_back(std::make_pair("Num. of iterations (i)", std::to_string(n_ite)));

		std::stringstream alpha_str;
		alpha_str << "{";
		for (unsigned i = 0; i < this->alpha.size(); i++)
			alpha_str << this->alpha[i] << (i == this->alpha.size()-1 ?"":", ");
		alpha_str << "}";

		headers[p].push_back(std::make_pair("alpha", alpha_str.str()));

		if (this->beta.size())
		{
			std::stringstream beta_str;
			beta_str << "{";
			for (unsigned i = 0; i < this->beta.size(); i++)
				beta_str << this->beta[i] << (i == this->beta.size()-1 ?"":", ");
			beta_str << "}";

			headers[p].push_back(std::make_pair("beta", beta_str.str()));
		}

		std::stringstream cp_coeff_str;
		cp_coeff_str << "{";
		for (unsigned i = 0; i < this->cp_coef.size(); i++)
			cp_coeff_str << this->cp_coef[i] << (i == this->cp_coef.size()-1 ?"":", ");
		cp_coeff_str << "}";

		headers[p].push_back(std::make_pair("Chase Pyndiah coeff.", cp_coeff_str.str()));

		headers[p].push_back(std::make_pair("Num. least reliable pos (p)", std::to_string(n_least_reliable_positions)));

		headers[p].push_back(std::make_pair("Num. test vectors (t)", std::to_string(n_test_vectors)));

		headers[p].push_back(std::make_pair("Num. competitors (c)", std::to_string(n_competitors)));

		headers[p].push_back(std::make_pair("Parity extended", (this->parity_extended ? "yes" : "no")));

		sub->get_headers(headers, full);
	}
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_turbo_product::parameters
::build(const module::Interleaver <Q> &itl,
              module::Decoder_chase_pyndiah<B,Q> &cp_r,
              module::Decoder_chase_pyndiah<B,Q> &cp_c,
              const std::unique_ptr<module::Encoder<B>>& encoder) const
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
				return new module::Decoder_turbo_product<B,Q>(n_ite, alpha, itl, cp_r, cp_c, beta, n_frames);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_turbo_product::parameters
::build_siso(const module::Interleaver <Q> &itl,
                   module::Decoder_chase_pyndiah<B,Q> &cp_r,
                   module::Decoder_chase_pyndiah<B,Q> &cp_c) const
{
	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_turbo_product
::build(const parameters              &params,
        const module::Interleaver <Q> &itl,
              module::Decoder_chase_pyndiah<B,Q> &cp_r,
              module::Decoder_chase_pyndiah<B,Q> &cp_c,
              const std::unique_ptr<module::Encoder<B>>& encoder)
{
	return params.template build<B,Q>(itl, cp_r, cp_c, encoder);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_turbo_product
::build_siso(const parameters              &params,
             const module::Interleaver <Q> &itl,
                   module::Decoder_chase_pyndiah<B,Q> &cp_r,
                   module::Decoder_chase_pyndiah<B,Q> &cp_c)
{
	return params.template build_siso<B,Q>(itl, cp_r, cp_c);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_product::parameters::build<B_8 ,Q_8 >(const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_chase_pyndiah<B_8 ,Q_8 > &, aff3ct::module::Decoder_chase_pyndiah<B_8 ,Q_8 > &, const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_turbo_product::parameters::build<B_16,Q_16>(const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_chase_pyndiah<B_16,Q_16> &, aff3ct::module::Decoder_chase_pyndiah<B_16,Q_16> &, const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_turbo_product::parameters::build<B_32,Q_32>(const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_chase_pyndiah<B_32,Q_32> &, aff3ct::module::Decoder_chase_pyndiah<B_32,Q_32> &, const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_turbo_product::parameters::build<B_64,Q_64>(const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_chase_pyndiah<B_64,Q_64> &, aff3ct::module::Decoder_chase_pyndiah<B_64,Q_64> &, const std::unique_ptr<module::Encoder<B_64>>&) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_product::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_chase_pyndiah<B_8 ,Q_8 > &, aff3ct::module::Decoder_chase_pyndiah<B_8 ,Q_8 > &, const std::unique_ptr<module::Encoder<B_8 >>&);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_turbo_product::build<B_16,Q_16>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_chase_pyndiah<B_16,Q_16> &, aff3ct::module::Decoder_chase_pyndiah<B_16,Q_16> &, const std::unique_ptr<module::Encoder<B_16>>&);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_turbo_product::build<B_32,Q_32>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_chase_pyndiah<B_32,Q_32> &, aff3ct::module::Decoder_chase_pyndiah<B_32,Q_32> &, const std::unique_ptr<module::Encoder<B_32>>&);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_turbo_product::build<B_64,Q_64>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_chase_pyndiah<B_64,Q_64> &, aff3ct::module::Decoder_chase_pyndiah<B_64,Q_64> &, const std::unique_ptr<module::Encoder<B_64>>&);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_turbo_product::parameters::build<B,Q>(const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_chase_pyndiah<B,Q> &, aff3ct::module::Decoder_chase_pyndiah<B,Q> &, const std::unique_ptr<module::Encoder<B>>& ) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_turbo_product::build<B,Q>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_chase_pyndiah<B,Q> &, aff3ct::module::Decoder_chase_pyndiah<B,Q> &, const std::unique_ptr<module::Encoder<B>>& );
#endif

#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_product::parameters::build_siso<B_8 ,Q_8 >(const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_chase_pyndiah<B_8 ,Q_8 > &, aff3ct::module::Decoder_chase_pyndiah<B_8 ,Q_8 > &) const;
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_turbo_product::parameters::build_siso<B_16,Q_16>(const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_chase_pyndiah<B_16,Q_16> &, aff3ct::module::Decoder_chase_pyndiah<B_16,Q_16> &) const;
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_turbo_product::parameters::build_siso<B_32,Q_32>(const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_chase_pyndiah<B_32,Q_32> &, aff3ct::module::Decoder_chase_pyndiah<B_32,Q_32> &) const;
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_turbo_product::parameters::build_siso<B_64,Q_64>(const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_chase_pyndiah<B_64,Q_64> &, aff3ct::module::Decoder_chase_pyndiah<B_64,Q_64> &) const;
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_product::build_siso<B_8 ,Q_8 >(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_chase_pyndiah<B_8 ,Q_8 > &, aff3ct::module::Decoder_chase_pyndiah<B_8 ,Q_8 > &);
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_turbo_product::build_siso<B_16,Q_16>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_chase_pyndiah<B_16,Q_16> &, aff3ct::module::Decoder_chase_pyndiah<B_16,Q_16> &);
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_turbo_product::build_siso<B_32,Q_32>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_chase_pyndiah<B_32,Q_32> &, aff3ct::module::Decoder_chase_pyndiah<B_32,Q_32> &);
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_turbo_product::build_siso<B_64,Q_64>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_chase_pyndiah<B_64,Q_64> &, aff3ct::module::Decoder_chase_pyndiah<B_64,Q_64> &);
#else
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_turbo_product::parameters::build_siso<B,Q>(const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_chase_pyndiah<B,Q> &, aff3ct::module::Decoder_chase_pyndiah<B,Q> &) const;
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_turbo_product::build_siso<B,Q>(const aff3ct::factory::Decoder_turbo_product::parameters&, const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_chase_pyndiah<B,Q> &, aff3ct::module::Decoder_chase_pyndiah<B,Q> &);
#endif
// ==================================================================================== explicit template instantiation
