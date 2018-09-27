#include <thread>

#include "BFER.hpp"
#include "Tools/Math/utils.h"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER_name   = "Simulation BFER";
const std::string aff3ct::factory::BFER_prefix = "sim";

BFER::parameters
::parameters(const std::string &name, const std::string &prefix)
: Simulation::parameters(name, prefix)
{
}

BFER::parameters* BFER::parameters
::clone() const
{
	return new BFER::parameters(*this);

	// if (src != nullptr) { clone->src = src->clone(); }
	// if (crc != nullptr) { clone->crc = crc->clone(); }
	// if (cdc != nullptr) { clone->cdc = cdc->clone(); }
	// if (mdm != nullptr) { clone->mdm = mdm->clone(); }
	// if (chn != nullptr) { clone->chn = chn->clone(); }
	// if (qnt != nullptr) { clone->qnt = qnt->clone(); }
	// if (mnt_mi != nullptr) { clone->mnt_mi = mnt_mi->clone(); }
	// if (mnt_er != nullptr) { clone->mnt_er = mnt_er->clone(); }
	// if (ter != nullptr) { clone->ter = ter->clone(); }

	// return clone;
}

std::vector<std::string> BFER::parameters
::get_names() const
{
	auto n = Simulation::parameters::get_names();
	if (src != nullptr) { auto nn = src->get_names(); for (auto &x : nn) n.push_back(x); }
	if (crc != nullptr) { auto nn = crc->get_names(); for (auto &x : nn) n.push_back(x); }
	if (cdc != nullptr) { auto nn = cdc->get_names(); for (auto &x : nn) n.push_back(x); }
	if (mdm != nullptr) { auto nn = mdm->get_names(); for (auto &x : nn) n.push_back(x); }
	if (chn != nullptr) { auto nn = chn->get_names(); for (auto &x : nn) n.push_back(x); }
	if (qnt != nullptr) { auto nn = qnt->get_names(); for (auto &x : nn) n.push_back(x); }
	if (mnt_mi != nullptr) { auto nn = mnt_mi->get_names(); for (auto &x : nn) n.push_back(x); }
	if (mnt_er != nullptr) { auto nn = mnt_er->get_names(); for (auto &x : nn) n.push_back(x); }
	if (ter != nullptr) { auto nn = ter->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> BFER::parameters
::get_short_names() const
{
	auto sn = Factory::parameters::get_short_names();
	if (src != nullptr) { auto nn = src->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (crc != nullptr) { auto nn = crc->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (cdc != nullptr) { auto nn = cdc->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (mdm != nullptr) { auto nn = mdm->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (chn != nullptr) { auto nn = chn->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (qnt != nullptr) { auto nn = qnt->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (mnt_mi != nullptr) { auto nn = mnt_mi->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (mnt_er != nullptr) { auto nn = mnt_er->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (ter != nullptr) { auto nn = ter->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> BFER::parameters
::get_prefixes() const
{
	auto p = Factory::parameters::get_prefixes();
	if (src != nullptr) { auto nn = src->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (crc != nullptr) { auto nn = crc->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (cdc != nullptr) { auto nn = cdc->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (mdm != nullptr) { auto nn = mdm->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (chn != nullptr) { auto nn = chn->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (qnt != nullptr) { auto nn = qnt->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (mnt_mi != nullptr) { auto nn = mnt_mi->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (mnt_er != nullptr) { auto nn = mnt_er->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (ter != nullptr) { auto nn = ter->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void BFER::parameters
::get_description(tools::Argument_map_info &args) const
{
	Simulation::parameters::get_description(args);

	auto p = this->get_prefix();

	args.add(
		{p+"-coset", "c"},
		tools::None(),
		"enable the coset approach.");

	args.add(
		{p+"-err-trk"},
		tools::None(),
		"enable the tracking of the bad frames (by default the frames are stored in the current folder).",
		tools::arg_rank::ADV);

	args.add(
		{p+"-err-trk-rev"},
		tools::None(),
		"automatically replay the saved frames.",
		tools::arg_rank::ADV);

	args.add(
		{p+"-err-trk-path"},
		tools::File(tools::openmode::read_write),
		"base path for the files where the bad frames will be stored or read.",
		tools::arg_rank::ADV);

	args.add(
		{p+"-err-trk-thold"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"dump only frames with a bit error count above or equal to this threshold.",
		tools::arg_rank::ADV);

	args.add(
		{p+"-coded"},
		tools::None(),
		"enable the coded monitoring (extends the monitored bits to the entire codeword).");


	auto pmon = mnt_er->get_prefix();

	args.add(
		{pmon+"-mutinfo"},
		tools::None(),
		"allow the computation of the mutual information.");
}

void BFER::parameters
::store(const tools::Argument_map_value &vals)
{
#if !defined(SYSTEMC)
	this->n_threads = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;
#endif

	Simulation::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-err-trk-path"   })) this->err_track_path      = vals.at    ({p+"-err-trk-path"   });
	if(vals.exist({p+"-err-trk-thold"  })) this->err_track_threshold = vals.to_int({p+"-err-trk-thold"  });
	if(vals.exist({p+"-err-trk-rev"    })) this->err_track_revert    = true;
	if(vals.exist({p+"-err-trk"        })) this->err_track_enable    = true;
	if(vals.exist({p+"-coset",      "c"})) this->coset               = true;
	if(vals.exist({p+"-coded",         })) this->coded_monitoring    = true;

	if (this->err_track_revert)
	{
		this->err_track_enable = false;
		this->n_threads = 1;
	}

	auto pmon = mnt_er->get_prefix();

	if(vals.exist({pmon+"-mutinfo"})) this->mutinfo = true;
}

void BFER::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Simulation::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Coset approach (c)", this->coset ? "yes" : "no"));
	headers[p].push_back(std::make_pair("Coded monitoring", this->coded_monitoring ? "yes" : "no"));

	std::string enable_track = (this->err_track_enable) ? "on" : "off";
	headers[p].push_back(std::make_pair("Bad frames tracking", enable_track));

	std::string enable_rev_track = (this->err_track_revert) ? "on" : "off";
	headers[p].push_back(std::make_pair("Bad frames replay", enable_rev_track));

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

	if (this->src != nullptr)
		headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->src->n_frames)));

	if (this->src != nullptr) { this->src->get_headers(headers, full); }
	if (this->crc != nullptr) { this->crc->get_headers(headers, full); }
	if (this->cdc != nullptr) { this->cdc->get_headers(headers, full); }
	if (this->mdm != nullptr) { this->mdm->get_headers(headers, full); }
	if (this->chn != nullptr) { this->chn->get_headers(headers, full); }
	if (this->qnt != nullptr) { this->qnt->get_headers(headers, full); }

	if (this->mnt_er != nullptr) { this->mnt_er->get_headers(headers, full); }

	headers[this->mnt_er->get_prefix()].push_back(std::make_pair("Compute mutual info", this->mutinfo ? "yes" : "no"));
	if (this->mutinfo)
		if (this->mnt_er != nullptr) { this->mnt_er->get_headers(headers, full); }

	if (this->ter != nullptr) { this->ter->get_headers(headers, full); }
}


void BFER::parameters
::set_src(Source::parameters *src)
{
	this->src.reset(src);
}

void BFER::parameters
::set_crc(CRC::parameters *crc)
{
	this->crc.reset(crc);
}

void BFER::parameters
::set_cdc(Codec::parameters *cdc)
{
	this->cdc.reset(cdc);
}

void BFER::parameters
::set_mdm(Modem::parameters *mdm)
{
	this->mdm.reset(mdm);
}

void BFER::parameters
::set_chn(Channel::parameters *chn)
{
	this->chn.reset(chn);
}

void BFER::parameters
::set_qnt(Quantizer::parameters *qnt)
{
	this->qnt.reset(qnt);
}

void BFER::parameters
::set_mnt_mi(Monitor_MI::parameters *mnt)
{
	this->mnt_mi.reset(mnt);
}

void BFER::parameters
::set_mnt_er(Monitor_BFER::parameters *mnt)
{
	this->mnt_er.reset(mnt);
}

void BFER::parameters
::set_ter(Terminal::parameters *ter)
{
	this->ter.reset(ter);
}

const Codec::parameters* BFER::parameters
::get_cdc() const
{
	return this->cdc.get();
}

