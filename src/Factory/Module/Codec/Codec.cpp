#include "Codec.hpp"

using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec::name   = "Codec";
const std::string aff3ct::factory::Codec::prefix = "cdc";

Codec::parameters
::parameters(const std::string prefix)
: Factory::parameters(Codec::name, Codec::name, prefix)
{
}

Codec::parameters
::parameters(const std::string name, const std::string prefix)
: Factory::parameters(name, Codec::name, prefix)
{
}

Codec::parameters
::~parameters()
{
}

Codec::parameters* Codec::parameters
::clone() const
{
	return new Codec::parameters(*this);
}

void Codec::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
}

void Codec::parameters
::store(const arg_val_map &vals)
{
}

void Codec::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
}

void Codec::parameters
::set_enc(Encoder::parameters *enc)
{
	this->enc = enc;
}

void Codec::parameters
::set_dec(Decoder::parameters *dec)
{
	this->dec = dec;
}

void Codec::parameters
::set_itl(Interleaver::parameters *itl)
{
	this->itl = itl;
}
