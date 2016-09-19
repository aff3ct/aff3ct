#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Launcher_BFER_turbo.hpp"
#include "../../../Simulation/BFER/Turbo/Simulation_BFER_turbo.hpp"

template <typename B, typename R, typename Q, typename QD>
Launcher_BFER_turbo<B,R,Q,QD>
::Launcher_BFER_turbo(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->params.code.tail_length = 4 * 3;
	if (typeid(Q) == typeid(short))
	{
		this->params.channel.quant_n_bits    = 6;
		this->params.channel.quant_point_pos = 3;
	}
	if (typeid(Q) == typeid(signed char))
	{
		this->params.channel.quant_n_bits    = 6;
		this->params.channel.quant_point_pos = 2;
	}

	// default parameters
	this->params.code.type                   = "TURBO";
	this->params.decoder.algo                = "LTE";
	this->params.decoder.implem              = "FAST";
	this->params.decoder.max                 = "MAX";

	this->params.decoder.max_iter            = 6;
	this->params.encoder.buffered            = true;
	this->params.code.interleaver            = "LTE";
	this->params.code.crc                    = "";
	this->params.decoder.scaling_factor      = "LTE_VEC";
	this->params.decoder.simd_strategy       = "";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_turbo<B,R,Q,QD>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	// ----------------------------------------------------------------------------------------------------------- crc
	this->opt_args[{"crc-type"}] =
		{"string",
		 "select the crc you want to use.",
		 "1-0x1, 2-0x1, 3-0x3, 4-ITU, 8-DVB-S2, 16-CCITT, 16-IBM, 24-LTEA, 32-GZIP"};

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

	// --------------------------------------------------------------------------------------------------- interleaver
	this->opt_args[{"itl-type"}] =
		{"string",
		 "specify the type of the interleaver.",
		 "LTE, CCSDS, RANDOM, COLUMNS, GOLDEN, NO"};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo decoder."};
	this->opt_args[{"dec-sf"}] =
		{"string",
		 "scaling factor type.",
		 "NO, LTE, LTE_VEC, ARRAY"};
	this->opt_args[{"dec-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};
	this->opt_args[{"dec-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXS, MAXL"};
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_turbo<B,R,Q,QD>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	// ----------------------------------------------------------------------------------------------------------- crc
	if(this->ar.exist_arg({"crc-type"})) this->params.code.crc = this->ar.get_arg({"crc-type"});

	// ------------------------------------------------------------------------------------------------------- encoder
	if(this->ar.exist_arg({"enc-no-buff"})) this->params.encoder.buffered = false;

	// --------------------------------------------------------------------------------------------------- interleaver
	if(this->ar.exist_arg({"itl-type"})) this->params.code.interleaver = this->ar.get_arg({"itl-type"});

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-ite", "i"})) this->params.decoder.max_iter       = this->ar.get_arg_int({"dec-ite", "i"});
	if(this->ar.exist_arg({"dec-sf"      })) this->params.decoder.scaling_factor = this->ar.get_arg    ({"dec-sf"      });
	if(this->ar.exist_arg({"dec-simd"    })) this->params.decoder.simd_strategy  = this->ar.get_arg    ({"dec-simd"    });
	if(this->ar.exist_arg({"dec-max"     })) this->params.decoder.max            = this->ar.get_arg    ({"dec-max"     });

	if (this->params.decoder.algo == "BCJR4" || this->params.decoder.algo == "CCSDS")
		this->params.code.tail_length = 4*4;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_turbo<B,R,Q,QD>
::print_header()
{
	Launcher_BFER<B,R,Q>::print_header();

	std::string buff_enc = (this->params.encoder.buffered) ? "on" : "off";

	// display configuration and simulation parameters
	this->stream << "# " << bold("* Decoding iterations per frame ") << " = " << this->params.decoder.max_iter       << std::endl;
	this->stream << "# " << bold("* Buffered encoding             ") << " = " << buff_enc                            << std::endl;
	this->stream << "# " << bold("* Interleaver                   ") << " = " << this->params.code.interleaver       << std::endl;
	if (!this->params.code.crc.empty())
	this->stream << "# " << bold("* CRC type                      ") << " = " << this->params.code.crc               << std::endl;
	this->stream << "# " << bold("* Scaling factor                ") << " = " << this->params.decoder.scaling_factor << std::endl;
	if (!this->params.decoder.simd_strategy.empty())
	this->stream << "# " << bold("* Decoder SIMD strategy         ") << " = " << this->params.decoder.simd_strategy  << std::endl;
	this->stream << "# " << bold("* Decoder MAX implementation    ") << " = " << this->params.decoder.max            << std::endl;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_turbo<B,R,Q,QD>
::build_simu()
{
	this->simu = new Simulation_BFER_turbo<B,R,Q,QD>(this->params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_turbo<B_8,R_8,Q_8,QD_8>;
template class Launcher_BFER_turbo<B_16,R_16,Q_16,QD_16>;
template class Launcher_BFER_turbo<B_32,R_32,Q_32,QD_32>;
template class Launcher_BFER_turbo<B_64,R_64,Q_64,QD_64>;
#else
template class Launcher_BFER_turbo<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
