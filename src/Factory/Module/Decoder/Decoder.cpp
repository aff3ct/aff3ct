#include "Tools/Documentation/documentation.h"

#include "Module/Decoder/Generic/ML/Decoder_maximum_likelihood_std.hpp"
#include "Module/Decoder/Generic/ML/Decoder_maximum_likelihood_naive.hpp"
#include "Module/Decoder/Generic/Chase/Decoder_chase_std.hpp"

#include "Factory/Module/Decoder/Decoder.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_name   = "Decoder";
const std::string aff3ct::factory::Decoder_prefix = "dec";

Decoder::parameters
::parameters(const std::string &name, const std::string &prefix)
: Factory::parameters(name, Decoder_name, prefix)
{
}

Decoder::parameters* Decoder::parameters
::clone() const
{
	return new Decoder::parameters(*this);
}

void Decoder::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder::parameters::";

	tools::add_arg(args, p, class_name+"p+cw-size,N",
		tools::Integer(tools::Positive(), tools::Non_zero()),
		tools::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		tools::Integer(tools::Positive(), tools::Non_zero()),
		tools::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+fra,F",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+type,D",
		tools::Text(tools::Including_set("ML", "CHASE")));

	tools::add_arg(args, p, class_name+"p+implem",
		tools::Text(tools::Including_set("STD", "NAIVE")));

	tools::add_arg(args, p, class_name+"p+hamming",
		tools::None());

	tools::add_arg(args, p, class_name+"p+flips",
		tools::Integer(tools::Positive()));

	tools::add_arg(args, p, class_name+"p+seed",
		tools::Integer(tools::Positive()));
}

void Decoder::parameters
::store(const tools::Argument_map_value &vals)
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

	if (full) headers[p].push_back(std::make_pair("Seed", std::to_string(this->seed)));
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder::parameters
::build(const std::unique_ptr<module::Encoder<B>>& encoder) const
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

template <typename B, typename Q>
module::Decoder_SIHO_HIHO<B,Q>* Decoder::parameters
::build_hiho(const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	if (encoder)
	{
		if (this->type == "ML")
		{
			if (this->implem == "STD"  ) return new module::Decoder_ML_std  <B,Q>(this->K, this->N_cw, *encoder, this->hamming, this->n_frames);
			if (this->implem == "NAIVE") return new module::Decoder_ML_naive<B,Q>(this->K, this->N_cw, *encoder, this->hamming, this->n_frames);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder::parameters::build<B_8 ,Q_8 >(const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder::parameters::build<B_16,Q_16>(const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder::parameters::build<B_32,Q_32>(const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder::parameters::build<B_64,Q_64>(const std::unique_ptr<module::Encoder<B_64>>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder::parameters::build_hiho<B_8 ,Q_8 >(const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_16,Q_16>* aff3ct::factory::Decoder::parameters::build_hiho<B_16,Q_16>(const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_32,Q_32>* aff3ct::factory::Decoder::parameters::build_hiho<B_32,Q_32>(const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B_64,Q_64>* aff3ct::factory::Decoder::parameters::build_hiho<B_64,Q_64>(const std::unique_ptr<module::Encoder<B_64>>&) const;
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder::parameters::build<B,Q>(const std::unique_ptr<module::Encoder<B>>&) const;
template aff3ct::module::Decoder_SIHO_HIHO<B,Q>* aff3ct::factory::Decoder::parameters::build_hiho<B,Q>(const std::unique_ptr<module::Encoder<B>>&) const;
#endif
// ==================================================================================== explicit template instantiation
