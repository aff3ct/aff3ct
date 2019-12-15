#if !defined(AFF3CT_8BIT_PREC) && !defined(AFF3CT_16BIT_PREC)

#include <utility>
#include <tuple>

#include "Tools/Documentation/documentation.h"
#include "Simulation/Legacy/EXIT/EXIT.hpp"
#include "Tools/general_utils.h"
#include "Factory/Simulation/EXIT/EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::EXIT_name   = "Simulation EXIT";
const std::string aff3ct::factory::EXIT_prefix = "sim";

EXIT
::EXIT(const std::string &prefix)
: Simulation(EXIT_name, prefix)
{
}

EXIT* EXIT
::clone() const
{
	return new EXIT(*this);

	// if (src != nullptr) { clone->src = src->clone(); }
	// if (cdc != nullptr) { clone->cdc = dynamic_cast<Codec_SISO*>(cdc->clone()); }
	// if (mdm != nullptr) { clone->mdm = mdm->clone(); }
	// if (chn != nullptr) { clone->chn = chn->clone(); }
	// if (qnt != nullptr) { clone->qnt = qnt->clone(); }
	// if (mnt != nullptr) { clone->mnt = mnt->clone(); }
	// if (ter != nullptr) { clone->ter = ter->clone(); }

	// return clone;
}

std::vector<std::string> EXIT
::get_names() const
{
	auto n = Simulation::get_names();
	if (src != nullptr) { auto nn = src->get_names(); for (auto &x : nn) n.push_back(x); }
	if (cdc != nullptr) { auto nn = cdc->get_names(); for (auto &x : nn) n.push_back(x); }
	if (mdm != nullptr) { auto nn = mdm->get_names(); for (auto &x : nn) n.push_back(x); }
	if (chn != nullptr) { auto nn = chn->get_names(); for (auto &x : nn) n.push_back(x); }
	if (qnt != nullptr) { auto nn = qnt->get_names(); for (auto &x : nn) n.push_back(x); }
	if (mnt != nullptr) { auto nn = mnt->get_names(); for (auto &x : nn) n.push_back(x); }
	if (ter != nullptr) { auto nn = ter->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> EXIT
::get_short_names() const
{
	auto sn = Factory::get_short_names();
	if (src != nullptr) { auto nn = src->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (cdc != nullptr) { auto nn = cdc->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (mdm != nullptr) { auto nn = mdm->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (chn != nullptr) { auto nn = chn->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (qnt != nullptr) { auto nn = qnt->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (mnt != nullptr) { auto nn = mnt->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (ter != nullptr) { auto nn = ter->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> EXIT
::get_prefixes() const
{
	auto p = Factory::get_prefixes();
	if (src != nullptr) { auto nn = src->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (cdc != nullptr) { auto nn = cdc->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (mdm != nullptr) { auto nn = mdm->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (chn != nullptr) { auto nn = chn->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (qnt != nullptr) { auto nn = qnt->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (mnt != nullptr) { auto nn = mnt->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (ter != nullptr) { auto nn = ter->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void EXIT
::get_description(cli::Argument_map_info &args) const
{
	Simulation::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::EXIT::";

	tools::add_arg(args, p, class_name+"p+siga-range",
		cli::Matlab_vector<float>(cli::Real(cli::Positive()), std::make_tuple(cli::Length(1)), std::make_tuple(cli::Length(1,3))),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+siga-min,a",
		cli::Real(cli::Positive()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+siga-max,A",
		cli::Real(cli::Positive()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+siga-step",
		cli::Real(cli::Positive(), cli::Non_zero()));

	args.add_link({p+"-siga-range"}, {p+"-siga-min", "a"});
	args.add_link({p+"-siga-range"}, {p+"-siga-max", "A"});
}

void EXIT
::store(const cli::Argument_map_value &vals)
{
	Simulation::store(vals);

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

void EXIT
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Simulation::get_headers(headers, full);

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

void EXIT
::set_src(Source *src)
{
	this->src.reset(src);
}

void EXIT
::set_cdc(Codec_SISO *cdc)
{
	this->cdc.reset(cdc);
}

void EXIT
::set_mdm(Modem *mdm)
{
	this->mdm.reset(mdm);
}

void EXIT
::set_chn(Channel *chn)
{
	this->chn.reset(chn);
}

void EXIT
::set_qnt(Quantizer *qnt)
{
	this->qnt.reset(qnt);
}

void EXIT
::set_mnt(Monitor_EXIT *mnt)
{
	this->mnt.reset(mnt);
}

void EXIT
::set_ter(Terminal *ter)
{
	this->ter.reset(ter);
}


template <typename B, typename R>
simulation::Simulation* EXIT
::build() const
{
#if defined(AFF3CT_SYSTEMC_SIMU)
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC/TLM  simulation is not available.");
#else
	return new simulation::EXIT<B,R>(*this);
#endif

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::simulation::Simulation* aff3ct::factory::EXIT::build<B_32,R_32>() const;
template aff3ct::simulation::Simulation* aff3ct::factory::EXIT::build<B_64,R_64>() const;
#else
template aff3ct::simulation::Simulation* aff3ct::factory::EXIT::build<B,R>() const;
#endif
// ==================================================================================== explicit template instantiation

#endif