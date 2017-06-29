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

#include "Factory_modem.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R, typename Q>
template <proto_max<Q> MAX>
Modem<B,R,Q>* Factory_modem<B,R,Q>
::_build(const std::string type,
         const int         N,
         const float       sigma,
         const int         bps,
         const std::string path,
         const int         upf,
         const int         cpm_L,
         const int         cpm_k,
         const int         cpm_p,
         const std::string mapping,
         const std::string wave,
         const bool        no_sig2,
         const int         n_frames)
{
	     if (type == "BPSK"     ) return new Modem_BPSK     <B,R,Q    >(N, sigma,                                               no_sig2, n_frames);
	else if (type == "BPSK_FAST") return new Modem_BPSK_fast<B,R,Q    >(N, sigma,                                               no_sig2, n_frames);
	else if (type == "PAM"      ) return new Modem_PAM      <B,R,Q,MAX>(N, sigma, bps,                                          no_sig2, n_frames);
	else if (type == "QAM"      ) return new Modem_QAM      <B,R,Q,MAX>(N, sigma, bps,                                          no_sig2, n_frames);
	else if (type == "PSK"      ) return new Modem_PSK      <B,R,Q,MAX>(N, sigma, bps,                                          no_sig2, n_frames);
	else if (type == "USER"     ) return new Modem_user     <B,R,Q,MAX>(N, sigma, bps, path,                                    no_sig2, n_frames);
	else if (type == "CPM"      ) return new Modem_CPM      <B,R,Q,MAX>(N, sigma, bps, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
Modem<B,R,Q>* Factory_modem<B,R,Q>
::_build_scma(const std::string type,
              const int         N,
              const float       sigma,
              const int         bps,
              const std::string psi_type,
              const bool        no_sig2,
              const int         n_ite,
              const int         n_frames)
{
	     if (psi_type == "PSI0") return new Modem_SCMA <B,R,Q,psi_0<Q>>(N, sigma, bps, no_sig2, n_ite, n_frames);
	else if (psi_type == "PSI1") return new Modem_SCMA <B,R,Q,psi_1<Q>>(N, sigma, bps, no_sig2, n_ite, n_frames);
	else if (psi_type == "PSI2") return new Modem_SCMA <B,R,Q,psi_2<Q>>(N, sigma, bps, no_sig2, n_ite, n_frames);
	else if (psi_type == "PSI3") return new Modem_SCMA <B,R,Q,psi_3<Q>>(N, sigma, bps, no_sig2, n_ite, n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
Modem<B,R,Q>* Factory_modem<B,R,Q>
::build(const std::string type,
        const int         N,
        const float       sigma,
        const std::string max_type,
        const std::string psi_type,
        const int         bps,
        const std::string path,
        const int         upf,
        const int         cpm_L,
        const int         cpm_k,
        const int         cpm_p,
        const std::string mapping,
        const std::string wave,
        const bool        no_sig2,
        const int         n_ite,
        const int         n_frames)
{
	if (type == "SCMA")
	{
		return _build_scma(type, N, sigma, bps, psi_type, no_sig2, n_ite, n_frames);
	}
	else
	{
		     if (max_type == "MAX"  ) return _build<max          <Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
		else if (max_type == "MAXL" ) return _build<max_linear   <Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
		else if (max_type == "MAXS" ) return _build<max_star     <Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
		else if (max_type == "MAXSS") return _build<max_star_safe<Q>>(type, N, sigma, bps, path, upf, cpm_L, cpm_k, cpm_p, mapping, wave, no_sig2, n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
int Factory_modem<B,R,Q>
::get_buffer_size_after_modulation(const std::string type,
                                   const int         N,
                                   const int         bps,
                                   const int         upf,
                                   const int         cpm_L)
{
	     if (type == "BPSK"     ) return Modem_BPSK     <B,R,Q>::size_mod(N                 );
	else if (type == "BPSK_FAST") return Modem_BPSK_fast<B,R,Q>::size_mod(N                 );
	else if (type == "SCMA"     ) return Modem_SCMA     <B,R,Q>::size_mod(N, bps            );
	else if (type == "PAM"      ) return Modem_PAM      <B,R,Q>::size_mod(N, bps            );
	else if (type == "QAM"      ) return Modem_QAM      <B,R,Q>::size_mod(N, bps            );
	else if (type == "PSK"      ) return Modem_PSK      <B,R,Q>::size_mod(N, bps            );
	else if (type == "USER"     ) return Modem_user     <B,R,Q>::size_mod(N, bps            );
	else if (type == "CPM"      ) return Modem_CPM      <B,R,Q>::size_mod(N, bps, cpm_L, upf);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
int Factory_modem<B,R,Q>
::get_buffer_size_after_filtering(const std::string type,
                                  const int         N,
                                  const int         bps,
                                  const int         cpm_L,
                                  const int         cpm_p)
{
	     if (type == "BPSK"     ) return Modem_BPSK     <B,R,Q>::size_fil(N                   );
	else if (type == "BPSK_FAST") return Modem_BPSK_fast<B,R,Q>::size_fil(N                   );
	else if (type == "SCMA"     ) return Modem_SCMA     <B,R,Q>::size_fil(N, bps              );
	else if (type == "PAM"      ) return Modem_PAM      <B,R,Q>::size_fil(N, bps              );
	else if (type == "QAM"      ) return Modem_QAM      <B,R,Q>::size_fil(N, bps              );
	else if (type == "PSK"      ) return Modem_PSK      <B,R,Q>::size_fil(N, bps              );
	else if (type == "USER"     ) return Modem_user     <B,R,Q>::size_fil(N, bps              );
	else if (type == "CPM"      ) return Modem_CPM      <B,R,Q>::size_fil(N, bps, cpm_L, cpm_p);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Factory_modem<B,R,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ----------------------------------------------------------------------------------------------------- modulator
	opt_args[{"mod-type"}] =
		{"string",
		 "type of the modulation to use in the simulation.",
		 "BPSK, BPSK_FAST, PSK, PAM, QAM, CPM, USER, SCMA"};

	opt_args[{"mod-bps"}] =
		{"positive_int",
		 "select the number of bits per symbol (default is 1)."};

	opt_args[{"mod-ups"}] =
		{"positive_int",
		 "select the symbol sampling factor (default is 1)."};

	opt_args[{"mod-const-path"}] =
		{"string",
		 "path to the ordered modulation symbols (constellation), to use with \"--mod-type USER\"."};

	opt_args[{"mod-cpm-std"}] =
		{"string",
		 "the selection of a default CPM standard hardly implemented (any of those parameters is "
		   "overwritten if the argument is given by the user)",
		 "GSM"};

	opt_args[{"mod-cpm-L"}] =
		{"positive_int",
		 "CPM pulse width or CPM memory (default is 2)"};

	opt_args[{"mod-cpm-k"}] =
		{"positive_int",
		 "modulation index numerator (default is 1)"};

	opt_args[{"mod-cpm-p"}] =
		{"positive_int",
		 "modulation index denominator (default is 2)"};

	opt_args[{"mod-cpm-map"}] =
		{"string",
		 "symbols mapping layout (default is NATURAL)",
		 "NATURAL, GRAY"};

	opt_args[{"mod-cpm-ws"}] =
		{"string",
		 "wave shape (default is GMSK)",
		 "GMSK, REC, RCOS"};

	// --------------------------------------------------------------------------------------------------- demodulator
	opt_args[{"dmod-max"}] =
		{"string",
		 "select the type of the max operation to use in the demodulation.",
		 "MAX, MAXL, MAXS, MAXSS"};

	opt_args[{"dmod-no-sig2"}] =
		{"",
		 "turn off the division by sigma square in the demodulation."};

	opt_args[{"dmod-psi"}] =
		{"string",
		 "select the type of the psi function to use in the SCMA demodulation.",
		 "PSI0, PSI1, PSI2, PSI3"};

	opt_args[{"dmod-ite"}] =
		{"positive int",
		 "select the number of iteration in the demodulator."};
}

template <typename B, typename R, typename Q>
void Factory_modem<B,R,Q>
::store_args(const Arguments_reader& ar, modem_parameters &params, const int N)
{

	// ----------------------------------------------------------------------------------------------------- modulator
	if(ar.exist_arg({"mod-type"})) params.type = ar.get_arg({"mod-type"});

	if (params.type.find("BPSK") != std::string::npos || params.type == "PAM")
		params.complex = false;


	if(ar.exist_arg({"mod-cpm-std"})) params.cpm_std = ar.get_arg({"mod-cpm-std"});
	if (params.type == "CPM")
	{
		if (!params.cpm_std.empty())
		{
			if (params.cpm_std == "GSM")
			{
				params.cpm_L          = 3;
				params.cpm_k          = 1;
				params.cpm_p          = 2;
				params.bits_per_symbol= 1;
				params.upsample_factor= 5;
				params.mapping        = "NATURAL";
				params.wave_shape     = "GMSK";
			}
			else
			{
				std::stringstream message;
				message << "Unknown CPM standard ('cpm_std' = " << params.cpm_std << ").";
				throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		}
	}

	if(ar.exist_arg({"mod-bps"       })) params.bits_per_symbol = ar.get_arg_int({"mod-bps"       });
	if(ar.exist_arg({"mod-ups"       })) params.upsample_factor = ar.get_arg_int({"mod-ups"       });
	if(ar.exist_arg({"mod-const-path"})) params.const_path      = ar.get_arg    ({"mod-const-path"});

	if(ar.exist_arg({"mod-cpm-L"     })) params.cpm_L           = ar.get_arg_int({"mod-cpm-L"     });
	if(ar.exist_arg({"mod-cpm-p"     })) params.cpm_p           = ar.get_arg_int({"mod-cpm-p"     });
	if(ar.exist_arg({"mod-cpm-k"     })) params.cpm_k           = ar.get_arg_int({"mod-cpm-k"     });
	if(ar.exist_arg({"mod-cpm-map"   })) params.mapping         = ar.get_arg    ({"mod-cpm-map"   });
	if(ar.exist_arg({"mod-cpm-ws"    })) params.wave_shape      = ar.get_arg    ({"mod-cpm-ws"    });

	// force the number of bits per symbol to 1 when BPSK mod
	if (params.type == "BPSK" || params.type == "BPSK_FAST")
		params.bits_per_symbol = 1;
	// force the number of bits per symbol to 3 when SCMA mod
	if (params.type == "SCMA")
		params.bits_per_symbol = 3;


	params.N_mod   = get_buffer_size_after_modulation(params.type,
	                                                  N,
	                                                  params.bits_per_symbol,
	                                                  params.upsample_factor,
	                                                  params.cpm_L);

	params.N_fil   = get_buffer_size_after_filtering (params.type,
	                                                  N,
	                                                  params.bits_per_symbol,
	                                                  params.cpm_L,
	                                                  params.cpm_p);

	// --------------------------------------------------------------------------------------------------- demodulator
	if(ar.exist_arg({"dmod-no-sig2"})) params.no_sig2 = true;
	if(ar.exist_arg({"dmod-ite"    })) params.n_ite   = ar.get_arg_int({"dmod-ite"});
	if(ar.exist_arg({"dmod-max"    })) params.max     = ar.get_arg    ({"dmod-max"});
	if(ar.exist_arg({"dmod-psi"    })) params.psi     = ar.get_arg    ({"dmod-psi"});
}

template <typename B, typename R, typename Q>
void Factory_modem<B,R,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"mod",  "Modulator parameter(s)"  });
	ar.push_back({"dmod", "Demodulator parameter(s)"});
}

template <typename B, typename R, typename Q>
void Factory_modem<B,R,Q>
::header(Header::params_list& head_mod, Header::params_list& head_demod, const modem_parameters& params)
{
	// ----------------------------------------------------------------------------------------------------- modulator
	head_mod.push_back(std::make_pair("Type", params.type));
	if (params.type == "CPM")
	{
		if(params.cpm_std.size())
			head_mod.push_back(std::make_pair("CPM standard", params.cpm_std));

		head_mod.push_back(std::make_pair("CPM L memory", std::to_string(params.cpm_L)    ));
		head_mod.push_back(std::make_pair("CPM h index", (std::to_string(params.cpm_k) +
		                                           std::string("/")                             +
		                                           std::to_string(params.cpm_p))   ));
		head_mod.push_back(std::make_pair("CPM wave shape",              params.wave_shape));
		head_mod.push_back(std::make_pair("CPM mapping",                 params.mapping   ));
	}

	head_mod.push_back(std::make_pair("Bits per symbol", std::to_string(params.bits_per_symbol)));
	head_mod.push_back(std::make_pair("Sampling factor", std::to_string(params.upsample_factor)));


	// --------------------------------------------------------------------------------------------------- demodulator
	std::string demod_sig2 = (params.no_sig2) ? "off" : "on";
	std::string demod_max  = (params.type == "BPSK"     ) ||
	                         (params.type == "BPSK_FAST") ||
	                         (params.type == "SCMA"     ) ?
	                         "unused" : params.max;
	std::string demod_ite  = std::to_string(params.n_ite);
	std::string demod_psi  = params.psi;

	head_demod.push_back(std::make_pair("Sigma square", demod_sig2));
	head_demod.push_back(std::make_pair("Max type",     demod_max ));
	if (params.type == "SCMA")
	{
		head_demod.push_back(std::make_pair("Number of iterations", demod_ite));
		head_demod.push_back(std::make_pair("Psi function",         demod_psi));
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_modem<B_8,R_8,R_8>;
template struct aff3ct::tools::Factory_modem<B_8,R_8,Q_8>;
template struct aff3ct::tools::Factory_modem<B_16,R_16,R_16>;
template struct aff3ct::tools::Factory_modem<B_16,R_16,Q_16>;
template struct aff3ct::tools::Factory_modem<B_32,R_32,R_32>;
template struct aff3ct::tools::Factory_modem<B_64,R_64,R_64>;
#else
template struct aff3ct::tools::Factory_modem<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template struct aff3ct::tools::Factory_modem<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
