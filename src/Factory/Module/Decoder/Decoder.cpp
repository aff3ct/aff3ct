#include "Module/Decoder/ML/Decoder_maximum_likelihood_std.hpp"
#include "Module/Decoder/ML/Decoder_maximum_likelihood_naive.hpp"
#include "Module/Decoder/ML/Decoder_chase_naive.hpp"

#include "Decoder.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_name   = "Decoder";
const std::string aff3ct::factory::Decoder_prefix = "dec";

Decoder::parameters
::parameters(const std::string &name, const std::string &prefix)
: Factory::parameters(name, Decoder_name, prefix)
{
}

Decoder::parameters
::~parameters()
{
}

Decoder::parameters* Decoder::parameters
::clone() const
{
	return new Decoder::parameters(*this);
}

void Decoder::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	auto p = this->get_prefix();

	req_args[{p+"-info-bits", "K"}] =
		{"strictly_positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{p+"-cw-size", "N"}] =
		{"strictly_positive_int",
		 "the codeword size."};

	opt_args[{p+"-fra", "F"}] =
		{"strictly_positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-type", "D"}] =
		{"string",
		 "select the algorithm you want to decode the codeword.",
		 "ML, CHASE"};

	opt_args[{p+"-implem"}] =
		{"string",
		 "select the implementation of the algorithm to decode.",
		 "STD, NAIVE"};

	opt_args[{p+"-hamming"}] =
		{"",
		 "enable the computation of the Hamming distance instead of the Euclidean distance in the ML/CHASE decoders."};

	opt_args[{p+"-flips"}] =
		{"strictly_positive_int",
		 "set the maximum number of flips in the CHASE decoder."};
}

void Decoder::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-info-bits", "K"})) this->K          = std::stoi(vals.at({p+"-info-bits", "K"}));
	if(exist(vals, {p+"-cw-size",   "N"})) this->N_cw       = std::stoi(vals.at({p+"-cw-size",   "N"}));
	if(exist(vals, {p+"-fra",       "F"})) this->n_frames   = std::stoi(vals.at({p+"-fra",       "F"}));
	if(exist(vals, {p+"-flips"         })) this->flips      = std::stoi(vals.at({p+"-flips"         }));
	if(exist(vals, {p+"-type",      "D"})) this->type       =           vals.at({p+"-type",      "D"});
	if(exist(vals, {p+"-implem"        })) this->implem     =           vals.at({p+"-implem"        });
	if(exist(vals, {p+"-no-sys"        })) this->systematic = false;
	if(exist(vals, {p+"-hamming"       })) this->hamming    = true;

	this->R = (float)this->K / (float)this->N_cw;
}

void Decoder::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type (D)",this->type));
	if(this->implem.size()) headers[p].push_back(std::make_pair("Implementation", this->implem));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Codeword size (N)", std::to_string(this->N_cw)));
	if (full) headers[p].push_back(std::make_pair("Code rate (R)", std::to_string(this->R)));
	headers[p].push_back(std::make_pair("Systematic", ((this->systematic) ? "yes" : "no")));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
	if(this->type == "ML" || this->type == "CHASE") 
		headers[p].push_back(std::make_pair("Distance", this->hamming ? "Hamming" : "Euclidean"));
	if(this->type == "CHASE")
		headers[p].push_back(std::make_pair("Max flips", std::to_string(this->flips)));
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder::parameters
::build(module::Encoder<B> *encoder) const
{
	if (encoder)
	{
		if (this->type == "ML") 
		{
			if (this->implem == "STD"  ) return new module::Decoder_ML_std  <B,Q>(this->K, this->N_cw, *encoder, this->hamming, this->n_frames);
			if (this->implem == "NAIVE") return new module::Decoder_ML_naive<B,Q>(this->K, this->N_cw, *encoder, this->hamming, this->n_frames);
		}
		else if (this->type == "CHASE")
		{
			if (this->implem == "NAIVE") return new module::Decoder_chase_naive<B,Q>(this->K, this->N_cw, *encoder, this->flips, this->hamming, this->n_frames);
		}
	}
	
	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder::parameters::build<B_8 ,Q_8 >(module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder::parameters::build<B_16,Q_16>(module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder::parameters::build<B_32,Q_32>(module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder::parameters::build<B_64,Q_64>(module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder::parameters::build<B,Q>(module::Encoder<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
