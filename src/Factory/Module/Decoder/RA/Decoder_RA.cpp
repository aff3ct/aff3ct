#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Decoder/RA/Decoder_RA.hpp"
#include "Factory/Module/Decoder/RA/Decoder_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_RA_name   = "Decoder RA";
const std::string aff3ct::factory::Decoder_RA_prefix = "dec";

Decoder_RA
::Decoder_RA(const std::string &prefix)
: Decoder(Decoder_RA_name, prefix),
  itl(new Interleaver("itl"))
{
	this->type   = "RA";
	this->implem = "STD";
}

Decoder_RA* Decoder_RA
::clone() const
{
	return new Decoder_RA(*this);
}

std::vector<std::string> Decoder_RA
::get_names() const
{
	auto n = Decoder::get_names();
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}
std::vector<std::string> Decoder_RA
::get_short_names() const
{
	auto sn = Decoder::get_short_names();
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Decoder_RA
::get_prefixes() const
{
	auto p = Decoder::get_prefixes();
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void Decoder_RA
::get_description(cli::Argument_map_info &args) const
{
	Decoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder_RA::";

	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = itl->get_prefix();

		args.erase({pi+"-size"});
	}

	cli::add_options(args.at({p+"-type", "D"}), 0, "RA");
	cli::add_options(args.at({p+"-implem"   }), 0, "STD");

	tools::add_arg(args, p, class_name+"p+ite,i",
		cli::Integer(cli::Positive(), cli::Non_zero()));
}

void Decoder_RA
::store(const cli::Argument_map_value &vals)
{
	Decoder::store(vals);

	auto p = this->get_prefix();

	if (itl != nullptr)
	{
		this->itl->core->size = this->N_cw;
		itl->store(vals);
	}

	if(vals.exist({p+"-ite", "i"})) this->n_ite = vals.to_int({p+"-ite", "i"});
}

void Decoder_RA
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Decoder::get_headers(headers, full);

	if (itl != nullptr)
		itl->get_headers(headers, full);

	if (this->type != "ML" && this->type != "CHASE")
	{
		auto p = this->get_prefix();

		headers[p].push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->n_ite)));
	}
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_RA
::build(const module::Interleaver<Q> &itl, module::Encoder<B> *encoder) const
{
	try
	{
		return Decoder::build<B,Q>(encoder);
	}
	catch (tools::cannot_allocate const&)
	{
		if (this->type == "RA")
		{
			if (this->implem == "STD" ) return new module::Decoder_RA<B,Q>(this->K, this->N_cw, itl, this->n_ite);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RA::build<B_8 ,Q_8 >(const aff3ct::module::Interleaver<Q_8 >&, module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_RA::build<B_16,Q_16>(const aff3ct::module::Interleaver<Q_16>&, module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_RA::build<B_32,Q_32>(const aff3ct::module::Interleaver<Q_32>&, module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_RA::build<B_64,Q_64>(const aff3ct::module::Interleaver<Q_64>&, module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_RA::build<B,Q>(aff3ct::module::Interleaver<Q>&, module::Encoder<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
