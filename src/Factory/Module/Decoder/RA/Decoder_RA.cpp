#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/RA/Decoder_RA.hpp"

#include "Decoder_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_RA_name   = "Decoder RA";
const std::string aff3ct::factory::Decoder_RA_prefix = "dec";

Decoder_RA::parameters
::parameters(const std::string &prefix)
: Decoder::parameters(Decoder_RA_name, prefix),
  itl(new Interleaver::parameters("itl"))
{
	this->type   = "RA";
	this->implem = "STD";
}

Decoder_RA::parameters
::~parameters()
{
	if (itl != nullptr) { delete itl; itl = nullptr; }
}

Decoder_RA::parameters* Decoder_RA::parameters
::clone() const
{
	auto clone = new Decoder_RA::parameters(*this);

	if (itl != nullptr) { clone->itl = itl->clone(); }

	return clone;
}

std::vector<std::string> Decoder_RA::parameters
::get_names() const
{
	auto n = Decoder::parameters::get_names();
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}
std::vector<std::string> Decoder_RA::parameters
::get_short_names() const
{
	auto sn = Decoder::parameters::get_short_names();
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Decoder_RA::parameters
::get_prefixes() const
{
	auto p = Decoder::parameters::get_prefixes();
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void Decoder_RA::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Decoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	itl->get_description(req_args, opt_args);

	auto pi = itl->get_prefix();

	req_args.erase({pi+"-size"    });
	opt_args.erase({pi+"-fra", "F"});

	tools::add_options(opt_args.at({p+"-type", "D"}), 0, "RA");
	tools::add_options(opt_args.at({p+"-implem"   }), 0, "STD");

	opt_args.add(
		{p+"-ite", "i"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"maximal number of iterations in the decoder.");
}

void Decoder_RA::parameters
::store(const tools::Argument_map_value &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	this->itl->core->size     = this->N_cw;
	this->itl->core->n_frames = this->n_frames;

	itl->store(vals);

	if(vals.exist({p+"-ite", "i"})) this->n_ite = vals.to_int({p+"-ite", "i"});
}

void Decoder_RA::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	itl->get_headers(headers, full);

	if (this->type != "ML" && this->type != "CHASE")
	{
		auto p = this->get_prefix();
		
		headers[p].push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->n_ite)));
	}
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_RA::parameters
::build(const module::Interleaver<Q> &itl, module::Encoder<B> *encoder) const
{
	try
	{
		return Decoder::parameters::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		if (this->type == "RA")
		{
			if (this->implem == "STD" ) return new module::Decoder_RA<B,Q>(this->K, this->N_cw, itl, this->n_ite, this->n_frames);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_RA
::build(const parameters &params, const module::Interleaver<Q> &itl, module::Encoder<B> *encoder)
{
	return params.template build<B,Q>(itl, encoder);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RA::parameters::build<B_8 ,Q_8 >(const aff3ct::module::Interleaver<Q_8 >&, module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_RA::parameters::build<B_16,Q_16>(const aff3ct::module::Interleaver<Q_16>&, module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_RA::parameters::build<B_32,Q_32>(const aff3ct::module::Interleaver<Q_32>&, module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_RA::parameters::build<B_64,Q_64>(const aff3ct::module::Interleaver<Q_64>&, module::Encoder<B_64>*) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RA::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<Q_8 >&, module::Encoder<B_8 >*);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_RA::build<B_16,Q_16>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<Q_16>&, module::Encoder<B_16>*);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_RA::build<B_32,Q_32>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<Q_32>&, module::Encoder<B_32>*);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_RA::build<B_64,Q_64>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<Q_64>&, module::Encoder<B_64>*);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_RA::parameters::build<B,Q>(const aff3ct::module::Interleaver<Q>&, module::Encoder<B>*) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_RA::build<B,Q>(const aff3ct::factory::Decoder_RA::parameters&, const aff3ct::module::Interleaver<Q>&, module::Encoder<B>*);
#endif
// ==================================================================================== explicit template instantiation
