#include <sstream>
#include <utility>
#include <thread>

#include "Tools/Documentation/documentation.h"
#include "Tools/Math/utils.h"
#include "Factory/Simulation/BFER/BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER_name   = "Simulation BFER";
const std::string aff3ct::factory::BFER_prefix = "sim";

BFER
::BFER(const std::string &name, const std::string &prefix)
: Simulation(name, prefix)
{
}

BFER* BFER
::clone() const
{
	return new BFER(*this);
}

std::vector<std::string> BFER
::get_names() const
{
	auto n = Simulation::get_names();
	if (src    != nullptr) { auto nn = src   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (crc    != nullptr) { auto nn = crc   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (cdc    != nullptr) { auto nn = cdc   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (mdm    != nullptr) { auto nn = mdm   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (chn    != nullptr) { auto nn = chn   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (qnt    != nullptr) { auto nn = qnt   ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (mnt_mi != nullptr) { auto nn = mnt_mi->get_names(); for (auto &x : nn) n.push_back(x); }
	if (mnt_er != nullptr) { auto nn = mnt_er->get_names(); for (auto &x : nn) n.push_back(x); }
	if (ter    != nullptr) { auto nn = ter   ->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> BFER
::get_short_names() const
{
	auto sn = Factory::get_short_names();
	if (src    != nullptr) { auto nn = src   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (crc    != nullptr) { auto nn = crc   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (cdc    != nullptr) { auto nn = cdc   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (mdm    != nullptr) { auto nn = mdm   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (chn    != nullptr) { auto nn = chn   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (qnt    != nullptr) { auto nn = qnt   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (mnt_mi != nullptr) { auto nn = mnt_mi->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (mnt_er != nullptr) { auto nn = mnt_er->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (ter    != nullptr) { auto nn = ter   ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> BFER
::get_prefixes() const
{
	auto p = Factory::get_prefixes();
	if (src    != nullptr) { auto nn = src   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (crc    != nullptr) { auto nn = crc   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (cdc    != nullptr) { auto nn = cdc   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (mdm    != nullptr) { auto nn = mdm   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (chn    != nullptr) { auto nn = chn   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (qnt    != nullptr) { auto nn = qnt   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (mnt_mi != nullptr) { auto nn = mnt_mi->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (mnt_er != nullptr) { auto nn = mnt_er->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (ter    != nullptr) { auto nn = ter   ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void BFER
::get_description(cli::Argument_map_info &args) const
{
	Simulation::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::BFER::";

	tools::add_arg(args, p, class_name+"p+coset,c",
		cli::None());

	tools::add_arg(args, p, class_name+"p+err-trk",
		cli::None(),
		cli::arg_rank::ADV);

	tools::add_arg(args, p, class_name+"p+err-trk-rev",
		cli::None(),
		cli::arg_rank::ADV);

	tools::add_arg(args, p, class_name+"p+err-trk-path",
		cli::File(cli::openmode::read_write),
		cli::arg_rank::ADV);

	tools::add_arg(args, p, class_name+"p+err-trk-thold",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::ADV);

	tools::add_arg(args, p, class_name+"p+coded",
		cli::None());

	auto pter = ter->get_prefix();

	tools::add_arg(args, pter, class_name+"p+sigma",
		cli::None());

	auto pmnt = mnt_er->get_prefix();

	tools::add_arg(args, pmnt, class_name+"p+mutinfo",
		cli::None());

#ifdef AFF3CT_MPI
	tools::add_arg(args, pmnt, class_name+"p+mpi-comm-freq",
		cli::Integer(cli::Positive(), cli::Non_zero()));
#else
	tools::add_arg(args, pmnt, class_name+"p+red-lazy",
		cli::None());

	tools::add_arg(args, pmnt, class_name+"p+red-lazy-freq",
		cli::Integer(cli::Positive(), cli::Non_zero()));
#endif

	tools::add_arg(args, p, class_name+"p+sequence-path",
		cli::File(cli::openmode::write),
		cli::arg_rank::ADV);
}

void BFER
::store(const cli::Argument_map_value &vals)
{
	using namespace std::chrono;

	this->n_threads = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;
	Simulation::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-err-trk-path" })) this->err_track_path      = vals.at    ({p+"-err-trk-path" });
	if(vals.exist({p+"-err-trk-thold"})) this->err_track_threshold = vals.to_int({p+"-err-trk-thold"});
	if(vals.exist({p+"-err-trk-rev"  })) this->err_track_revert    = true;
	if(vals.exist({p+"-err-trk"      })) this->err_track_enable    = true;
	if(vals.exist({p+"-coset",    "c"})) this->coset               = true;
	if(vals.exist({p+"-coded",       })) this->coded_monitoring    = true;

	if (vals.exist({p+"-sequence-path"}))
		this->sequence_path = vals.at({p+"-sequence-path"});

	if (this->err_track_revert)
	{
		this->err_track_enable = false;
		this->n_threads = 1;
	}

	auto pter = ter->get_prefix();

	if(vals.exist({pter+"-sigma"})) this->ter_sigma = true;

	auto pmnt = mnt_er->get_prefix();

	if(vals.exist({pmnt+"-mutinfo"})) this->mnt_mutinfo = true;

#ifdef AFF3CT_MPI
	if(vals.exist({pmnt+"-mpi-comm-freq"})) this->mnt_mpi_comm_freq = milliseconds(vals.to_int({pmnt+"-mpi-comm-freq"}));
#else
	if(vals.exist({pmnt+"-red-lazy"})) this->mnt_red_lazy = true;
	if(vals.exist({pmnt+"-red-lazy-freq"}))
	{
		this->mnt_red_lazy = true;
		this->mnt_red_lazy_freq = milliseconds(vals.to_int({pmnt+"-red-lazy-freq"}));
	}
#endif
}

void BFER
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Simulation::get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->noise.get() != nullptr && (this->noise.get()->type == "EBN0" || this->noise.get()->type == "ESN0"))
	{
		std::string pter = ter->get_prefix();
		headers[pter].push_back(std::make_pair("Show Sigma", this->ter_sigma ? "on" : "off"));
	}

	std::string pmnt = mnt_er->get_prefix();
#ifdef AFF3CT_MPI
	headers[pmnt].push_back(std::make_pair("MPI comm. freq. (ms)", std::to_string(this->mnt_mpi_comm_freq.count())));
#else
	headers[pmnt].push_back(std::make_pair("Lazy reduction", this->mnt_red_lazy ? "on" : "off"));
	if (this->mnt_red_lazy)
		headers[pmnt].push_back(std::make_pair("Lazy reduction freq. (ms)",
		                                       std::to_string(this->mnt_red_lazy_freq.count())));
#endif

	headers[p].push_back(std::make_pair("Coset approach (c)", this->coset ? "yes" : "no"));
	headers[p].push_back(std::make_pair("Coded monitoring", this->coded_monitoring ? "yes" : "no"));

	std::string enable_track = (this->err_track_enable) ? "on" : "off";
	headers[p].push_back(std::make_pair("Bad frames tracking", enable_track));

	std::string enable_rev_track = (this->err_track_revert) ? "on" : "off";
	headers[p].push_back(std::make_pair("Bad frames replay", enable_rev_track));

	if (!this->sequence_path.empty())
		headers[p].push_back(std::make_pair("Path export sequence (dot)", this->sequence_path));

	if (this->err_track_threshold)
		headers[p].push_back(std::make_pair("Bad frames threshold", std::to_string(this->err_track_threshold)));

	if (this->err_track_enable || this->err_track_revert)
	{
		std::string path = this->err_track_path + std::string("_$noise.[src,enc,chn]");
		headers[p].push_back(std::make_pair("Bad frames base path", path));
	}

	if (this->src != nullptr && this->cdc != nullptr)
	{
		const auto bit_rate = (float)this->src->K / (float)this->cdc->N;
		// find the greatest common divisor of K and N
		auto gcd = tools::greatest_common_divisor(this->src->K, this->cdc->N);
		std::stringstream br_str;
		br_str << bit_rate << " (" << this->src->K/gcd << "/" << this->cdc->N/gcd << ")";

		headers[p].push_back(std::make_pair("Bit rate", br_str.str()));
	}

	if (this->src != nullptr) { this->src->get_headers(headers, full); }
	if (this->crc != nullptr) { this->crc->get_headers(headers, full); }
	if (this->cdc != nullptr) { this->cdc->get_headers(headers, full); }
	if (this->mdm != nullptr) { this->mdm->get_headers(headers, full); }
	if (this->chn != nullptr) { this->chn->get_headers(headers, full); }
	if (this->qnt != nullptr) { this->qnt->get_headers(headers, full); }

	if (this->mnt_er != nullptr) { this->mnt_er->get_headers(headers, full); }

	headers[this->mnt_er->get_prefix()].push_back(std::make_pair("Compute mutual info",
	                                                             this->mnt_mutinfo ? "yes" : "no"));
	if (this->mnt_mutinfo)
		if (this->mnt_er != nullptr) { this->mnt_er->get_headers(headers, full); }

	if (this->ter != nullptr) { this->ter->get_headers(headers, full); }
}

void BFER
::set_src(Source *src)
{
	this->src.reset(src);
}

void BFER
::set_crc(CRC *crc)
{
	this->crc.reset(crc);
}

void BFER
::set_cdc(Codec *cdc)
{
	this->cdc.reset(cdc);
}

void BFER
::set_mdm(Modem *mdm)
{
	this->mdm.reset(mdm);
}

void BFER
::set_chn(Channel *chn)
{
	this->chn.reset(chn);
}

void BFER
::set_qnt(Quantizer *qnt)
{
	this->qnt.reset(qnt);
}

void BFER
::set_mnt_mi(Monitor_MI *mnt)
{
	this->mnt_mi.reset(mnt);
}

void BFER
::set_mnt_er(Monitor_BFER *mnt)
{
	this->mnt_er.reset(mnt);
}

void BFER
::set_ter(Terminal *ter)
{
	this->ter.reset(ter);
}

const Codec* BFER
::get_cdc() const
{
	return this->cdc.get();
}

