#include "Tools/Exception/exception.hpp"
#include "Tools/Code/SCMA/modem_SCMA_functions.hpp"

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

const std::string aff3ct::factory::Modem::name   = "Modem";
const std::string aff3ct::factory::Modem::prefix = "mdm";

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
module::Modem<B,R,Q>* Modem
::_build(const parameters &params)
{
	     if (params.type == "BPSK"     ) return new module::Modem_BPSK     <B,R,Q    >(params.N, params.sigma,                                                                                                      params.no_sig2, params.n_frames);
	else if (params.type == "BPSK_FAST") return new module::Modem_BPSK_fast<B,R,Q    >(params.N, params.sigma,                                                                                                      params.no_sig2, params.n_frames);
	else if (params.type == "PAM"      ) return new module::Modem_PAM      <B,R,Q,MAX>(params.N, params.sigma, params.bps,                                                                                          params.no_sig2, params.n_frames);
	else if (params.type == "QAM"      ) return new module::Modem_QAM      <B,R,Q,MAX>(params.N, params.sigma, params.bps,                                                                                          params.no_sig2, params.n_frames);
	else if (params.type == "PSK"      ) return new module::Modem_PSK      <B,R,Q,MAX>(params.N, params.sigma, params.bps,                                                                                          params.no_sig2, params.n_frames);
	else if (params.type == "USER"     ) return new module::Modem_user     <B,R,Q,MAX>(params.N, params.sigma, params.bps, params.const_path,                                                                       params.no_sig2, params.n_frames);
	else if (params.type == "CPM"      ) return new module::Modem_CPM      <B,R,Q,MAX>(params.N, params.sigma, params.bps, params.upf, params.cpm_L, params.cpm_k, params.cpm_p, params.mapping, params.wave_shape, params.no_sig2, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem
::_build_scma(const parameters &params)
{
	     if (params.psi == "PSI0") return new module::Modem_SCMA <B,R,Q,tools::psi_0<Q>>(params.N, params.sigma, params.bps, params.no_sig2, params.n_ite, params.n_frames);
	else if (params.psi == "PSI1") return new module::Modem_SCMA <B,R,Q,tools::psi_1<Q>>(params.N, params.sigma, params.bps, params.no_sig2, params.n_ite, params.n_frames);
	else if (params.psi == "PSI2") return new module::Modem_SCMA <B,R,Q,tools::psi_2<Q>>(params.N, params.sigma, params.bps, params.no_sig2, params.n_ite, params.n_frames);
	else if (params.psi == "PSI3") return new module::Modem_SCMA <B,R,Q,tools::psi_3<Q>>(params.N, params.sigma, params.bps, params.no_sig2, params.n_ite, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* Modem
::build(const parameters &params)
{
	if (params.type == "SCMA")
	{
		return _build_scma<B,R,Q>(params);
	}
	else
	{
		     if (params.max == "MAX"  ) return _build<B,R,Q,tools::max          <Q>>(params);
		else if (params.max == "MAXL" ) return _build<B,R,Q,tools::max_linear   <Q>>(params);
		else if (params.max == "MAXS" ) return _build<B,R,Q,tools::max_star     <Q>>(params);
		else if (params.max == "MAXSS") return _build<B,R,Q,tools::max_star_safe<Q>>(params);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

int Modem
::get_buffer_size_after_modulation(const std::string type,
                                   const int         N,
                                   const int         bps,
                                   const int         upf,
                                   const int         cpm_L)
{
	     if (type == "BPSK"     ) return module::Modem_BPSK     <>::size_mod(N                 );
	else if (type == "BPSK_FAST") return module::Modem_BPSK_fast<>::size_mod(N                 );
	else if (type == "SCMA"     ) return module::Modem_SCMA     <>::size_mod(N, bps            );
	else if (type == "PAM"      ) return module::Modem_PAM      <>::size_mod(N, bps            );
	else if (type == "QAM"      ) return module::Modem_QAM      <>::size_mod(N, bps            );
	else if (type == "PSK"      ) return module::Modem_PSK      <>::size_mod(N, bps            );
	else if (type == "USER"     ) return module::Modem_user     <>::size_mod(N, bps            );
	else if (type == "CPM"      ) return module::Modem_CPM      <>::size_mod(N, bps, cpm_L, upf);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

int Modem
::get_buffer_size_after_filtering(const std::string type,
                                  const int         N,
                                  const int         bps,
                                  const int         cpm_L,
                                  const int         cpm_p)
{
	     if (type == "BPSK"     ) return module::Modem_BPSK     <>::size_fil(N                   );
	else if (type == "BPSK_FAST") return module::Modem_BPSK_fast<>::size_fil(N                   );
	else if (type == "SCMA"     ) return module::Modem_SCMA     <>::size_fil(N, bps              );
	else if (type == "PAM"      ) return module::Modem_PAM      <>::size_fil(N, bps              );
	else if (type == "QAM"      ) return module::Modem_QAM      <>::size_fil(N, bps              );
	else if (type == "PSK"      ) return module::Modem_PSK      <>::size_fil(N, bps              );
	else if (type == "USER"     ) return module::Modem_user     <>::size_fil(N, bps              );
	else if (type == "CPM"      ) return module::Modem_CPM      <>::size_fil(N, bps, cpm_L, cpm_p);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Modem
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	// ----------------------------------------------------------------------------------------------------- modulator
	req_args[{p+"-fra-size", "N"}] =
		{"positive_int",
		 "number of symbols by frame."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-type"}] =
		{"string",
		 "type of the modulation to use in the simulation.",
		 "BPSK, BPSK_FAST, PSK, PAM, QAM, CPM, USER, SCMA"};

	opt_args[{p+"-bps"}] =
		{"positive_int",
		 "select the number of bits per symbol (default is 1)."};

	opt_args[{p+"-ups"}] =
		{"positive_int",
		 "select the symbol sampling factor (default is 1)."};

	opt_args[{p+"-const-path"}] =
		{"string",
		 "path to the ordered modulation symbols (constellation), to use with \"--mod-type USER\"."};

	opt_args[{p+"-cpm-std"}] =
		{"string",
		 "the selection of a default CPM standard hardly implemented (any of those parameters is "
		   "overwritten if the argument is given by the user)",
		 "GSM"};

	opt_args[{p+"-cpm-L"}] =
		{"positive_int",
		 "CPM pulse width or CPM memory (default is 2)"};

	opt_args[{p+"-cpm-k"}] =
		{"positive_int",
		 "modulation index numerator (default is 1)"};

	opt_args[{p+"-cpm-p"}] =
		{"positive_int",
		 "modulation index denominator (default is 2)"};

	opt_args[{p+"-cpm-map"}] =
		{"string",
		 "symbols mapping layout (default is NATURAL)",
		 "NATURAL, GRAY"};

	opt_args[{p+"-cpm-ws"}] =
		{"string",
		 "wave shape (default is GMSK)",
		 "GMSK, REC, RCOS"};

	// --------------------------------------------------------------------------------------------------- demodulator
	opt_args[{p+"-max"}] =
		{"string",
		 "select the type of the max operation to use in the demodulator.",
		 "MAX, MAXL, MAXS, MAXSS"};

	opt_args[{p+"-sigma"}] =
		{"posive_float",
		 "noise variance value for the demodulator."};

	opt_args[{p+"-no-sig2"}] =
		{"",
		 "turn off the division by sigma square in the demodulator."};

	opt_args[{p+"-psi"}] =
		{"string",
		 "select the type of the psi function to use in the SCMA demodulator.",
		 "PSI0, PSI1, PSI2, PSI3"};

	opt_args[{p+"-ite"}] =
		{"positive int",
		 "select the number of iteration in the demodulator."};
}

void Modem
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	// ----------------------------------------------------------------------------------------------------- modulator
	if(exist(vals, {p+"-fra-size", "N"})) params.N          = std::stoi(vals.at({p+"-fra-size", "N"}));
	if(exist(vals, {p+"-fra",      "F"})) params.n_frames   = std::stoi(vals.at({p+"-fra",      "F"}));
	if(exist(vals, {p+"-type"         })) params.type       =           vals.at({p+"-type"         });
	if(exist(vals, {p+"-cpm-std"      })) params.cpm_std    =           vals.at({p+"-cpm-std"      });
	if(exist(vals, {p+"-bps"          })) params.bps        = std::stoi(vals.at({p+"-bps"          }));
	if(exist(vals, {p+"-ups"          })) params.upf        = std::stoi(vals.at({p+"-ups"          }));
	if(exist(vals, {p+"-const-path"   })) params.const_path =           vals.at({p+"-const-path"   });
	if(exist(vals, {p+"-cpm-L"        })) params.cpm_L      = std::stoi(vals.at({p+"-cpm-L"        }));
	if(exist(vals, {p+"-cpm-p"        })) params.cpm_p      = std::stoi(vals.at({p+"-cpm-p"        }));
	if(exist(vals, {p+"-cpm-k"        })) params.cpm_k      = std::stoi(vals.at({p+"-cpm-k"        }));
	if(exist(vals, {p+"-cpm-map"      })) params.mapping    =           vals.at({p+"-cpm-map"      });
	if(exist(vals, {p+"-cpm-ws"       })) params.wave_shape =           vals.at({p+"-cpm-ws"       });

	if (params.type.find("BPSK") != std::string::npos || params.type == "PAM")
		params.complex = false;

	if (params.type == "CPM")
	{
		if (!params.cpm_std.empty())
		{
			if (params.cpm_std == "GSM")
			{
				params.cpm_L      = 3;
				params.cpm_k      = 1;
				params.cpm_p      = 2;
				params.bps        = 1;
				params.upf        = 5;
				params.mapping    = "NATURAL";
				params.wave_shape = "GMSK";
			}
			else
			{
				std::stringstream message;
				message << "Unknown CPM standard ('cpm_std' = " << params.cpm_std << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		}
	}

	// force the number of bits per symbol to 1 when BPSK mod
	if (params.type == "BPSK" || params.type == "BPSK_FAST")
		params.bps = 1;
	// force the number of bits per symbol to 3 when SCMA mod
	if (params.type == "SCMA")
		params.bps = 3;

	params.N_mod = get_buffer_size_after_modulation(params.type,
	                                                params.N,
	                                                params.bps,
	                                                params.upf,
	                                                params.cpm_L);

	params.N_fil = get_buffer_size_after_filtering (params.type,
	                                                params.N,
	                                                params.bps,
	                                                params.cpm_L,
	                                                params.cpm_p);

	// --------------------------------------------------------------------------------------------------- demodulator
	if(exist(vals, {p+"-no-sig2"})) params.no_sig2 = true;
	if(exist(vals, {p+"-sigma"  })) params.sigma   = std::stof(vals.at({p+"-sigma"}));
	if(exist(vals, {p+"-ite"    })) params.n_ite   = std::stoi(vals.at({p+"-ite"  }));
	if(exist(vals, {p+"-max"    })) params.max     =           vals.at({p+"-max"  });
	if(exist(vals, {p+"-psi"    })) params.psi     =           vals.at({p+"-psi"  });
}

void Modem
::make_header(params_list& head_mdm, const parameters& params)
{
	// ----------------------------------------------------------------------------------------------------- modulator
	head_mdm.push_back(std::make_pair("Type", params.type));
	head_mdm.push_back(std::make_pair("Frame size (N)", std::to_string(params.N)));
	head_mdm.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
	if (params.type == "CPM")
	{
		if(params.cpm_std.size())
			head_mdm.push_back(std::make_pair("CPM standard", params.cpm_std));

		head_mdm.push_back(std::make_pair("CPM L memory", std::to_string(params.cpm_L)));
		head_mdm.push_back(std::make_pair("CPM h index", (std::to_string(params.cpm_k) + std::string("/") +
		                                                  std::to_string(params.cpm_p))));
		head_mdm.push_back(std::make_pair("CPM wave shape", params.wave_shape));
		head_mdm.push_back(std::make_pair("CPM mapping", params.mapping));
	}

	head_mdm.push_back(std::make_pair("Bits per symbol", std::to_string(params.bps)));
	head_mdm.push_back(std::make_pair("Sampling factor", std::to_string(params.upf)));

	// --------------------------------------------------------------------------------------------------- demodulator
	std::string demod_sig2 = (params.no_sig2) ? "off" : "on";
	std::string demod_max  = (params.type == "BPSK"     ) ||
	                         (params.type == "BPSK_FAST") ||
	                         (params.type == "SCMA"     ) ?
	                         "unused" : params.max;
	std::string demod_ite  = std::to_string(params.n_ite);
	std::string demod_psi  = params.psi;

	if (params.sigma != -1.f)
		head_mdm.push_back(std::make_pair("Sigma value", std::to_string(params.sigma)));
	head_mdm.push_back(std::make_pair("Sigma square", demod_sig2));
	head_mdm.push_back(std::make_pair("Max type", demod_max));
	if (params.type == "SCMA")
	{
		head_mdm.push_back(std::make_pair("Number of iterations", demod_ite));
		head_mdm.push_back(std::make_pair("Psi function", demod_psi));
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Modem<B_8 ,R_8 ,Q_8 >* aff3ct::factory::Modem::build<B_8 ,R_8 ,Q_8 >(const aff3ct::factory::Modem::parameters&);
template aff3ct::module::Modem<B_8 ,R_8 ,R_8 >* aff3ct::factory::Modem::build<B_8 ,R_8 ,R_8 >(const aff3ct::factory::Modem::parameters&);
template aff3ct::module::Modem<B_16,R_16,Q_16>* aff3ct::factory::Modem::build<B_16,R_16,Q_16>(const aff3ct::factory::Modem::parameters&);
template aff3ct::module::Modem<B_16,R_16,R_16>* aff3ct::factory::Modem::build<B_16,R_16,R_16>(const aff3ct::factory::Modem::parameters&);
template aff3ct::module::Modem<B_32,R_32,Q_32>* aff3ct::factory::Modem::build<B_32,R_32,Q_32>(const aff3ct::factory::Modem::parameters&);
template aff3ct::module::Modem<B_64,R_64,Q_64>* aff3ct::factory::Modem::build<B_64,R_64,Q_64>(const aff3ct::factory::Modem::parameters&);
#else
template aff3ct::module::Modem<B,R,Q>* aff3ct::factory::Modem::build<B,R,Q>(const aff3ct::factory::Modem::parameters&);
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template aff3ct::module::Modem<B,R,R>* aff3ct::factory::Modem::build<B,R,R>(const aff3ct::factory::Modem::parameters&);
#endif
#endif
// ==================================================================================== explicit template instantiation
