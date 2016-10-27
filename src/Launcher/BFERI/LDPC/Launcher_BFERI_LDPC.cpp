#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/BFERI/LDPC/Simulation_BFERI_LDPC.hpp"

#include "Launcher_BFERI_LDPC.hpp"

template <typename B, typename R, typename Q>
Launcher_BFERI_LDPC<B,R,Q>
::Launcher_BFERI_LDPC(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFERI<B,R,Q>(argc, argv, stream)
{
	this->params.code       .type             = "LDPC";
	this->params.encoder    .type             = "COSET";
	this->params.interleaver.type             = "RANDOM";
	this->params.quantizer  .n_bits           = 6;
	this->params.quantizer  .n_decimals       = 2;
	this->params.decoder    .n_ite            = 1;
	this->params.decoder    .type             = "BP_FLOODING";
	this->params.decoder    .implem           = "SPA";
	this->params.decoder    .offset           = 0.f;
	this->params.decoder    .normalize_factor = 1.f;
	this->params.decoder    .enable_syndrome  = true;
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_LDPC<B,R,Q>
::build_args()
{
	Launcher_BFERI<B,R,Q>::build_args();

	// ---------------------------------------------------------------------------------------------------------- code
	this->req_args[{"cde-alist-path"}] =
		{"string",
		 "path to the AList formated file."};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-type", "D"}].push_back("BP, BP_FLOODING, BP_LAYERED");
	this->opt_args[{"dec-implem"   }].push_back("ONMS, SPA, LSPA");
	this->opt_args[{"dec-ite", "i"}] =
			{"positive_int",
			 "maximal number of iterations in the turbo decoder."};
	this->opt_args[{"dec-off"}] =
		{"float",
		 "offset used in the offset min-sum BP algorithm (works only with \"--dec-implem ONMS\")."};
	this->opt_args[{"dec-norm"}] =
		{"positive_float",
		 "normalization factor used in the normalized min-sum BP algorithm (works only with \"--dec-implem ONMS\")."};
	this->opt_args[{"dec-no-synd"}] =
		{"",
		 "disable the syndrome detection (disable the stop criterion in the LDPC decoders)."};
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_LDPC<B,R,Q>
::store_args()
{
	Launcher_BFERI<B,R,Q>::store_args();

	// ---------------------------------------------------------------------------------------------------------- code
	if(this->ar.exist_arg({"cde-alist-path"})) this->params.code.alist_path = this->ar.get_arg({"cde-alist-path"});

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-ite", "i"})) this->params.decoder.n_ite            = this->ar.get_arg_int  ({"dec-ite", "i"});
	if(this->ar.exist_arg({"dec-off"     })) this->params.decoder.offset           = this->ar.get_arg_float({"dec-off"     });
	if(this->ar.exist_arg({"dec-norm"    })) this->params.decoder.normalize_factor = this->ar.get_arg_float({"dec-norm"    });
	if(this->ar.exist_arg({"dec-no-synd" })) this->params.decoder.enable_syndrome  = false;

}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFERI_LDPC<B,R,Q>
::build_simu()
{
	return new Simulation_BFERI_LDPC<B,R,Q>(this->params);
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFERI_LDPC<B,R,Q>
::header_code()
{
	auto p = Launcher_BFERI<B,R,Q>::header_code();

	p.push_back(std::make_pair("AList file path", this->params.code.alist_path));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFERI_LDPC<B,R,Q>
::header_decoder()
{
	auto p = Launcher_BFERI<B,R,Q>::header_decoder();

	std::string syndrome = this->params.decoder.enable_syndrome ? "on" : "off";

	p.push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->params.decoder.n_ite)));
	if (this->params.decoder.implem == "ONMS")
	{
		p.push_back(std::make_pair("Offset", std::to_string(this->params.decoder.offset)));
		p.push_back(std::make_pair("Normalize factor", std::to_string(this->params.decoder.normalize_factor)));
	}
	p.push_back(std::make_pair("Syndrome detection", syndrome));

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFERI_LDPC<B_8,R_8,Q_8>;
template class Launcher_BFERI_LDPC<B_16,R_16,Q_16>;
template class Launcher_BFERI_LDPC<B_32,R_32,Q_32>;
template class Launcher_BFERI_LDPC<B_64,R_64,Q_64>;
#else
template class Launcher_BFERI_LDPC<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
