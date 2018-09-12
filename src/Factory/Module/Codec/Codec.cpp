#include <algorithm>

#include "Tools/general_utils.h"
#include "Tools/Math/utils.h"

#include "Codec.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_name   = "Codec";
const std::string aff3ct::factory::Codec_prefix = "cdc";

Codec::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Codec_name, Codec_name, prefix)
{
}

Codec::parameters
::parameters(const std::string &name, const std::string &prefix)
: Factory::parameters(name, Codec_name, prefix)
{
}

Codec::parameters* Codec::parameters
::clone() const
{
	return new Codec::parameters(*this);
}

void Codec::parameters
::enable_puncturer()
{
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "This codec does not support to be punctured.");
}

std::vector<std::string> Codec::parameters
::get_names() const
{
	auto n = Factory::parameters::get_names();
	if (enc != nullptr) { auto nn = enc->get_names(); for (auto &x : nn) n.push_back(x); }
	if (dec != nullptr) { auto nn = dec->get_names(); for (auto &x : nn) n.push_back(x); }
	if (pct != nullptr) { auto nn = pct->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> Codec::parameters
::get_short_names() const
{
	auto sn = Factory::parameters::get_short_names();
	if (enc != nullptr) { auto nn = enc->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (dec != nullptr) { auto nn = dec->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (pct != nullptr) { auto nn = pct->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Codec::parameters
::get_prefixes() const
{
	auto p = Factory::parameters::get_prefixes();
	if (enc != nullptr) { auto nn = enc->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (dec != nullptr) { auto nn = dec->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (pct != nullptr) { auto nn = pct->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void Codec::parameters
::get_description(tools::Argument_map_info &args) const
{
}

void Codec::parameters
::store(const tools::Argument_map_value &vals)
{
}

void Codec::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
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

void Codec::parameters
::set_enc(Encoder::parameters *enc)
{
	this->enc.reset(enc);
}

void Codec::parameters
::set_dec(Decoder::parameters *dec)
{
	this->dec.reset(dec);
}

void Codec::parameters
::set_pct(Puncturer::parameters *pct)
{
	this->pct.reset(pct);
}

void Codec::parameters
::set_itl(Interleaver::parameters *itl)
{
	this->itl.reset(itl);
}


void Codec::parameters
::set_enc(tools::auto_cloned_unique_ptr<Encoder::parameters>&& enc)
{
	this->enc = std::move(enc);
}

void Codec::parameters
::set_dec(tools::auto_cloned_unique_ptr<Decoder::parameters>&& dec)
{
	this->dec = std::move(dec);
}

void Codec::parameters
::set_pct(tools::auto_cloned_unique_ptr<Puncturer::parameters>&& pct)
{
	this->pct = std::move(pct);
}

void Codec::parameters
::set_itl(tools::auto_cloned_unique_ptr<Interleaver::parameters>&& itl)
{
	this->itl = std::move(itl);
}
