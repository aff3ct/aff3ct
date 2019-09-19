#if !defined(AFF3CT_8BIT_PREC) && !defined(AFF3CT_16BIT_PREC)

#include <utility>
#include <tuple>

#include "Tools/Documentation/documentation.h"
#include "Simulation/EXIT/EXIT.hpp"
#include "Tools/general_utils.h"
#include "Factory/Simulation/EXIT/EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::EXIT_name   = "Simulation EXIT";
const std::string aff3ct::factory::EXIT_prefix = "sim";

EXIT::parameters
::parameters(const std::string &prefix)
: Simulation::parameters(EXIT_name, prefix)
{
}

EXIT::parameters* EXIT::parameters
::clone() const
{
	return new EXIT::parameters(*this);

	// if (src != nullptr) { clone->src = src->clone(); }
	// if (cdc != nullptr) { clone->cdc = dynamic_cast<Codec_SISO::parameters*>(cdc->clone()); }
	// if (mdm != nullptr) { clone->mdm = mdm->clone(); }
	// if (chn != nullptr) { clone->chn = chn->clone(); }
	// if (qnt != nullptr) { clone->qnt = qnt->clone(); }
	// if (mnt != nullptr) { clone->mnt = mnt->clone(); }
	// if (ter != nullptr) { clone->ter = ter->clone(); }

	// return clone;
}

std::vector<std::string> EXIT::parameters
::get_names() const
{
	auto n = Simulation::parameters::get_names();
	if (src != nullptr) { auto nn = src->get_names(); for (auto &x : nn) n.push_back(x); }
	if (cdc != nullptr) { auto nn = cdc->get_names(); for (auto &x : nn) n.push_back(x); }
	if (mdm != nullptr) { auto nn = mdm->get_names(); for (auto &x : nn) n.push_back(x); }
	if (chn != nullptr) { auto nn = chn->get_names(); for (auto &x : nn) n.push_back(x); }
	if (qnt != nullptr) { auto nn = qnt->get_names(); for (auto &x : nn) n.push_back(x); }
	if (mnt != nullptr) { auto nn = mnt->get_names(); for (auto &x : nn) n.push_back(x); }
	if (ter != nullptr) { auto nn = ter->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> EXIT::parameters
::get_short_names() const
{
	auto sn = Factory::parameters::get_short_names();
	if (src != nullptr) { auto nn = src->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (cdc != nullptr) { auto nn = cdc->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (mdm != nullptr) { auto nn = mdm->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (chn != nullptr) { auto nn = chn->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (qnt != nullptr) { auto nn = qnt->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (mnt != nullptr) { auto nn = mnt->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (ter != nullptr) { auto nn = ter->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> EXIT::parameters
::get_prefixes() const
{
	auto p = Factory::parameters::get_prefixes();
	if (src != nullptr) { auto nn = src->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (cdc != nullptr) { auto nn = cdc->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (mdm != nullptr) { auto nn = mdm->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (chn != nullptr) { auto nn = chn->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (qnt != nullptr) { auto nn = qnt->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (mnt != nullptr) { auto nn = mnt->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (ter != nullptr) { auto nn = ter->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void EXIT::parameters
::get_description(tools::Argument_map_info &args) const
{
	Simulation::parameters::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::EXIT::parameters::";

	tools::add_arg(args, p, class_name+"p+siga-range",
		tools::Matlab_vector<float>(tools::Real(tools::Positive()), std::make_tuple(tools::Length(1)), std::make_tuple(tools::Length(1,3))),
		tools::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+siga-min,a",
		tools::Real(tools::Positive()),
		tools::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+siga-max,A",
		tools::Real(tools::Positive()),
		tools::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+siga-step",
		tools::Real(tools::Positive(), tools::Non_zero()));

	args.add_link({p+"-siga-range"}, {p+"-siga-min", "a"});
	args.add_link({p+"-siga-range"}, {p+"-siga-max", "A"});
}

void EXIT::parameters
::store(const tools::Argument_map_value &vals)
{
	Simulation::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-siga-range"}))
		this->sig_a_range = tools::generate_range(vals.to_list<std::vector<float>>({p+"-siga-range"}), 0.1f);
	else
	{
		float sig_a_min = 0.f, sig_a_max = 0.f, sig_a_step = 0.1f;
		if(vals.exist({p+"-siga-min",  "a"})) sig_a_min  = vals.to_float({p+"-siga-min",  "a"});
		if(vals.exist({p+"-siga-max",  "A"})) sig_a_max  = vals.to_float({p+"-siga-max",  "A"});
		if(vals.exist({p+"-siga-step"     })) sig_a_step = vals.to_float({p+"-siga-step"     });

		this->sig_a_range = tools::generate_range({{sig_a_min, sig_a_max}}, sig_a_step);
	}
}

void EXIT::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Simulation::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	std::stringstream sig_a_range_str;
	sig_a_range_str << this->sig_a_range.front() << " -> " << this->sig_a_range.back();
	headers[p].push_back(std::make_pair("Sigma-A range (a)", sig_a_range_str.str()));

	if (this->src != nullptr && this->cdc != nullptr)
	{
		const auto bit_rate = (float)this->src->K / (float)this->cdc->N_cw;
		headers[p].push_back(std::make_pair("Bit rate", std::to_string(bit_rate)));
	}

	if (this->src != nullptr)
		headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->src->n_frames)));

	if (this->src != nullptr) { this->src->get_headers(headers, full); }
	if (this->cdc != nullptr) { this->cdc->get_headers(headers, full); }
	if (this->mdm != nullptr) { this->mdm->get_headers(headers, full); }
	if (this->chn != nullptr) { this->chn->get_headers(headers, full); }
	if (this->qnt != nullptr) { this->qnt->get_headers(headers, full); }
	if (this->mnt != nullptr) { this->mnt->get_headers(headers, full); }
	if (this->ter != nullptr) { this->ter->get_headers(headers, full); }
}

void EXIT::parameters
::set_src(Source::parameters *src)
{
	this->src.reset(src);
}

void EXIT::parameters
::set_cdc(Codec_SISO::parameters *cdc)
{
	this->cdc.reset(cdc);
}

void EXIT::parameters
::set_mdm(Modem::parameters *mdm)
{
	this->mdm.reset(mdm);
}

void EXIT::parameters
::set_chn(Channel::parameters *chn)
{
	this->chn.reset(chn);
}

void EXIT::parameters
::set_qnt(Quantizer::parameters *qnt)
{
	this->qnt.reset(qnt);
}

void EXIT::parameters
::set_mnt(Monitor_EXIT::parameters *mnt)
{
	this->mnt.reset(mnt);
}

void EXIT::parameters
::set_ter(Terminal::parameters *ter)
{
	this->ter.reset(ter);
}


template <typename B, typename R>
simulation::EXIT<B,R>* EXIT::parameters
::build() const
{
#if defined(AFF3CT_SYSTEMC_SIMU)
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC/TLM  simulation is not available.");
#else
	return new simulation::EXIT<B,R>(*this);
#endif

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
simulation::EXIT<B,R>* EXIT
::build(const parameters &params)
{
	return params.template build<B,R>();
}



// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::simulation::EXIT<B_32,R_32>* aff3ct::factory::EXIT::build<B_32,R_32>(const aff3ct::factory::EXIT::parameters&);
template aff3ct::simulation::EXIT<B_64,R_64>* aff3ct::factory::EXIT::build<B_64,R_64>(const aff3ct::factory::EXIT::parameters&);
#else
template aff3ct::simulation::EXIT<B,R>* aff3ct::factory::EXIT::build<B,R>(const aff3ct::factory::EXIT::parameters&);
#endif
// ==================================================================================== explicit template instantiation

#endif