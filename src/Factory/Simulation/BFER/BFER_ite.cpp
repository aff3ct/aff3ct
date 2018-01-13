#include "Tools/Exception/exception.hpp"

#include "Simulation/BFER/Iterative/SystemC/SC_BFER_ite.hpp"
#include "Simulation/BFER/Iterative/Threads/BFER_ite_threads.hpp"

#include "BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER_ite_name   = "Simulation BFER iterative";
const std::string aff3ct::factory::BFER_ite_prefix = "sim";

BFER_ite::parameters
::parameters(const std::string &prefix)
: BFER::parameters(BFER_ite_name, prefix)
{
}

BFER_ite::parameters
::~parameters()
{
	if (itl != nullptr) { delete itl; itl = nullptr; }
}

BFER_ite::parameters* BFER_ite::parameters
::clone() const
{
	auto clone = new BFER_ite::parameters(*this);

	if (src != nullptr) { clone->src = src->clone(); }
	if (crc != nullptr) { clone->crc = crc->clone(); }
	if (cdc != nullptr) { clone->cdc = cdc->clone(); }
	if (mdm != nullptr) { clone->mdm = mdm->clone(); }
	if (chn != nullptr) { clone->chn = chn->clone(); }
	if (qnt != nullptr) { clone->qnt = qnt->clone(); }
	if (mnt != nullptr) { clone->mnt = mnt->clone(); }
	if (ter != nullptr) { clone->ter = ter->clone(); }
	if (itl != nullptr) { clone->itl = itl->clone(); }

	clone->set_cdc(clone->cdc);

	return clone;
}

std::vector<std::string> BFER_ite::parameters
::get_names() const
{
	auto n = Simulation::parameters::get_names();
	if (this->src != nullptr) { auto nn = this->src->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->crc != nullptr) { auto nn = this->crc->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->cdc != nullptr) { auto nn = this->cdc->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->itl != nullptr) { auto nn = this->itl->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->mdm != nullptr) { auto nn = this->mdm->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->chn != nullptr) { auto nn = this->chn->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->qnt != nullptr) { auto nn = this->qnt->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->mnt != nullptr) { auto nn = this->mnt->get_names(); for (auto &x : nn) n.push_back(x); }
	if (this->ter != nullptr) { auto nn = this->ter->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> BFER_ite::parameters
::get_short_names() const
{
	auto sn = Factory::parameters::get_short_names();
	if (this->src != nullptr) { auto nn = this->src->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->crc != nullptr) { auto nn = this->crc->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->cdc != nullptr) { auto nn = this->cdc->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->itl != nullptr) { auto nn = this->itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->mdm != nullptr) { auto nn = this->mdm->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->chn != nullptr) { auto nn = this->chn->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->qnt != nullptr) { auto nn = this->qnt->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->mnt != nullptr) { auto nn = this->mnt->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (this->ter != nullptr) { auto nn = this->ter->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> BFER_ite::parameters
::get_prefixes() const
{
	auto p = Factory::parameters::get_prefixes();
	if (this->src != nullptr) { auto nn = this->src->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->crc != nullptr) { auto nn = this->crc->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->cdc != nullptr) { auto nn = this->cdc->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->itl != nullptr) { auto nn = this->itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->mdm != nullptr) { auto nn = this->mdm->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->chn != nullptr) { auto nn = this->chn->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->qnt != nullptr) { auto nn = this->qnt->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->mnt != nullptr) { auto nn = this->mnt->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (this->ter != nullptr) { auto nn = this->ter->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void BFER_ite::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	BFER::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	opt_args[{p+"-ite", "I"}] =
		{"positive_int",
		 "number of global iterations between the demodulator and the decoder."};

	opt_args[{p+"-crc-start"}] =
		{"positive_int",
		 "iteration number to start the CRC checking in the turbo demodulation process."};
}

void BFER_ite::parameters
::store(const arg_val_map &vals)
{
	BFER::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-ite",  "I"})) this->n_ite     = std::stoi(vals.at({p+"-ite",  "I"}));
	if(exist(vals, {p+"-crc-start"})) this->crc_start = std::stoi(vals.at({p+"-crc-start"}));
}

void BFER_ite::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	BFER::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Global iterations (I)", std::to_string(this->n_ite)));

	if (this->crc != nullptr && this->crc->type != "NO")
		headers[p].push_back(std::make_pair("CRC start ite.", std::to_string(this->crc_start)));

	if (this->src != nullptr) { this->src->get_headers(headers, full); }
	if (this->crc != nullptr) { this->crc->get_headers(headers, full); }
	if (this->cdc != nullptr) { this->cdc->get_headers(headers, full); }
	if (this->itl != nullptr) { this->itl->get_headers(headers, full); }
	if (this->mdm != nullptr) { this->mdm->get_headers(headers, full); }
	if (this->chn != nullptr) { this->chn->get_headers(headers, full); }
	if (this->qnt != nullptr) { this->qnt->get_headers(headers, full); }
	if (this->mnt != nullptr) { this->mnt->get_headers(headers, full); }
	if (this->ter != nullptr) { this->ter->get_headers(headers, full); }
}

template <typename B, typename R, typename Q>
simulation::BFER_ite<B,R,Q>* BFER_ite::parameters
::build() const
{
#if defined(SYSTEMC)
	return new simulation::SC_BFER_ite<B,R,Q>(*this);
#else
	return new simulation::BFER_ite_threads<B,R,Q>(*this);
#endif
}

template <typename B, typename R, typename Q>
simulation::BFER_ite<B,R,Q>* BFER_ite
::build(const parameters &params)
{
	return params.template build<B,R,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::simulation::BFER_ite<B_8 ,R_8 ,Q_8 >* aff3ct::factory::BFER_ite::build<B_8 ,R_8 ,Q_8 >(const aff3ct::factory::BFER_ite::parameters&);
template aff3ct::simulation::BFER_ite<B_16,R_16,Q_16>* aff3ct::factory::BFER_ite::build<B_16,R_16,Q_16>(const aff3ct::factory::BFER_ite::parameters&);
template aff3ct::simulation::BFER_ite<B_32,R_32,Q_32>* aff3ct::factory::BFER_ite::build<B_32,R_32,Q_32>(const aff3ct::factory::BFER_ite::parameters&);
template aff3ct::simulation::BFER_ite<B_64,R_64,Q_64>* aff3ct::factory::BFER_ite::build<B_64,R_64,Q_64>(const aff3ct::factory::BFER_ite::parameters&);
#else
template aff3ct::simulation::BFER_ite<B,R,Q>* aff3ct::factory::BFER_ite::build<B,R,Q>(const aff3ct::factory::BFER_ite::parameters&);
#endif
// ==================================================================================== explicit template instantiation

