#include <utility>

#include "Tools/Documentation/documentation.h"
#include "Module/Decoder/Generic/ML/Decoder_maximum_likelihood_std.hpp"
#include "Module/Decoder/Generic/ML/Decoder_maximum_likelihood_naive.hpp"
#include "Module/Decoder/Generic/Chase/Decoder_chase_std.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_name   = "Decoder";
const std::string aff3ct::factory::Decoder_prefix = "dec";

Decoder
::Decoder(const std::string &name, const std::string &prefix)
: Factory(name, Decoder_name, prefix)
{
}

Decoder* Decoder
::clone() const
{
	return new Decoder(*this);
}

void Decoder
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder::";

	tools::add_arg(args, p, class_name+"p+cw-size,N",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+fra,F",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+type,D",
		cli::Text(cli::Including_set("ML", "CHASE")));

	tools::add_arg(args, p, class_name+"p+implem",
		cli::Text(cli::Including_set("STD", "NAIVE")));

	tools::add_arg(args, p, class_name+"p+hamming",
		cli::None());

	tools::add_arg(args, p, class_name+"p+flips",
		cli::Integer(cli::Positive()));

	tools::add_arg(args, p, class_name+"p+seed",
		cli::Integer(cli::Positive()));
}

void Decoder
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K          = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-cw-size",   "N"})) this->N_cw       = vals.to_int({p+"-cw-size",   "N"});
	if(vals.exist({p+"-fra",       "F"})) this->n_frames   = vals.to_int({p+"-fra",       "F"});
	if(vals.exist({p+"-flips"         })) this->flips      = vals.to_int({p+"-flips"         });
	if(vals.exist({p+"-seed"          })) this->seed       = vals.to_int({p+"-seed"          });
	if(vals.exist({p+"-type",      "D"})) this->type       = vals.at    ({p+"-type",      "D"});
	if(vals.exist({p+"-implem"        })) this->implem     = vals.at    ({p+"-implem"        });
	if(vals.exist({p+"-no-sys"        })) this->systematic = false;
	if(vals.exist({p+"-hamming"       })) this->hamming    = true;

	this->R = (float)this->K / (float)this->N_cw;
}

void Decoder
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
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

	if (full) headers[p].push_back(std::make_pair("Seed", std::to_string(this->seed)));
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder
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
			if (this->implem == "STD") return new module::Decoder_chase_std<B,Q>(this->K, this->N_cw, *encoder, this->flips, this->hamming, this->n_frames);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder::build<B_8 ,Q_8 >(module::Encoder<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder::build<B_16,Q_16>(module::Encoder<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder::build<B_32,Q_32>(module::Encoder<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder::build<B_64,Q_64>(module::Encoder<B_64>*) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder::build<B,Q>(module::Encoder<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
