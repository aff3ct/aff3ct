#include <algorithm>
#include <utility>
#include <memory>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Tools/Code/SCMA/modem_SCMA_functions.hpp"
#include "Tools/Code/SCMA/Codebook.hpp"
#include "Module/Modem/OOK/Modem_OOK_BSC.hpp"
#include "Module/Modem/OOK/Modem_OOK_BEC.hpp"
#include "Module/Modem/OOK/Modem_OOK_AWGN.hpp"
#include "Module/Modem/OOK/Modem_OOK_optical.hpp"
#include "Module/Modem/OOK/Modem_OOK_optical_rop_estimate.hpp"
#include "Module/Modem/BPSK/Modem_BPSK.hpp"
#include "Module/Modem/BPSK/Modem_BPSK_fast.hpp"
#include "Module/Modem/CPM/Modem_CPM.hpp"
#include "Module/Modem/SCMA/Modem_SCMA.hpp"
#include "Module/Modem/Generic/Modem_generic.hpp"
#include "Module/Modem/Generic/Modem_generic_fast.hpp"
#include "Tools/Constellation/PAM/Constellation_PAM.hpp"
#include "Tools/Constellation/PSK/Constellation_PSK.hpp"
#include "Tools/Constellation/QAM/Constellation_QAM.hpp"
#include "Tools/Constellation/User/Constellation_user.hpp"
#include "Factory/Module/Modem/Modem.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Modem_name   = "Modem";
const std::string aff3ct::factory::Modem_prefix = "mdm";

Modem
::Modem(const std::string &prefix)
: Factory(Modem_name, Modem_name, prefix)
{
}

Modem* Modem
::clone() const
{
	return new Modem(*this);
}

void Modem
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Modem::";

	// ----------------------------------------------------------------------------------------------------- modulator
	tools::add_arg(args, p, class_name+"p+fra-size,N",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("BPSK", "OOK", "PSK", "PAM", "QAM", "CPM", "USER", "SCMA")));

	tools::add_arg(args, p, class_name+"p+implem",
		cli::Text(cli::Including_set("STD", "FAST")));

	tools::add_arg(args, p, class_name+"p+bps",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+const-path",
		cli::File(cli::openmode::read_write));

	tools::add_arg(args, p, class_name+"p+cb-path",
		cli::File(cli::openmode::read));

	tools::add_arg(args, p, class_name+"p+cpm-std",
		cli::Text(cli::Including_set("GSM")));

	tools::add_arg(args, p, class_name+"p+cpm-L",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+cpm-k",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+cpm-p",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+cpm-upf",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+cpm-map",
		cli::Text(cli::Including_set("NATURAL", "GRAY")));

	tools::add_arg(args, p, class_name+"p+cpm-ws",
		cli::Text(cli::Including_set("GMSK", "REC", "RCOS")));

	// --------------------------------------------------------------------------------------------------- demodulator
	tools::add_arg(args, p, class_name+"p+max",
		cli::Text(cli::Including_set("MAX", "MAXL", "MAXS", "MAXSS")));

	tools::add_arg(args, p, class_name+"p+no-sig2",
		cli::None());

	tools::add_arg(args, p, class_name+"p+psi",
		cli::Text(cli::Including_set("PSI0", "PSI1", "PSI2", "PSI3")));

	tools::add_arg(args, p, class_name+"p+ite",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+rop-est",
		cli::Integer(cli::Positive()));
}

void Modem
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	// ----------------------------------------------------------------------------------------------------- modulator
	if(vals.exist({p+"-type"    })) this->type          = vals.at     ({p+"-type"    });
	if(vals.exist({p+"-implem"  })) this->implem        = vals.at     ({p+"-implem"  });
	if(vals.exist({p+"-cpm-std" })) this->cpm_std       = vals.at     ({p+"-cpm-std" });
	if(vals.exist({p+"-cb-path" })) this->codebook_path = vals.to_file({p+"-cb-path"});

	if (this->type == "CPM")
	{
		if (!this->cpm_std.empty())
		{
			if (this->cpm_std == "GSM")
			{
				this->cpm_L          = 3;
				this->cpm_k          = 1;
				this->cpm_p          = 2;
				this->bps            = 1;
				this->cpm_upf        = 5;
				this->cpm_mapping    = "NATURAL";
				this->cpm_wave_shape = "GMSK";
			}
			else
			{
				std::stringstream message;
				message << "Unknown CPM standard ('cpm_std' = " << this->cpm_std << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		}
	}

	if (this->type == "SCMA")
	{
		tools::Codebook<float> cb(this->codebook_path);

		this->bps = (int)cb.get_system_bps(); // codebook bps is float so here bps is stocked rounded
	}

	if(vals.exist({p+"-fra-size", "N"})) this->N              = vals.to_int ({p+"-fra-size", "N"});
	if(vals.exist({p+"-bps"          })) this->bps            = vals.to_int ({p+"-bps"          });
	if(vals.exist({p+"-const-path"   })) this->const_path     = vals.to_file({p+"-const-path"   });
	if(vals.exist({p+"-cpm-L"        })) this->cpm_L          = vals.to_int ({p+"-cpm-L"        });
	if(vals.exist({p+"-cpm-p"        })) this->cpm_p          = vals.to_int ({p+"-cpm-p"        });
	if(vals.exist({p+"-cpm-k"        })) this->cpm_k          = vals.to_int ({p+"-cpm-k"        });
	if(vals.exist({p+"-cpm-upf"      })) this->cpm_upf        = vals.to_int ({p+"-cpm-upf"      });
	if(vals.exist({p+"-cpm-map"      })) this->cpm_mapping    = vals.at     ({p+"-cpm-map"      });
	if(vals.exist({p+"-cpm-ws"       })) this->cpm_wave_shape = vals.at     ({p+"-cpm-ws"       });
	if(vals.exist({p+"-rop-est"      })) this->rop_est_bits   = vals.to_int ({p+"-rop-est"      });

	// force the number of bits per symbol to 1 when BPSK mod
	if (this->type == "BPSK" || this->type == "OOK")
		this->bps = 1;

	std::unique_ptr<tools::Constellation<float>> cstl;
	try
	{
		cstl.reset(this->build_constellation<float>());
	}
	catch(tools::cannot_allocate &) {}

	if (cstl != nullptr && this->type == "USER")
		this->bps = cstl->get_n_bits_per_symbol();

	this->complex = is_complex_mod(this->type, this->bps, cstl.get());

	this->N_mod = get_buffer_size_after_modulation(this->type,
	                                               this->N,
	                                               this->bps,
	                                               this->cpm_upf,
	                                               this->cpm_L,
	                                               this->cpm_p,
	                                               cstl.get());

	this->N_fil = get_buffer_size_after_filtering (this->type,
	                                               this->N,
	                                               this->bps,
	                                               this->cpm_L,
	                                               this->cpm_p,
	                                               cstl.get());

	// --------------------------------------------------------------------------------------------------- demodulator
	if(vals.exist({p+"-no-sig2"})) this->no_sig2 = true;
	if(vals.exist({p+"-ite"    })) this->n_ite   = vals.to_int  ({p+"-ite"  });
	if(vals.exist({p+"-max"    })) this->max     = vals.at      ({p+"-max"  });
	if(vals.exist({p+"-psi"    })) this->psi     = vals.at      ({p+"-psi"  });
}

void Modem
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	// ----------------------------------------------------------------------------------------------------- modulator
	headers[p].push_back(std::make_pair("Type",           this->type  ));
	headers[p].push_back(std::make_pair("Implementation", this->implem));
	if (full) headers[p].push_back(std::make_pair("Frame size (N)", std::to_string(this->N)));
	if (this->type == "CPM")
	{
		if(this->cpm_std.size())
			headers[p].push_back(std::make_pair("CPM standard", this->cpm_std));

		headers[p].push_back(std::make_pair("CPM L memory", std::to_string(this->cpm_L)));
		headers[p].push_back(std::make_pair("CPM h index", (std::to_string(this->cpm_k) + std::string("/") +
		                                                    std::to_string(this->cpm_p))));
		headers[p].push_back(std::make_pair("CPM wave shape", this->cpm_wave_shape));
		headers[p].push_back(std::make_pair("CPM mapping", this->cpm_mapping));
		headers[p].push_back(std::make_pair("CPM sampling factor", std::to_string(this->cpm_upf)));
	}

	headers[p].push_back(std::make_pair("Bits per symbol", std::to_string(this->bps)));

	// --------------------------------------------------------------------------------------------------- demodulator
	std::string demod_sig2 = (this->no_sig2) ? "off" : "on";
	std::string demod_max  = (this->type == "BPSK") ||
	                         (this->type == "OOK" ) ||
	                         (this->type == "SCMA") ?
	                         "unused" : this->max;
	std::string demod_ite  = std::to_string(this->n_ite);
	std::string demod_psi  = this->psi;

	headers[p].push_back(std::make_pair("Sigma square", demod_sig2));
	if (demod_max != "unused")
		headers[p].push_back(std::make_pair("Max type", demod_max));
	if (this->type == "SCMA")
	{
		headers[p].push_back(std::make_pair("Number of iterations", demod_ite    ));
		headers[p].push_back(std::make_pair("Psi function",         demod_psi    ));
		headers[p].push_back(std::make_pair("Codebook",             codebook_path));
	}

	if (full) headers[p].push_back(std::make_pair("Channel type", channel_type));

	if (this->type == "OOK" && channel_type == "OPTICAL")
	{
		std::string str_est = "known";
		if (this->rop_est_bits > 0)
			str_est = "on " + std::to_string(this->rop_est_bits) + " bits";
		headers[p].push_back(std::make_pair("ROP estimation", str_est));
	}
}

template <typename R>
tools::Constellation<R>* Modem
::build_constellation() const
{
	if (this->type == "PAM" ) return new tools::Constellation_PAM <R>(this->bps);
	if (this->type == "QAM" ) return new tools::Constellation_QAM <R>(this->bps);
	if (this->type == "PSK" ) return new tools::Constellation_PSK <R>(this->bps);
	if (this->type == "USER") return new tools::Constellation_user<R>(this->const_path);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX, tools::proto_max_i<Q> MAXI>
module::Modem<B,R,Q>* Modem
::_build(const tools::Constellation<R>* cstl) const
{
	if (this->type == "BPSK" && this->implem == "STD" ) return new module::Modem_BPSK     <B,R,Q    >(this->N,                                                                                                           this->no_sig2);
	if (this->type == "BPSK" && this->implem == "FAST") return new module::Modem_BPSK_fast<B,R,Q    >(this->N,                                                                                                           this->no_sig2);
	if (this->type == "CPM"  && this->implem == "STD" ) return new module::Modem_CPM      <B,R,Q,MAX>(this->N, this->bps, this->cpm_upf, this->cpm_L, this->cpm_k, this->cpm_p, this->cpm_mapping, this->cpm_wave_shape, this->no_sig2);

	if (cstl != nullptr)
	{
		if (this->implem == "FAST")
			return new module::Modem_generic_fast<B,R,Q,MAX,MAXI>(N, *cstl, this->no_sig2);
		else
			return new module::Modem_generic     <B,R,Q,MAX     >(N, *cstl, this->no_sig2);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem
::_build_scma() const
{
	if (this->psi == "PSI0") return new module::Modem_SCMA<B,R,Q,tools::psi_0<Q>>(this->N, this->codebook_path, this->no_sig2, this->n_ite);
	if (this->psi == "PSI1") return new module::Modem_SCMA<B,R,Q,tools::psi_1<Q>>(this->N, this->codebook_path, this->no_sig2, this->n_ite);
	if (this->psi == "PSI2") return new module::Modem_SCMA<B,R,Q,tools::psi_2<Q>>(this->N, this->codebook_path, this->no_sig2, this->n_ite);
	if (this->psi == "PSI3") return new module::Modem_SCMA<B,R,Q,tools::psi_3<Q>>(this->N, this->codebook_path, this->no_sig2, this->n_ite);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem
::build(const tools::Constellation<R>* cstl) const
{
	if (this->type == "SCMA" && this->implem == "STD")
	{
		return _build_scma<B,R,Q>();
	}
	else if (this->type == "OOK" && this->implem == "STD")
	{
		if (channel_type == "AWGN") return new module::Modem_OOK_AWGN<B,R,Q>(this->N, this->no_sig2);
		if (channel_type == "BEC" ) return new module::Modem_OOK_BEC <B,R,Q>(this->N               );
		if (channel_type == "BSC" ) return new module::Modem_OOK_BSC <B,R,Q>(this->N               );
	}
	else
	{
		if (this->max == "MAX"  ) return _build<B,R,Q,tools::max          <Q>,tools::max_i          <Q>>(cstl);
		if (this->max == "MAXL" ) return _build<B,R,Q,tools::max_linear   <Q>,tools::max_linear_i   <Q>>(cstl);
		if (this->max == "MAXS" ) return _build<B,R,Q,tools::max_star     <Q>,tools::max_star_i     <Q>>(cstl);
		if (this->max == "MAXSS") return _build<B,R,Q,tools::max_star_safe<Q>,tools::max_star_safe_i<Q>>(cstl);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem
::build(const tools::Distributions<R>& dist) const
{
	if (this->type == "OOK" && this->implem == "STD" && channel_type == "OPTICAL")
	{
		if (this->rop_est_bits == 0)
			return new module::Modem_OOK_optical<B,R,Q>(this->N, dist);
		else
			return new module::Modem_OOK_optical_rop_estimate<B,R,Q>(this->N, rop_est_bits, dist);
	}
	return build<B,R,Q>();
}

bool Modem
::has_constellation(const std::string &type)
{
	return type == "PAM" || type == "QAM" || type == "PSK" || type == "USER";
}

int Modem
::get_buffer_size_after_modulation(const std::string &type,
                                   const int         N,
                                   const int         bps,
                                   const int         cpm_upf,
                                   const int         cpm_L,
                                   const int         cpm_p,
                                   const tools::Constellation<float>* c)
{
	if (c != nullptr && has_constellation(type))
		return module::Modem_generic<>::size_mod(N, *c);

	if (type == "BPSK") return module::Modem_BPSK<>::size_mod(N                            );
	if (type == "OOK" ) return module::Modem_OOK <>::size_mod(N                            );
	if (type == "SCMA") return module::Modem_SCMA<>::size_mod(N, bps                       );
	if (type == "CPM" ) return module::Modem_CPM <>::size_mod(N, bps, cpm_L, cpm_p, cpm_upf);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

int Modem
::get_buffer_size_after_filtering(const std::string &type,
                                  const int         N,
                                  const int         bps,
                                  const int         cpm_L,
                                  const int         cpm_p,
                                  const tools::Constellation<float>* c)
{
	if (c != nullptr && has_constellation(type))
		return module::Modem_generic<>::size_fil(N, *c);

	if (type == "BPSK") return module::Modem_BPSK<>::size_fil(N                   );
	if (type == "OOK" ) return module::Modem_OOK <>::size_fil(N                   );
	if (type == "SCMA") return module::Modem_SCMA<>::size_fil(N, bps              );
	if (type == "CPM" ) return module::Modem_CPM <>::size_fil(N, bps, cpm_L, cpm_p);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

bool Modem
::is_complex_mod(const std::string &type, const int bps, const tools::Constellation<float>* c)
{
	if (c != nullptr && has_constellation(type))
		return module::Modem_generic<>::is_complex_mod(*c);

	if (type == "BPSK") return module::Modem_BPSK<>::is_complex_mod();
	if (type == "OOK" ) return module::Modem_OOK <>::is_complex_mod();
	if (type == "SCMA") return module::Modem_SCMA<>::is_complex_mod();
	if (type == "CPM" ) return module::Modem_CPM <>::is_complex_mod();

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

bool Modem
::is_complex_fil(const std::string &type, const int bps, const tools::Constellation<float>* c)
{
	if (c != nullptr && has_constellation(type))
		return module::Modem_generic<>::is_complex_fil(*c);

	if (type == "BPSK") return module::Modem_BPSK<>::is_complex_fil();
	if (type == "OOK" ) return module::Modem_OOK <>::is_complex_fil();
	if (type == "SCMA") return module::Modem_SCMA<>::is_complex_fil();
	if (type == "CPM" ) return module::Modem_CPM <>::is_complex_fil();

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Modem<B_8 ,R_8 ,Q_8 >* aff3ct::factory::Modem::build<B_8 ,R_8 ,Q_8 >(const tools::Constellation<R_8 >*) const;
template aff3ct::module::Modem<B_8 ,R_8 ,R_8 >* aff3ct::factory::Modem::build<B_8 ,R_8 ,R_8 >(const tools::Constellation<R_8 >*) const;
template aff3ct::module::Modem<B_16,R_16,Q_16>* aff3ct::factory::Modem::build<B_16,R_16,Q_16>(const tools::Constellation<R_16>*) const;
template aff3ct::module::Modem<B_16,R_16,R_16>* aff3ct::factory::Modem::build<B_16,R_16,R_16>(const tools::Constellation<R_16>*) const;
template aff3ct::module::Modem<B_32,R_32,Q_32>* aff3ct::factory::Modem::build<B_32,R_32,Q_32>(const tools::Constellation<R_32>*) const;
template aff3ct::module::Modem<B_64,R_64,Q_64>* aff3ct::factory::Modem::build<B_64,R_64,Q_64>(const tools::Constellation<R_64>*) const;

template aff3ct::module::Modem<B_8 ,R_8 ,Q_8 >* aff3ct::factory::Modem::build<B_8 ,R_8 ,Q_8 >(const tools::Distributions<R_8 >&) const;
template aff3ct::module::Modem<B_8 ,R_8 ,R_8 >* aff3ct::factory::Modem::build<B_8 ,R_8 ,R_8 >(const tools::Distributions<R_8 >&) const;
template aff3ct::module::Modem<B_16,R_16,Q_16>* aff3ct::factory::Modem::build<B_16,R_16,Q_16>(const tools::Distributions<R_16>&) const;
template aff3ct::module::Modem<B_16,R_16,R_16>* aff3ct::factory::Modem::build<B_16,R_16,R_16>(const tools::Distributions<R_16>&) const;
template aff3ct::module::Modem<B_32,R_32,Q_32>* aff3ct::factory::Modem::build<B_32,R_32,Q_32>(const tools::Distributions<R_32>&) const;
template aff3ct::module::Modem<B_64,R_64,Q_64>* aff3ct::factory::Modem::build<B_64,R_64,Q_64>(const tools::Distributions<R_64>&) const;

template tools::Constellation<R_32>* aff3ct::factory::Modem::build_constellation<R_32>() const;
template tools::Constellation<R_64>* aff3ct::factory::Modem::build_constellation<R_64>() const;
#else
template aff3ct::module::Modem<B,R,Q>* aff3ct::factory::Modem::build<B,R,Q>(const tools::Constellation<R>*) const;

template aff3ct::module::Modem<B,R,Q>* aff3ct::factory::Modem::build<B,R,Q>(const tools::Distributions<R>&) const;
#if !defined(AFF3CT_32BIT_PREC) && !defined(AFF3CT_64BIT_PREC)
template aff3ct::module::Modem<B,R,R>* aff3ct::factory::Modem::build<B,R,R>(const tools::Constellation<R>*) const;

template aff3ct::module::Modem<B,R,R>* aff3ct::factory::Modem::build<B,R,R>(const tools::Distributions<R>&) const;
#endif
template tools::Constellation<R>* aff3ct::factory::Modem::build_constellation<R>() const;
#endif

// ==================================================================================== explicit template instantiation
