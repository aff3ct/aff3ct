#include "Tools/Exception/exception.hpp"
#include "Tools/Code/SCMA/modem_SCMA_functions.hpp"

#include "Module/Modem/OOK/Modem_OOK.hpp"
#include "Module/Modem/BPSK/Modem_BPSK.hpp"
#include "Module/Modem/BPSK/Modem_BPSK_fast.hpp"
#include "Module/Modem/PAM/Modem_PAM.hpp"
#include "Module/Modem/QAM/Modem_QAM.hpp"
#include "Module/Modem/PSK/Modem_PSK.hpp"
#include "Module/Modem/CPM/Modem_CPM.hpp"
#include "Module/Modem/SCMA/Modem_SCMA.hpp"
#include "Module/Modem/User/Modem_user.hpp"

#include "Modem.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Modem_name   = "Modem";
const std::string aff3ct::factory::Modem_prefix = "mdm";

Modem::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Modem_name, Modem_name, prefix)
{
}

Modem::parameters
::~parameters()
{
}

Modem::parameters* Modem::parameters
::clone() const
{
	return new Modem::parameters(*this);
}

void Modem::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();

	// ----------------------------------------------------------------------------------------------------- modulator
	args.add(
		{p+"-fra-size", "N"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"number of symbols by frame.",
		tools::arg_rank::REQ);

	args.add(
		{p+"-fra", "F"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"set the number of inter frame level to process.");

	args.add(
		{p+"-type"},
		tools::Text(tools::Including_set("BPSK", "BPSK_FAST", "OOK", "PSK", "PAM", "QAM", "CPM", "USER", "SCMA")),
		"type of the modulation to use in the simulation.");

	args.add(
		{p+"-bps"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"select the number of bits per symbol (default is 1).");

	args.add(
		{p+"-ups"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"select the symbol sampling factor (default is 1).");

	args.add(
		{p+"-const-path"},
		tools::File(tools::openmode::read),
		"path to the ordered modulation symbols (constellation), to use with \"--mod-type USER\".");

	args.add(
		{p+"-cpm-std"},
		tools::Text(tools::Including_set("GSM")),
		"the selection of a default CPM standard hardly implemented (any of those parameters is "
		   "overwritten if the argument is given by the user)");

	args.add(
		{p+"-cpm-L"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"CPM pulse width or CPM memory (default is 2).");

	args.add(
		{p+"-cpm-k"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"modulation index numerator (default is 1).");

	args.add(
		{p+"-cpm-p"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"modulation index denominator (default is 2).");

	args.add(
		{p+"-cpm-map"},
		tools::Text(tools::Including_set("NATURAL", "GRAY")),
		"symbols mapping layout (default is NATURAL).");

	args.add(
		{p+"-cpm-ws"},
		tools::Text(tools::Including_set("GMSK", "REC", "RCOS")),
		"wave shape (default is GMSK).");


	// --------------------------------------------------------------------------------------------------- demodulator
	args.add(
		{p+"-max"},
		tools::Text(tools::Including_set("MAX", "MAXL", "MAXS", "MAXSS")),
		"select the type of the max operation to use in the demodulator.");

	args.add(
		{p+"-sigma"},
		tools::Real(tools::Positive(), tools::Non_zero()),
		"noise variance value for the demodulator.");

	args.add(
		{p+"-no-sig2"},
		tools::None(),
		"turn off the division by sigma square in the demodulator.");

	args.add(
		{p+"-psi"},
		tools::Text(tools::Including_set("PSI0", "PSI1", "PSI2", "PSI3")),
		"select the type of the psi function to use in the SCMA demodulator.");

	args.add(
		{p+"-ite"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"number of iteration in the demodulator.");
}

void Modem::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	// ----------------------------------------------------------------------------------------------------- modulator
	if(vals.exist({p+"-type"         })) this->type    = vals.at({p+"-type"   });
	if(vals.exist({p+"-cpm-std"      })) this->cpm_std = vals.at({p+"-cpm-std"});

	if (this->type == "CPM")
	{
		if (!this->cpm_std.empty())
		{
			if (this->cpm_std == "GSM")
			{
				this->cpm_L      = 3;
				this->cpm_k      = 1;
				this->cpm_p      = 2;
				this->bps        = 1;
				this->upf        = 5;
				this->mapping    = "NATURAL";
				this->wave_shape = "GMSK";
			}
			else
			{
				std::stringstream message;
				message << "Unknown CPM standard ('cpm_std' = " << this->cpm_std << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		}
	}

	if(vals.exist({p+"-fra-size", "N"})) this->N          = vals.to_int({p+"-fra-size", "N"});
	if(vals.exist({p+"-fra",      "F"})) this->n_frames   = vals.to_int({p+"-fra",      "F"});
	if(vals.exist({p+"-bps"          })) this->bps        = vals.to_int({p+"-bps"          });
	if(vals.exist({p+"-ups"          })) this->upf        = vals.to_int({p+"-ups"          });
	if(vals.exist({p+"-const-path"   })) this->const_path = vals.at    ({p+"-const-path"   });
	if(vals.exist({p+"-cpm-L"        })) this->cpm_L      = vals.to_int({p+"-cpm-L"        });
	if(vals.exist({p+"-cpm-p"        })) this->cpm_p      = vals.to_int({p+"-cpm-p"        });
	if(vals.exist({p+"-cpm-k"        })) this->cpm_k      = vals.to_int({p+"-cpm-k"        });
	if(vals.exist({p+"-cpm-map"      })) this->mapping    = vals.at    ({p+"-cpm-map"      });
	if(vals.exist({p+"-cpm-ws"       })) this->wave_shape = vals.at    ({p+"-cpm-ws"       });

	// force the number of bits per symbol to 1 when BPSK mod
	if (this->type == "BPSK" || this->type == "BPSK_FAST" || this->type == "OOK")
		this->bps = 1;
	// force the number of bits per symbol to 3 when SCMA mod
	if (this->type == "SCMA")
		this->bps = 3;

	this->complex = is_complex_mod(this->type, this->bps);

	this->N_mod = get_buffer_size_after_modulation(this->type,
	                                               this->N,
	                                               this->bps,
	                                               this->upf,
	                                               this->cpm_L,
	                                               this->cpm_p);

	this->N_fil = get_buffer_size_after_filtering (this->type,
	                                               this->N,
	                                               this->bps,
	                                               this->cpm_L,
	                                               this->cpm_p);

	// --------------------------------------------------------------------------------------------------- demodulator
	if(vals.exist({p+"-no-sig2"})) this->no_sig2 = true;
	if(vals.exist({p+"-sigma"  })) this->sigma   = vals.to_float({p+"-sigma"});
	if(vals.exist({p+"-ite"    })) this->n_ite   = vals.to_int  ({p+"-ite"  });
	if(vals.exist({p+"-max"    })) this->max     = vals.at      ({p+"-max"  });
	if(vals.exist({p+"-psi"    })) this->psi     = vals.at      ({p+"-psi"  });
}

void Modem::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	// ----------------------------------------------------------------------------------------------------- modulator
	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Frame size (N)", std::to_string(this->N)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
	if (this->type == "CPM")
	{
		if(this->cpm_std.size())
			headers[p].push_back(std::make_pair("CPM standard", this->cpm_std));

		headers[p].push_back(std::make_pair("CPM L memory", std::to_string(this->cpm_L)));
		headers[p].push_back(std::make_pair("CPM h index", (std::to_string(this->cpm_k) + std::string("/") +
		                                                    std::to_string(this->cpm_p))));
		headers[p].push_back(std::make_pair("CPM wave shape", this->wave_shape));
		headers[p].push_back(std::make_pair("CPM mapping", this->mapping));
	}

	headers[p].push_back(std::make_pair("Bits per symbol", std::to_string(this->bps)));
	headers[p].push_back(std::make_pair("Sampling factor", std::to_string(this->upf)));

	// --------------------------------------------------------------------------------------------------- demodulator
	std::string demod_sig2 = (this->no_sig2) ? "off" : "on";
	std::string demod_max  = (this->type == "BPSK"     ) ||
	                         (this->type == "BPSK_FAST") ||
	                         (this->type == "OOK"      ) ||
	                         (this->type == "SCMA"     ) ?
	                         "unused" : this->max;
	std::string demod_ite  = std::to_string(this->n_ite);
	std::string demod_psi  = this->psi;

	if (this->sigma != -1.f && full)
		headers[p].push_back(std::make_pair("Sigma value", std::to_string(this->sigma)));
	headers[p].push_back(std::make_pair("Sigma square", demod_sig2));
	if (demod_max != "unused")
		headers[p].push_back(std::make_pair("Max type", demod_max));
	if (this->type == "SCMA")
	{
		headers[p].push_back(std::make_pair("Number of iterations", demod_ite));
		headers[p].push_back(std::make_pair("Psi function", demod_psi));
	}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
module::Modem<B,R,Q>* Modem::parameters
::_build() const
{
	     if (this->type == "BPSK"     ) return new module::Modem_BPSK     <B,R,Q    >(this->N,                   tools::Sigma<R>((R)this->sigma),                                                                                               this->no_sig2, this->n_frames);
	else if (this->type == "BPSK_FAST") return new module::Modem_BPSK_fast<B,R,Q    >(this->N,                   tools::Sigma<R>((R)this->sigma),                                                                                               this->no_sig2, this->n_frames);
	else if (this->type == "OOK"      ) return new module::Modem_OOK      <B,R,Q    >(this->N,                   tools::Sigma<R>((R)this->sigma),                                                                                               this->no_sig2, this->n_frames);
	else if (this->type == "PAM"      ) return new module::Modem_PAM      <B,R,Q,MAX>(this->N,                   tools::Sigma<R>((R)this->sigma), this->bps,                                                                                    this->no_sig2, this->n_frames);
	else if (this->type == "QAM"      ) return new module::Modem_QAM      <B,R,Q,MAX>(this->N,                   tools::Sigma<R>((R)this->sigma), this->bps,                                                                                    this->no_sig2, this->n_frames);
	else if (this->type == "PSK"      ) return new module::Modem_PSK      <B,R,Q,MAX>(this->N,                   tools::Sigma<R>((R)this->sigma), this->bps,                                                                                    this->no_sig2, this->n_frames);
	else if (this->type == "USER"     ) return new module::Modem_user     <B,R,Q,MAX>(this->N, this->const_path, tools::Sigma<R>((R)this->sigma), this->bps,                                                                                    this->no_sig2, this->n_frames);
	else if (this->type == "CPM"      ) return new module::Modem_CPM      <B,R,Q,MAX>(this->N,                   tools::Sigma<R>((R)this->sigma), this->bps, this->upf, this->cpm_L, this->cpm_k, this->cpm_p, this->mapping, this->wave_shape, this->no_sig2, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem::parameters
::_build_scma() const
{
	     if (this->psi == "PSI0") return new module::Modem_SCMA <B,R,Q,tools::psi_0<Q>>(this->N, tools::Sigma<R>((R)this->sigma), this->bps, this->no_sig2, this->n_ite, this->n_frames);
	else if (this->psi == "PSI1") return new module::Modem_SCMA <B,R,Q,tools::psi_1<Q>>(this->N, tools::Sigma<R>((R)this->sigma), this->bps, this->no_sig2, this->n_ite, this->n_frames);
	else if (this->psi == "PSI2") return new module::Modem_SCMA <B,R,Q,tools::psi_2<Q>>(this->N, tools::Sigma<R>((R)this->sigma), this->bps, this->no_sig2, this->n_ite, this->n_frames);
	else if (this->psi == "PSI3") return new module::Modem_SCMA <B,R,Q,tools::psi_3<Q>>(this->N, tools::Sigma<R>((R)this->sigma), this->bps, this->no_sig2, this->n_ite, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem::parameters
::build() const
{
	if (this->type == "SCMA")
	{
		return _build_scma<B,R,Q>();
	}
	else
	{
		     if (this->max == "MAX"  ) return _build<B,R,Q,tools::max          <Q>>();
		else if (this->max == "MAXL" ) return _build<B,R,Q,tools::max_linear   <Q>>();
		else if (this->max == "MAXS" ) return _build<B,R,Q,tools::max_star     <Q>>();
		else if (this->max == "MAXSS") return _build<B,R,Q,tools::max_star_safe<Q>>();
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem
::build(const parameters &params)
{
	return params.template build<B,R,Q>();
}

int Modem
::get_buffer_size_after_modulation(const std::string &type,
                                   const int         N,
                                   const int         bps,
                                   const int         upf,
                                   const int         cpm_L,
                                   const int         cpm_p)
{
	     if (type == "BPSK"     ) return module::Modem_BPSK     <>::size_mod(N                        );
	else if (type == "BPSK_FAST") return module::Modem_BPSK_fast<>::size_mod(N                        );
	else if (type == "OOK"      ) return module::Modem_OOK      <>::size_mod(N                        );
	else if (type == "SCMA"     ) return module::Modem_SCMA     <>::size_mod(N, bps                   );
	else if (type == "PAM"      ) return module::Modem_PAM      <>::size_mod(N, bps                   );
	else if (type == "QAM"      ) return module::Modem_QAM      <>::size_mod(N, bps                   );
	else if (type == "PSK"      ) return module::Modem_PSK      <>::size_mod(N, bps                   );
	else if (type == "USER"     ) return module::Modem_user     <>::size_mod(N, bps                   );
	else if (type == "CPM"      ) return module::Modem_CPM      <>::size_mod(N, bps, cpm_L, cpm_p, upf);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

int Modem
::get_buffer_size_after_filtering(const std::string &type,
                                  const int         N,
                                  const int         bps,
                                  const int         cpm_L,
                                  const int         cpm_p)
{
	     if (type == "BPSK"     ) return module::Modem_BPSK     <>::size_fil(N                   );
	else if (type == "BPSK_FAST") return module::Modem_BPSK_fast<>::size_fil(N                   );
	else if (type == "OOK"      ) return module::Modem_OOK      <>::size_fil(N                   );
	else if (type == "SCMA"     ) return module::Modem_SCMA     <>::size_fil(N, bps              );
	else if (type == "PAM"      ) return module::Modem_PAM      <>::size_fil(N, bps              );
	else if (type == "QAM"      ) return module::Modem_QAM      <>::size_fil(N, bps              );
	else if (type == "PSK"      ) return module::Modem_PSK      <>::size_fil(N, bps              );
	else if (type == "USER"     ) return module::Modem_user     <>::size_fil(N, bps              );
	else if (type == "CPM"      ) return module::Modem_CPM      <>::size_fil(N, bps, cpm_L, cpm_p);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

bool Modem
::is_complex_mod(const std::string &type, const int bps)
{
	     if (type == "BPSK"     ) return module::Modem_BPSK     <>::is_complex_mod();
	else if (type == "BPSK_FAST") return module::Modem_BPSK_fast<>::is_complex_mod();
	else if (type == "OOK"      ) return module::Modem_OOK      <>::is_complex_mod();
	else if (type == "SCMA"     ) return module::Modem_SCMA     <>::is_complex_mod();
	else if (type == "PAM"      ) return module::Modem_PAM      <>::is_complex_mod();
	else if (type == "QAM"      ) return module::Modem_QAM      <>::is_complex_mod();
	else if (type == "PSK"      ) return module::Modem_PSK      <>::is_complex_mod();
	else if (type == "USER"     ) return module::Modem_user     <>::is_complex_mod();
	else if (type == "CPM"      ) return module::Modem_CPM      <>::is_complex_mod();

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

bool Modem
::is_complex_fil(const std::string &type, const int bps)
{
	     if (type == "BPSK"     ) return module::Modem_BPSK     <>::is_complex_fil();
	else if (type == "BPSK_FAST") return module::Modem_BPSK_fast<>::is_complex_fil();
	else if (type == "OOK"      ) return module::Modem_OOK      <>::is_complex_fil();
	else if (type == "SCMA"     ) return module::Modem_SCMA     <>::is_complex_fil();
	else if (type == "PAM"      ) return module::Modem_PAM      <>::is_complex_fil();
	else if (type == "QAM"      ) return module::Modem_QAM      <>::is_complex_fil();
	else if (type == "PSK"      ) return module::Modem_PSK      <>::is_complex_fil();
	else if (type == "USER"     ) return module::Modem_user     <>::is_complex_fil();
	else if (type == "CPM"      ) return module::Modem_CPM      <>::is_complex_fil();

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Modem<B_8 ,R_8 ,Q_8 >* aff3ct::factory::Modem::parameters::build<B_8 ,R_8 ,Q_8 >() const;
template aff3ct::module::Modem<B_8 ,R_8 ,R_8 >* aff3ct::factory::Modem::parameters::build<B_8 ,R_8 ,R_8 >() const;
template aff3ct::module::Modem<B_16,R_16,Q_16>* aff3ct::factory::Modem::parameters::build<B_16,R_16,Q_16>() const;
template aff3ct::module::Modem<B_16,R_16,R_16>* aff3ct::factory::Modem::parameters::build<B_16,R_16,R_16>() const;
template aff3ct::module::Modem<B_32,R_32,Q_32>* aff3ct::factory::Modem::parameters::build<B_32,R_32,Q_32>() const;
template aff3ct::module::Modem<B_64,R_64,Q_64>* aff3ct::factory::Modem::parameters::build<B_64,R_64,Q_64>() const;
template aff3ct::module::Modem<B_8 ,R_8 ,Q_8 >* aff3ct::factory::Modem::build<B_8 ,R_8 ,Q_8 >(const aff3ct::factory::Modem::parameters&);
template aff3ct::module::Modem<B_8 ,R_8 ,R_8 >* aff3ct::factory::Modem::build<B_8 ,R_8 ,R_8 >(const aff3ct::factory::Modem::parameters&);
template aff3ct::module::Modem<B_16,R_16,Q_16>* aff3ct::factory::Modem::build<B_16,R_16,Q_16>(const aff3ct::factory::Modem::parameters&);
template aff3ct::module::Modem<B_16,R_16,R_16>* aff3ct::factory::Modem::build<B_16,R_16,R_16>(const aff3ct::factory::Modem::parameters&);
template aff3ct::module::Modem<B_32,R_32,Q_32>* aff3ct::factory::Modem::build<B_32,R_32,Q_32>(const aff3ct::factory::Modem::parameters&);
template aff3ct::module::Modem<B_64,R_64,Q_64>* aff3ct::factory::Modem::build<B_64,R_64,Q_64>(const aff3ct::factory::Modem::parameters&);
#else
template aff3ct::module::Modem<B,R,Q>* aff3ct::factory::Modem::parameters::build<B,R,Q>() const;
template aff3ct::module::Modem<B,R,Q>* aff3ct::factory::Modem::build<B,R,Q>(const aff3ct::factory::Modem::parameters&);
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template aff3ct::module::Modem<B,R,R>* aff3ct::factory::Modem::parameters::build<B,R,R>() const;
template aff3ct::module::Modem<B,R,R>* aff3ct::factory::Modem::build<B,R,R>(const aff3ct::factory::Modem::parameters&);
#endif
#endif
// ==================================================================================== explicit template instantiation
