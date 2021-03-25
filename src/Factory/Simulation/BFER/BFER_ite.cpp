#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Simulation/BFER/Iterative/Simulation_BFER_ite.hpp"
#include "Factory/Simulation/BFER/BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER_ite_name   = "Simulation BFER iterative";
const std::string aff3ct::factory::BFER_ite_prefix = "sim";

BFER_ite
::BFER_ite(const std::string &prefix)
: BFER(BFER_ite_name, prefix)
{
}

BFER_ite* BFER_ite
::clone() const
{
	return new BFER_ite(*this);
}

std::vector<std::string> BFER_ite
::get_names() const
{
	auto n = Simulation::get_names();
	if (this->src    != nullptr) { auto nn = this->src   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->crc    != nullptr) { auto nn = this->crc   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->cdc    != nullptr) { auto nn = this->cdc   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->itl    != nullptr) { auto nn = this->itl   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->mdm    != nullptr) { auto nn = this->mdm   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->chn    != nullptr) { auto nn = this->chn   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->qnt    != nullptr) { auto nn = this->qnt   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->mnt_er != nullptr) { auto nn = this->mnt_er->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->mnt_mi != nullptr) { auto nn = this->mnt_mi->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->ter    != nullptr) { auto nn = this->ter   ->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> BFER_ite
::get_short_names() const
{
	auto sn = Factory::get_short_names();
	if (this->src    != nullptr) { auto nn = this->src   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->crc    != nullptr) { auto nn = this->crc   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->cdc    != nullptr) { auto nn = this->cdc   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->itl    != nullptr) { auto nn = this->itl   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->mdm    != nullptr) { auto nn = this->mdm   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->chn    != nullptr) { auto nn = this->chn   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->qnt    != nullptr) { auto nn = this->qnt   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->mnt_er != nullptr) { auto nn = this->mnt_er->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->mnt_mi != nullptr) { auto nn = this->mnt_mi->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->ter    != nullptr) { auto nn = this->ter   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> BFER_ite
::get_prefixes() const
{
	auto p = Factory::get_prefixes();
	if (this->src    != nullptr) { auto nn = this->src   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->crc    != nullptr) { auto nn = this->crc   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->cdc    != nullptr) { auto nn = this->cdc   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->itl    != nullptr) { auto nn = this->itl   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->mdm    != nullptr) { auto nn = this->mdm   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->chn    != nullptr) { auto nn = this->chn   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->qnt    != nullptr) { auto nn = this->qnt   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->mnt_er != nullptr) { auto nn = this->mnt_er->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->mnt_mi != nullptr) { auto nn = this->mnt_mi->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->ter    != nullptr) { auto nn = this->ter   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void BFER_ite
::get_description(cli::Argument_map_info &args) const
{
	BFER::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::BFER_ite::";

	tools::add_arg(args, p, class_name+"p+ite,I",
		cli::Integer(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+crc-earlyt",
		cli::None());
}

void BFER_ite
::store(const cli::Argument_map_value &vals)
{
	BFER::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-ite",   "I"})) this->n_ite = vals.to_int({p+"-ite", "I"});
	if(vals.exist({p+"-crc-earlyt"})) this->crc_early_termination = true;

	this->mnt_mutinfo = false;
}

void BFER_ite
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	BFER::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Global iterations (I)", std::to_string(this->n_ite)));

	if (this->crc != nullptr && this->crc->type != "NO")
		headers[p].push_back(std::make_pair("CRC early termination", this->crc_early_termination ? "on" : "off"));

	if (this->src    != nullptr) { this->src   ->get_headers(headers, full); }
	if (this->crc    != nullptr) { this->crc   ->get_headers(headers, full); }
	if (this->cdc    != nullptr) { this->cdc   ->get_headers(headers, full); }
	if (this->itl    != nullptr) { this->itl   ->get_headers(headers, full); }
	if (this->mdm    != nullptr) { this->mdm   ->get_headers(headers, full); }
	if (this->chn    != nullptr) { this->chn   ->get_headers(headers, full); }
	if (this->qnt    != nullptr) { this->qnt   ->get_headers(headers, full); }
	if (this->mnt_er != nullptr) { this->mnt_er->get_headers(headers, full); }
	if (this->ter    != nullptr) { this->ter   ->get_headers(headers, full); }
}

const Codec_SISO* BFER_ite
::get_cdc() const
{
	return dynamic_cast<Codec_SISO*>(this->cdc.get());
}

template <typename B, typename R, typename Q>
simulation::Simulation* BFER_ite
::build() const
{
	return new simulation::Simulation_BFER_ite<B,R,Q>(*this);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::simulation::Simulation* aff3ct::factory::BFER_ite::build<B_8 ,R_8 ,Q_8 >() const;
template aff3ct::simulation::Simulation* aff3ct::factory::BFER_ite::build<B_16,R_16,Q_16>() const;
template aff3ct::simulation::Simulation* aff3ct::factory::BFER_ite::build<B_32,R_32,Q_32>() const;
template aff3ct::simulation::Simulation* aff3ct::factory::BFER_ite::build<B_64,R_64,Q_64>() const;
#else
template aff3ct::simulation::Simulation* aff3ct::factory::BFER_ite::build<B,R,Q>() const;
#endif
// ==================================================================================== explicit template instantiation

