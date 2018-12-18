#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"

#include "Tools/Code/SCMA/modem_SCMA_functions.hpp"

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

#include "Tools/Constellation/PAM/Constellation_PAM.hpp"
#include "Tools/Constellation/PSK/Constellation_PSK.hpp"
#include "Tools/Constellation/QAM/Constellation_QAM.hpp"
#include "Tools/Constellation/User/Constellation_user.hpp"

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

Modem::parameters* Modem::parameters
::clone() const
{
	return new Modem::parameters(*this);
}

void Modem::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Modem::parameters::";

	// ----------------------------------------------------------------------------------------------------- modulator
	tools::add_arg(args, p, class_name+"p+fra-size,N",
		tools::Integer(tools::Positive(), tools::Non_zero()),
		tools::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+fra,F",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+type",
		tools::Text(tools::Including_set("BPSK", "OOK", "PSK", "PAM", "QAM", "CPM", "USER", "SCMA")));

	tools::add_arg(args, p, class_name+"p+implem",
		tools::Text(tools::Including_set("STD", "FAST")));

	tools::add_arg(args, p, class_name+"p+bps",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+const-path",
		tools::File(tools::openmode::read_write));

	tools::add_arg(args, p, class_name+"p+codebook",
		tools::File(tools::openmode::read));

	tools::add_arg(args, p, class_name+"p+cpm-std",
		tools::Text(tools::Including_set("GSM")));

	tools::add_arg(args, p, class_name+"p+cpm-L",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+cpm-k",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+cpm-p",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+cpm-upf",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+cpm-map",
		tools::Text(tools::Including_set("NATURAL", "GRAY")));

	tools::add_arg(args, p, class_name+"p+cpm-ws",
		tools::Text(tools::Including_set("GMSK", "REC", "RCOS")));


	// --------------------------------------------------------------------------------------------------- demodulator
	tools::add_arg(args, p, class_name+"p+max",
		tools::Text(tools::Including_set("MAX", "MAXL", "MAXS", "MAXSS")));

	tools::add_arg(args, p, class_name+"p+noise",
		tools::Real(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+no-sig2",
		tools::None());

	tools::add_arg(args, p, class_name+"p+psi",
		tools::Text(tools::Including_set("PSI0", "PSI1", "PSI2", "PSI3")));

	tools::add_arg(args, p, class_name+"p+ite",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+rop-est",
		tools::Integer(tools::Positive()));
}

void Modem::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	// ----------------------------------------------------------------------------------------------------- modulator
	if(vals.exist({p+"-type"    })) this->type     = vals.at     ({p+"-type"    });
	if(vals.exist({p+"-implem"  })) this->implem   = vals.at     ({p+"-implem"  });
	if(vals.exist({p+"-cpm-std" })) this->cpm_std  = vals.at     ({p+"-cpm-std" });
	if(vals.exist({p+"-codebook"})) this->codebook = vals.to_file({p+"-codebook"});

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
		tools::Codebook<float> cb(this->codebook);

		this->bps      = cb.get_system_bps(); // codebook bps is float so here bps is stocked rounded
		this->n_frames = cb.get_number_of_users();
	}

	if(vals.exist({p+"-fra-size", "N"})) this->N              = vals.to_int ({p+"-fra-size", "N"});
	if(vals.exist({p+"-fra",      "F"})) this->n_frames       = vals.to_int ({p+"-fra",      "F"});
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



	std::unique_ptr<tools::Constellation<float>> cstl(this->build_constellation<float>());

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
	if(vals.exist({p+"-noise"  })) this->noise   = vals.to_float({p+"-noise"});
	if(vals.exist({p+"-ite"    })) this->n_ite   = vals.to_int  ({p+"-ite"  });
	if(vals.exist({p+"-max"    })) this->max     = vals.at      ({p+"-max"  });
	if(vals.exist({p+"-psi"    })) this->psi     = vals.at      ({p+"-psi"  });
}

void Modem::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	// ----------------------------------------------------------------------------------------------------- modulator
	headers[p].push_back(std::make_pair("Type",           this->type  ));
	headers[p].push_back(std::make_pair("Implementation", this->implem));
	if (full) headers[p].push_back(std::make_pair("Frame size (N)", std::to_string(this->N)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
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

	if (this->noise != -1.f && full)
		headers[p].push_back(std::make_pair("Sigma value", std::to_string(this->noise)));
	headers[p].push_back(std::make_pair("Sigma square", demod_sig2));
	if (demod_max != "unused")
		headers[p].push_back(std::make_pair("Max type", demod_max));
	if (this->type == "SCMA")
	{
		headers[p].push_back(std::make_pair("Number of iterations", demod_ite));
		headers[p].push_back(std::make_pair("Psi function",         demod_psi));
		headers[p].push_back(std::make_pair("Codebook",             codebook ));
	}

	if (full) headers[p].push_back(std::make_pair("Channel type", channel_type));

	if (this->type == "OOK")
	{
		std::string str_est = "known";
		if (this->rop_est_bits > 0)
			str_est = "on " + std::to_string(this->rop_est_bits) + " bits";
		headers[p].push_back(std::make_pair("ROP estimation", str_est));
	}
}

template <typename R>
tools::Constellation<R>* Modem::parameters
::build_constellation() const
{
	if (this->type == "PAM" ) return new tools::Constellation_PAM <R>(this->bps);
	if (this->type == "QAM" ) return new tools::Constellation_QAM <R>(this->bps);
	if (this->type == "PSK" ) return new tools::Constellation_PSK <R>(this->bps);
	if (this->type == "USER") return new tools::Constellation_user<R>(this->const_path);

	return nullptr;
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
module::Modem<B,R,Q>* Modem::parameters
::_build() const
{
	if (this->type == "BPSK" && this->implem == "STD" ) return new module::Modem_BPSK     <B,R,Q    >(this->N, tools::Sigma<R>((R)this->noise),                                                                                                           this->no_sig2, this->n_frames);
	if (this->type == "BPSK" && this->implem == "FAST") return new module::Modem_BPSK_fast<B,R,Q    >(this->N, tools::Sigma<R>((R)this->noise),                                                                                                           this->no_sig2, this->n_frames);
	if (this->type == "CPM"  && this->implem == "STD" ) return new module::Modem_CPM      <B,R,Q,MAX>(this->N, tools::Sigma<R>((R)this->noise), this->bps, this->cpm_upf, this->cpm_L, this->cpm_k, this->cpm_p, this->cpm_mapping, this->cpm_wave_shape, this->no_sig2, this->n_frames);


	std::unique_ptr<tools::Constellation<R>> cstl(this->build_constellation<R>());
	if (cstl != nullptr) return new module::Modem_generic<B,R,Q,MAX>(N, std::move(cstl), tools::Sigma<R>((R)this->noise), this->no_sig2, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem::parameters
::_build_scma() const
{
	std::unique_ptr<tools::Codebook<R>> CB(new tools::Codebook<R>(this->codebook));
	if (this->psi == "PSI0") return new module::Modem_SCMA <B,R,Q,tools::psi_0<Q>>(this->N, std::move(CB), tools::Sigma<R>((R)this->noise), this->no_sig2, this->n_ite, this->n_frames);
	if (this->psi == "PSI1") return new module::Modem_SCMA <B,R,Q,tools::psi_1<Q>>(this->N, std::move(CB), tools::Sigma<R>((R)this->noise), this->no_sig2, this->n_ite, this->n_frames);
	if (this->psi == "PSI2") return new module::Modem_SCMA <B,R,Q,tools::psi_2<Q>>(this->N, std::move(CB), tools::Sigma<R>((R)this->noise), this->no_sig2, this->n_ite, this->n_frames);
	if (this->psi == "PSI3") return new module::Modem_SCMA <B,R,Q,tools::psi_3<Q>>(this->N, std::move(CB), tools::Sigma<R>((R)this->noise), this->no_sig2, this->n_ite, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem::parameters
::build() const
{
	if (this->type == "SCMA" && this->implem == "STD")
	{
		return _build_scma<B,R,Q>();
	}
	else if (this->type == "OOK" && this->implem == "STD")
	{
		if (channel_type == "AWGN") return new module::Modem_OOK_AWGN<B,R,Q>(this->N, tools::Sigma<R>((R)this->noise), this->no_sig2, this->n_frames);
		if (channel_type == "BEC" ) return new module::Modem_OOK_BEC <B,R,Q>(this->N, tools::EP   <R>((R)this->noise),                this->n_frames);
		if (channel_type == "BSC" ) return new module::Modem_OOK_BSC <B,R,Q>(this->N, tools::EP   <R>((R)this->noise),                this->n_frames);
	}
	else
	{
		if (this->max == "MAX"  ) return _build<B,R,Q,tools::max          <Q>>();
		if (this->max == "MAXL" ) return _build<B,R,Q,tools::max_linear   <Q>>();
		if (this->max == "MAXS" ) return _build<B,R,Q,tools::max_star     <Q>>();
		if (this->max == "MAXSS") return _build<B,R,Q,tools::max_star_safe<Q>>();
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem::parameters
::build(const tools::Distributions<R>& dist) const
{
	if (this->type == "OOK" && this->implem == "STD" && channel_type == "OPTICAL")
	{
		if (this->rop_est_bits == 0)
			return new module::Modem_OOK_optical<B,R,Q>(this->N, dist, tools::ROP<R>((R)this->noise), this->n_frames);
		else
			return new module::Modem_OOK_optical_rop_estimate<B,R,Q>(this->N, rop_est_bits, dist, this->n_frames);
	}
	return build<B,R,Q>();
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem
::build(const parameters &params)
{
	return params.template build<B,R,Q>();
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem
::build(const parameters &params, const tools::Distributions<R>& dist)
{
	return params.template build<B,R,Q>(dist);
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

template aff3ct::module::Modem<B_8 ,R_8 ,Q_8 >* aff3ct::factory::Modem::parameters::build<B_8 ,R_8 ,Q_8 >(const tools::Distributions<R_8 >&) const;
template aff3ct::module::Modem<B_8 ,R_8 ,R_8 >* aff3ct::factory::Modem::parameters::build<B_8 ,R_8 ,R_8 >(const tools::Distributions<R_8 >&) const;
template aff3ct::module::Modem<B_16,R_16,Q_16>* aff3ct::factory::Modem::parameters::build<B_16,R_16,Q_16>(const tools::Distributions<R_16>&) const;
template aff3ct::module::Modem<B_16,R_16,R_16>* aff3ct::factory::Modem::parameters::build<B_16,R_16,R_16>(const tools::Distributions<R_16>&) const;
template aff3ct::module::Modem<B_32,R_32,Q_32>* aff3ct::factory::Modem::parameters::build<B_32,R_32,Q_32>(const tools::Distributions<R_32>&) const;
template aff3ct::module::Modem<B_64,R_64,Q_64>* aff3ct::factory::Modem::parameters::build<B_64,R_64,Q_64>(const tools::Distributions<R_64>&) const;
template aff3ct::module::Modem<B_8 ,R_8 ,Q_8 >* aff3ct::factory::Modem::build<B_8 ,R_8 ,Q_8 >(const aff3ct::factory::Modem::parameters&, const tools::Distributions<R_8 >&);
template aff3ct::module::Modem<B_8 ,R_8 ,R_8 >* aff3ct::factory::Modem::build<B_8 ,R_8 ,R_8 >(const aff3ct::factory::Modem::parameters&, const tools::Distributions<R_8 >&);
template aff3ct::module::Modem<B_16,R_16,Q_16>* aff3ct::factory::Modem::build<B_16,R_16,Q_16>(const aff3ct::factory::Modem::parameters&, const tools::Distributions<R_16>&);
template aff3ct::module::Modem<B_16,R_16,R_16>* aff3ct::factory::Modem::build<B_16,R_16,R_16>(const aff3ct::factory::Modem::parameters&, const tools::Distributions<R_16>&);
template aff3ct::module::Modem<B_32,R_32,Q_32>* aff3ct::factory::Modem::build<B_32,R_32,Q_32>(const aff3ct::factory::Modem::parameters&, const tools::Distributions<R_32>&);
template aff3ct::module::Modem<B_64,R_64,Q_64>* aff3ct::factory::Modem::build<B_64,R_64,Q_64>(const aff3ct::factory::Modem::parameters&, const tools::Distributions<R_64>&);
#else
template aff3ct::module::Modem<B,R,Q>* aff3ct::factory::Modem::parameters::build<B,R,Q>() const;
template aff3ct::module::Modem<B,R,Q>* aff3ct::factory::Modem::build<B,R,Q>(const aff3ct::factory::Modem::parameters&);

template aff3ct::module::Modem<B,R,Q>* aff3ct::factory::Modem::parameters::build<B,R,Q>(const tools::Distributions<R>&) const;
template aff3ct::module::Modem<B,R,Q>* aff3ct::factory::Modem::build<B,R,Q>(const aff3ct::factory::Modem::parameters&, const tools::Distributions<R>&);
#if !defined(AFF3CT_32BIT_PREC) && !defined(AFF3CT_64BIT_PREC)
template aff3ct::module::Modem<B,R,R>* aff3ct::factory::Modem::parameters::build<B,R,R>() const;
template aff3ct::module::Modem<B,R,R>* aff3ct::factory::Modem::build<B,R,R>(const aff3ct::factory::Modem::parameters&);

template aff3ct::module::Modem<B,R,R>* aff3ct::factory::Modem::parameters::build<B,R,R>(const tools::Distributions<R>&) const;
template aff3ct::module::Modem<B,R,R>* aff3ct::factory::Modem::build<B,R,R>(const aff3ct::factory::Modem::parameters&, const tools::Distributions<R>&);
#endif
#endif

// ==================================================================================== explicit template instantiation
