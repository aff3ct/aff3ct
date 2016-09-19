#include <cmath>
#include <chrono>
#include <iostream>

#include "Tools/Display/bash_tools.h"

#include "Launcher.hpp"

template <typename B, typename R, typename Q>
Launcher<B,R,Q>
::Launcher(const int argc, const char **argv, std::ostream &stream)
: ar(argc, argv), simu(nullptr), stream(stream)
{
	// define type names
	type_names[typeid(char)]        = "char ("        + std::to_string(sizeof(char)*8)        + " bits)";
	type_names[typeid(signed char)] = "signed char (" + std::to_string(sizeof(signed char)*8) + " bits)";
	type_names[typeid(short)]       = "short ("       + std::to_string(sizeof(short)*8)       + " bits)";
	type_names[typeid(int)]         = "int ("         + std::to_string(sizeof(int)*8)         + " bits)";
	type_names[typeid(long long)]   = "long long ("   + std::to_string(sizeof(long long)*8)   + " bits)";
	type_names[typeid(float)]       = "float ("       + std::to_string(sizeof(float)*8)       + " bits)";
	type_names[typeid(double)]      = "double ("      + std::to_string(sizeof(double)*8)      + " bits)";

	// default parameters
	params.simulation.snr_step        = 0.1f;
	params.simulation.disable_display = false;
	params.simulation.n_threads       = 1;
	params.simulation.stop_time       = std::chrono::seconds(0);
	params.simulation.display_freq    = std::chrono::milliseconds(500);
	params.code.tail_length           = 0;
	params.code.generation_method     = "RAND";
	params.channel.domain             = "LLR";
	params.channel.type               = "AWGN";
	params.modulator.type             = "BPSK";
	params.modulator.bits_per_symbol  = 1;
	params.modulator.upsample_factor  = 1;
	params.modulator.demod_max        = "MAXSS";

	params.channel.quant_min_max      = 0.f;
	if (typeid(R) == typeid(double))
		params.channel.quantizer_type = "STD";
	else
		params.channel.quantizer_type = "STD_FAST";

	params.modulator.disable_demod_sig2 = false;
}

template <typename B, typename R, typename Q>
Launcher<B,R,Q>
::~Launcher() 
{
	if (simu != nullptr) delete simu;
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::build_args()
{
	// ---------------------------------------------------------------------------------------------------- simulation
	req_args[{"sim-snr-min", "m"}] =
		{"float",
		 "minimal signal/noise ratio to simulate."};
	req_args[{"sim-snr-max", "M"}] =
		{"float",
		 "maximal signal/noise ratio to simulate."};
	opt_args[{"sim-snr-step", "s"}] =
		{"positive_float",
		 "signal/noise ratio step between each simulation."};
	opt_args[{"sim-type"}] =
		{"string",
		 "select the type of simulation to launch (default is BFER).",
		 "BFER, BFERI, EXIT, GEN"};
	opt_args[{"sim-stop-time"}] =
		{"positive_int",
		 "time in sec after what the current SNR iteration should stop."};
	opt_args[{"sim-threads", "t"}] =
		{"positive_int",
		 "enable multi-threaded mode and specify the number of threads."};
	opt_args[{"sim-domain"}] =
		{"string",
		 "choose the domain in which you want to compute.",
		 "LR, LLR"};
#ifdef MULTI_PREC
	opt_args[{"sim-prec", "p"}] =
		{"positive_int",
		 "the simulation precision in bit.",
		 "8, 16, 32, 64"};
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	req_args[{"cde-type"}] =
		{"string",
		 "select the code type you want to use.",
		 "POLAR, TURBO, REPETITION, RA, RSC, UNCODED" };
	req_args[{"cde-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (only information bits)."};
	req_args[{"cde-size", "N"}] =
		{"positive_int",
		 "total number of bit transmitted (includes parity bits)."};

	// -------------------------------------------------------------------------------------------------------- source
	opt_args[{"src-type"}] =
		{"string",
		 "method used to generate the codewords.",
		 "RAND, RAND_FAST, AZCW"};

	// ----------------------------------------------------------------------------------------------------- modulator
	opt_args[{"mod-type"}] =
		{"string",
		 "type of the modulation to use in the simulation.",
		 "BPSK, BPSK_FAST, PSK, PAM, QAM, GSM, GSM_TBLESS"};
	opt_args[{"mod-bps"}] =
		{"positive_int",
		 "select the number of bits per symbol (default is 1)."};
	opt_args[{"mod-ups"}] =
		{"positive_int",
		 "select the symbol upsample factor (default is 1)."};

	// --------------------------------------------------------------------------------------------------- demodulator
	opt_args[{"dmod-max"}] =
		{"string",
		 "select the type of the max operation to use in the demodulation.",
		 "MAX, MAXL, MAXS, MAXSS"};
	opt_args[{"dmod-no-sig2"}] =
		{"",
		 "turn off the division by sigma square in the demodulation."};

	// ------------------------------------------------------------------------------------------------------- channel
	std::string chan_avail = "NO, AWGN, AWGN_FAST";
#ifdef CHANNEL_GSL
	chan_avail += ", AWGN_GSL";
#endif 
#ifdef CHANNEL_MKL
	chan_avail += ", AWGN_MKL";
#endif
	chan_avail += ", ";
	opt_args[{"chn-type"}] =
		{"string",
		 "type of the channel to use in the simulation.",
		 chan_avail};

	// ----------------------------------------------------------------------------------------------------- quantizer
	if ((typeid(Q) != typeid(float)) && (typeid(Q) != typeid(double)))
	{
		opt_args[{"qnt-type"}] =
			{"string",
			 "type of the quantizer to use in the simulation.",
			 "STD, STD_FAST, TRICKY"};
		opt_args[{"qnt-dec"}] =
			{"positive_int",
			 "the position of the fixed point in the quantified representation."};
		opt_args[{"qnt-bits"}] =
			{"positive_int",
			 "the number of bits used for the quantizer."};
		opt_args[{"qnt-range"}] =
			{"positive_float",
			 "the min/max bound for the tricky quantizer."};
	}

	// ------------------------------------------------------------------------------------------------------- decoder
	opt_args[{"dec-type", "D"}] =
		{"string",
		 "select the algorithm you want to decode the codeword."};
	opt_args[{"dec-implem"}] =
		{"string",
		 "select the implementation of the algorithm to decode."};

	// ------------------------------------------------------------------------------------------------------ terminal
	opt_args[{"term-no"}] =
		{"",
		 "disable reporting for each iteration."};
	opt_args[{"term-freq"}] =
		{"positive_int",
		 "display frequency in ms (refresh time step for each iteration, 0 = disable display refresh)."};

	// --------------------------------------------------------------------------------------------------------- other
	opt_args[{"version", "v"}] =
		{"",
		 "print informations about the version of the code."};
	opt_args[{"help", "h"}] =
		{"",
		 "print this help."};
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::store_args()
{
	using namespace std::chrono;

	// ---------------------------------------------------------------------------------------------------- simulation
	params.simulation.snr_min = ar.get_arg_float({"sim-snr-min", "m"}); // required
	params.simulation.snr_max = ar.get_arg_float({"sim-snr-max", "M"}); // required

	if(ar.exist_arg({"sim-type"         })) params.simulation.type      = ar.get_arg      ({"sim-type"         });
	if(ar.exist_arg({"sim-snr-step", "s"})) params.simulation.snr_step  = ar.get_arg_float({"sim-snr-step", "s"});
	if(ar.exist_arg({"sim-threads",  "t"})) params.simulation.n_threads = ar.get_arg_int  ({"sim-threads",  "t"});
	if(ar.exist_arg({"sim-domain"       })) params.channel.domain       = ar.get_arg      ({"sim-domain"       });
	if(ar.exist_arg({"sim-stop-time"    })) params.simulation.stop_time = seconds(ar.get_arg_int({"sim-stop-time"}));

	// ---------------------------------------------------------------------------------------------------------- code
	params.code.type   = ar.get_arg    ({"cde-type"          }); // required
	params.code.K      = ar.get_arg_int({"cde-info-bits", "K"}); // required
	params.code.N      = ar.get_arg_int({"cde-size",      "N"}); // required
	params.code.N_code = ar.get_arg_int({"cde-size",      "N"});
	params.code.m      = std::ceil(std::log2(params.code.N));
	if (params.code.K > params.code.N)
	{
		std::cerr << bold_red("(EE) K have to be smaller than N, exiting.") << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// -------------------------------------------------------------------------------------------------------- source
	if(ar.exist_arg({"src-type"})) params.code.generation_method = ar.get_arg({"src-type"});

	// ----------------------------------------------------------------------------------------------------- modulator
	if(ar.exist_arg({"mod-type"})) params.modulator.type            = ar.get_arg      ({"mod-type"});
	if(ar.exist_arg({"mod-type"})) params.modulator.type            = ar.get_arg      ({"mod-type"});
	if(ar.exist_arg({"mod-bps" })) params.modulator.bits_per_symbol = ar.get_arg_float({"mod-bps" });
	if(ar.exist_arg({"mod-ups" })) params.modulator.upsample_factor = ar.get_arg_int  ({"mod-ups" });

	// force the number of bits per symbol to 1 when BPSK mod
	if (params.modulator.type == "BPSK" || params.modulator.type == "BPSK_FAST")
		params.modulator.bits_per_symbol = 1;

	if (params.modulator.type == "GSM" || params.modulator.type == "GSM_TBLESS")
	{
		params.modulator.bits_per_symbol = 1;
		params.modulator.upsample_factor = 5;
	}

	// --------------------------------------------------------------------------------------------------- demodulator
	if(ar.exist_arg({"dmod-no-sig2"})) params.modulator.disable_demod_sig2 = true;
	if(ar.exist_arg({"dmod-max"    })) params.modulator.demod_max          = ar.get_arg({"dmod-max"});

	// ------------------------------------------------------------------------------------------------------- channel
	if(ar.exist_arg({"chn-type"})) params.channel.type = ar.get_arg({"chn-type"});

	// ----------------------------------------------------------------------------------------------------- quantizer
	if ((typeid(Q) != typeid(float)) && (typeid(Q) != typeid(double)))
	{
		if(ar.exist_arg({"qnt-type" })) params.channel.quantizer_type  = ar.get_arg      ({"qnt-type" });
		if(ar.exist_arg({"qnt-dec"  })) params.channel.quant_point_pos = ar.get_arg_int  ({"qnt-dec"  });
		if(ar.exist_arg({"qnt-bits" })) params.channel.quant_n_bits    = ar.get_arg_int  ({"qnt-bits" });
		if(ar.exist_arg({"qnt-range"})) params.channel.quant_min_max   = ar.get_arg_float({"qnt-range"});
	}

	// ------------------------------------------------------------------------------------------------------- decoder
	if(ar.exist_arg({"dec-type",  "D"})) params.decoder.algo   = ar.get_arg({"dec-type",  "D"});
	if(ar.exist_arg({"dec-implem"    })) params.decoder.implem = ar.get_arg({"dec-implem"    });

	// ------------------------------------------------------------------------------------------------------ terminal
	if(ar.exist_arg({"term-no"  })) params.simulation.disable_display = true;
	if(ar.exist_arg({"term-freq"})) params.simulation.display_freq    = milliseconds(ar.get_arg_int({"term-freq"}));
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::read_arguments()
{
	this->build_args();

	opt_args[{"help", "h"}] =
		{"",
		 "print this help."};

	auto display_help = true;
	if (ar.parse_arguments(req_args, opt_args))
	{
		this->store_args();

		display_help = false;

		// print usage if there is "-h" or "--help" on the command line
		if(ar.exist_arg({"help", "h"})) display_help = true;
	}

	if (display_help)
	{
		std::vector<std::vector<std::string>> arg_grp;
		arg_grp.push_back({"sim",  "Simulation parameter(s)" });
		arg_grp.push_back({"cde",  "Code parameter(s)"       });
		arg_grp.push_back({"src",  "Source parameter(s)"     });
		arg_grp.push_back({"crc",  "CRC parameter(s)"        });
		arg_grp.push_back({"itl",  "Interleaver parameter(s)"});
		arg_grp.push_back({"pct",  "Puncturer parameter(s)"  });
		arg_grp.push_back({"dpct", "Depuncturer parameter(s)"});
		arg_grp.push_back({"enc",  "Encoder parameter(s)"    });
		arg_grp.push_back({"mod",  "Modulator parameter(s)"  });
		arg_grp.push_back({"dmod", "Demodulator parameter(s)"});
		arg_grp.push_back({"chn",  "Channel parameter(s)"    });
		arg_grp.push_back({"qnt",  "Quantizer parameter(s)"  });
		arg_grp.push_back({"dec",  "Decoder parameter(s)"    });
		arg_grp.push_back({"mnt",  "Monitor parameter(s)"    });
		arg_grp.push_back({"term", "Terminal parameter(s)"   });

		ar.print_usage(arg_grp);
		exit(EXIT_FAILURE);
	}

	if (!ar.check_arguments())
		std::exit(EXIT_FAILURE);
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::print_header()
{
	std::string N = std::to_string(params.code.N);
	if (params.code.tail_length > 0)
		N += " + " + std::to_string(params.code.tail_length) + " (tail bits)";

	std::string quantif = "unused";
	if (type_names[typeid(R)] != type_names[typeid(Q)])
	{
		if (params.channel.quantizer_type == "TRICKY")
			quantif = "{"+std::to_string(params.channel.quant_n_bits)+", "+std::to_string(params.channel.quant_min_max)+"f}";
		else
			quantif = "{"+std::to_string(params.channel.quant_n_bits)+", "+std::to_string(params.channel.quant_point_pos)+"}";
	}

	std::string demod_sig2 = (params.modulator.disable_demod_sig2) ? "off" : "on";
	std::string demod_max  = (params.modulator.type == "BPSK") ||
	                           (params.modulator.type == "BPSK_FAST") ?
	                           "unused" : params.modulator.demod_max;

	std::string modulation = std::to_string((int)(1 << params.modulator.bits_per_symbol)) + "-" + params.modulator.type;
	if ((params.modulator.type == "BPSK") || (params.modulator.type == "BPSK_FAST")|| (params.modulator.type == "GSM") ||
		(params.modulator.type == "GSM_TBLESS"))
		modulation = params.modulator.type;
	modulation += " (" + std::to_string(params.modulator.upsample_factor) + "-UPS)";

	// display configuration and simulation parameters
	stream << "# " << bold("-------------------------------------------------")                                 << std::endl;
	stream << "# " << bold("---- A FAST FORWARD ERROR CORRECTION TOOL >> ----")                                 << std::endl;
	stream << "# " << bold("-------------------------------------------------")                                 << std::endl;
	stream << "#"                                                                                               << std::endl;
	stream << "# " << bold_underlined("Simulation parameters:")                                                 << std::endl;
	stream << "# " << bold("* Simulation type               ") << " = " << params.simulation.type               << std::endl;
	stream << "# " << bold("* Code type                     ") << " = " << params.code.type << " codes"         << std::endl;
	stream << "# " << bold("* Number of information bits (K)") << " = " << params.code.K                        << std::endl;
	stream << "# " << bold("* Codeword length            (N)") << " = " << N                                    << std::endl;
	stream << "# " << bold("* SNR min                       ") << " = " << params.simulation.snr_min   << " dB" << std::endl;
	stream << "# " << bold("* SNR max                       ") << " = " << params.simulation.snr_max   << " dB" << std::endl;
	stream << "# " << bold("* SNR step                      ") << " = " << params.simulation.snr_step  << " dB" << std::endl;
	stream << "# " << bold("* Domain                        ") << " = " << params.channel.domain                << std::endl;
	stream << "# " << bold("* Codewords generation method   ") << " = " << params.code.generation_method        << std::endl;
	stream << "# " << bold("* Modulation type               ") << " = " << modulation                           << std::endl;
	stream << "# " << bold("* Demodulation sigma square     ") << " = " << demod_sig2                           << std::endl;
	stream << "# " << bold("* Demodulation max type         ") << " = " << demod_max                            << std::endl;
	stream << "# " << bold("* Channel type                  ") << " = " << params.channel.type                  << std::endl;
	stream << "# " << bold("* Type of bits               (B)") << " = " << type_names[typeid(B)]                << std::endl;
	stream << "# " << bold("* Type of reals              (R)") << " = " << type_names[typeid(R)]                << std::endl;

	if ((typeid(Q) != typeid(float)) && (typeid(Q) != typeid(double)))
	{
		stream << "# " << bold("* Type of quantified reals   (Q)") << " = " << type_names[typeid(Q)]         << std::endl;
		stream << "# " << bold("* Quantizer type                ") << " = " << params.channel.quantizer_type << std::endl;
		stream << "# " << bold("* Fixed-point representation    ") << " = " << quantif                       << std::endl;
	}
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::launch()
{
	this->read_arguments();
	this->print_header(); 
	stream << "#" << std::endl;
	this->build_simu();

	// launch the simulation
	stream << "# The simulation is running..." << std::endl;
	simu->launch();
	stream << "# End of the simulation." << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher<B_8,R_8,Q_8>;
template class Launcher<B_16,R_16,Q_16>;
template class Launcher<B_32,R_32,Q_32>;
template class Launcher<B_64,R_64,Q_64>;
#else
template class Launcher<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
