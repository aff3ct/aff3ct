#if !defined(PREC_8_BIT) && !defined(PREC_16_BIT)

#include "Simulation/EXIT/EXIT.hpp"
#include "Tools/general_utils.h"

#include "EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::EXIT_name   = "Simulation EXIT";
const std::string aff3ct::factory::EXIT_prefix = "sim";

EXIT::parameters
::parameters(const std::string &prefix)
: Simulation::parameters(EXIT_name, prefix)
{
}

EXIT::parameters
::~parameters()
{
	if (src != nullptr) { delete src; src = nullptr; }
	if (cdc != nullptr) { delete cdc; cdc = nullptr; }
	if (mdm != nullptr) { delete mdm; mdm = nullptr; }
	if (chn != nullptr) { delete chn; chn = nullptr; }
	if (qnt != nullptr) { delete qnt; qnt = nullptr; }
	if (mnt != nullptr) { delete mnt; mnt = nullptr; }
	if (ter != nullptr) { delete ter; ter = nullptr; }
}

EXIT::parameters* EXIT::parameters
::clone() const
{
	auto clone = new EXIT::parameters(*this);

	if (src != nullptr) { clone->src = src->clone(); }
	if (cdc != nullptr) { clone->cdc = dynamic_cast<Codec_SISO::parameters*>(cdc->clone()); }
	if (mdm != nullptr) { clone->mdm = mdm->clone(); }
	if (chn != nullptr) { clone->chn = chn->clone(); }
	if (qnt != nullptr) { clone->qnt = qnt->clone(); }
	if (mnt != nullptr) { clone->mnt = mnt->clone(); }
	if (ter != nullptr) { clone->ter = ter->clone(); }

	return clone;
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

struct sigma_range_D1_splitter : tools::Splitter
{
	static std::vector<std::string> split(const std::string& val)
	{
		const std::string head      = "{([";
		const std::string queue     = "})]";
		const std::string separator = ",";

		return Splitter::split(val, head, queue, separator);
	}
};

struct sigma_range_D2_splitter : tools::Splitter
{
	static std::vector<std::string> split(const std::string& val)
	{
		const std::string head      = "";
		const std::string queue     = "";
		const std::string separator = ":";

		return Splitter::split(val, head, queue, separator);
	}
};

void EXIT::parameters
::get_description(tools::Argument_map_info &args) const
{
	Simulation::parameters::get_description(args);

	auto p = this->get_prefix();

	args.add(
		{p+"-siga-range"},
		tools::List2D<float,sigma_range_D1_splitter,sigma_range_D2_splitter>(
		              tools::Real(),
		              std::make_tuple(tools::Length(1)),
		              std::make_tuple(tools::Length(1,3))),
		"sigma range used in EXIT charts (Matlab style: \"0.5:2.5,2.55,2.6:0.05:3\" with a default step of 0.1).",
		tools::Argument_info::REQUIRED);

	args.add(
		{p+"-siga-min", "a"},
		tools::Real(tools::Positive()),
		"sigma min value used in EXIT charts.",
		tools::Argument_info::REQUIRED);

	args.add(
		{p+"-siga-max", "A"},
		tools::Real(tools::Positive()),
		"sigma max value used in EXIT charts.",
		tools::Argument_info::REQUIRED);

	args.add(
		{p+"-siga-step"},
		tools::Real(tools::Positive(), tools::Non_zero()),
		"sigma step value used in EXIT charts.");

	args.add_link({p+"-siga-range"}, {p+"-siga-min",  "a"});
	args.add_link({p+"-siga-range"}, {p+"-siga-max",  "A"});
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
		float snr_min = 0.f, snr_max = 0.f, snr_step = 0.1f;
		if(vals.exist({p+"-siga-min",  "m"})) snr_min  = vals.to_float({p+"-siga-min",  "m"});
		if(vals.exist({p+"-siga-max",  "M"})) snr_max  = vals.to_float({p+"-siga-max",  "M"});
		if(vals.exist({p+"-siga-step"     })) snr_step = vals.to_float({p+"-siga-step"     });

		this->snr_range = tools::generate_range({{snr_min, snr_max}}, snr_step);
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

template <typename B, typename R>
simulation::EXIT<B,R>* EXIT::parameters
::build() const
{
#if defined(SYSTEMC)
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC/TLM  simulation is not available.");
#else
	return new simulation::EXIT<B,R>(*this);
#endif
}

template <typename B, typename R>
simulation::EXIT<B,R>* EXIT
::build(const parameters &params)
{
	return params.template build<B,R>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::simulation::EXIT<B_32,R_32>* aff3ct::factory::EXIT::build<B_32,R_32>(const aff3ct::factory::EXIT::parameters&);
template aff3ct::simulation::EXIT<B_64,R_64>* aff3ct::factory::EXIT::build<B_64,R_64>(const aff3ct::factory::EXIT::parameters&);
#else
template aff3ct::simulation::EXIT<B,R>* aff3ct::factory::EXIT::build<B,R>(const aff3ct::factory::EXIT::parameters&);
#endif
// ==================================================================================== explicit template instantiation

#endif