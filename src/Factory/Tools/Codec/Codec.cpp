#include <algorithm>
#include <sstream>
#include <utility>

#include "Tools/general_utils.h"
#include "Tools/Math/utils.h"
#include "Factory/Tools/Codec/Codec.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_name   = "Codec";
const std::string aff3ct::factory::Codec_prefix = "cdc";

Codec
::Codec(const std::string &prefix)
: Factory(Codec_name, Codec_name, prefix)
{
}

Codec
::Codec(const std::string &name, const std::string &prefix)
: Factory(name, Codec_name, prefix)
{
}

Codec* Codec
::clone() const
{
	return new Codec(*this);
}

void Codec
::enable_puncturer()
{
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "This codec does not support to be punctured.");
}

std::vector<std::string> Codec
::get_names() const
{
	auto n = Factory::get_names();
	if (enc != nullptr) { auto nn = enc->get_names(); for (auto &x : nn) n.push_back(x); }
	if (dec != nullptr) { auto nn = dec->get_names(); for (auto &x : nn) n.push_back(x); }
	if (pct != nullptr) { auto nn = pct->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> Codec
::get_short_names() const
{
	auto sn = Factory::get_short_names();
	if (enc != nullptr) { auto nn = enc->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (dec != nullptr) { auto nn = dec->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (pct != nullptr) { auto nn = pct->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Codec
::get_prefixes() const
{
	auto p = Factory::get_prefixes();
	if (enc != nullptr) { auto nn = enc->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (dec != nullptr) { auto nn = dec->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (pct != nullptr) { auto nn = pct->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void Codec
::get_description(cli::Argument_map_info &args) const
{
}

void Codec
::store(const cli::Argument_map_value &vals)
{
}

void Codec
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();
	auto v = tools::split(this->get_name(), ' ');
	auto name = v.size() >= 2 ? v[1] : "UNKNOWN";
	for (size_t i = 2; i < v.size(); i++)
		name += "_" + v[i];
	std::transform(name.begin(), name.end(), name.begin(), toupper);

	headers[p].push_back(std::make_pair("Type",                                name            ));
	headers[p].push_back(std::make_pair("Info. bits (K)",       std::to_string(this->K        )));
	headers[p].push_back(std::make_pair("Codeword size (N_cw)", std::to_string(this->N_cw     )));
	headers[p].push_back(std::make_pair("Frame size (N)",       std::to_string(this->N        )));


	const auto code_rate = (float)this->K / (float)this->N;
	// find the greatest common divisor of K and N
	auto gcd = tools::greatest_common_divisor(this->K, this->N);
	std::stringstream cr_str;
	cr_str << code_rate << " (" << this->K/gcd << "/" << this->N/gcd << ")";

	headers[p].push_back(std::make_pair("Code rate", cr_str.str()));
}

void Codec
::set_enc(Encoder *enc)
{
	this->enc.reset(enc);
}

void Codec
::set_dec(Decoder *dec)
{
	this->dec.reset(dec);
}

void Codec
::set_pct(Puncturer *pct)
{
	this->pct.reset(pct);
}

void Codec
::set_itl(Interleaver *itl)
{
	this->itl.reset(itl);
}


void Codec
::set_enc(tools::auto_cloned_unique_ptr<Encoder>&& enc)
{
	this->enc = std::move(enc);
}

void Codec
::set_dec(tools::auto_cloned_unique_ptr<Decoder>&& dec)
{
	this->dec = std::move(dec);
}

void Codec
::set_pct(tools::auto_cloned_unique_ptr<Puncturer>&& pct)
{
	this->pct = std::move(pct);
}

void Codec
::set_itl(tools::auto_cloned_unique_ptr<Interleaver>&& itl)
{
	this->itl = std::move(itl);
}
