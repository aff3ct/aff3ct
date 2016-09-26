#include <cmath>
#include <chrono>
#include <iostream>

#include "Tools/bash_tools.h"

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
	simu_params.snr_step            = 0.1f;
	simu_params.disable_display     = false;
	simu_params.n_threads           = 1;
	simu_params.stop_time           = std::chrono::seconds(0);
	simu_params.display_freq        = std::chrono::milliseconds(500);
	code_params.tail_length         = 0;
	code_params.generation_method   = "RAND";
	chan_params.domain              = "LLR";
	chan_params.type                = "AWGN";
	mod_params.type                 = "BPSK";
	mod_params.bits_per_symbol      = 1;
	mod_params.upsample_factor      = 1;
	mod_params.demod_max            = "MAXSS";

	this->chan_params.quant_min_max = 0.f;
	if (typeid(R) == typeid(double))
		chan_params.quantizer_type  = "STD";
	else
		chan_params.quantizer_type  = "STD_FAST";

	mod_params.disable_demod_sig2   = false;
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
	req_args["K"                 ] = "n_bits";
	doc_args["K"                 ] = "useful number of bit transmitted (only information bits).";
	req_args["N"                 ] = "n_bits";
	doc_args["N"                 ] = "total number of bit transmitted (includes parity bits).";
	req_args["snr-min"           ] = "snr_min_value";
	doc_args["snr-min"           ] = "minimal signal/noise ratio to simulate.";
	req_args["snr-max"           ] = "snr_max_value";
	doc_args["snr-max"           ] = "maximal signal/noise ratio to simulate.";
	req_args["code-type"         ] = "code-type";
	doc_args["code-type"         ] = "select the code type you want to use (ex: POLAR, TURBO, REPETITION, RA, RSC, UNCODED).";
	opt_args["mod-type"          ] = "mod_type";
	doc_args["mod-type"          ] = "type of the modulation to use in the simulation (ex: BPSK, BPSK_FAST, PSK, PAM, QAM).";
	opt_args["mod-bps"           ] = "bps_number";
	doc_args["mod-bps"           ] = "select the number of bits per symbol (default is 1).";
	doc_args["mod-ups"           ] = "select the symbol upsample factor (default is 1).";
	opt_args["mod-ups"           ] = "ups_factor";
	opt_args["demod-max"         ] = "max_type";
	doc_args["demod-max"         ] = "select the type of the max operation to use in the demodulation (MAX, MAXL, MAXS or MAXSS).";
	opt_args["simu-type"         ] = "name";
	doc_args["simu-type"         ] = "select the type of simulation to launch (default is BFER).";
#ifdef MULTI_PREC
	opt_args["prec"              ] = "prec";
	doc_args["prec"              ] = "the simulation precision in bit (ex: 8, 16, 32 or 64).";
#endif
	opt_args["snr-step"          ] = "snr_step_value";
	doc_args["snr-step"          ] = "signal/noise ratio step between each simulation.";
	opt_args["disable-display"   ] = "";
	doc_args["disable-display"   ] = "disable reporting for each iteration.";
	opt_args["stop-time"         ] = "time_value";
	doc_args["stop-time"         ] = "time in sec after what the current SNR iteration should stop.";
	opt_args["display-freq"      ] = "freq_value";
	doc_args["display-freq"      ] = "display frequency in ms (refresh time step for each iteration, 0 = disable display refresh).";
	opt_args["n-threads"         ] = "n_threads";
	doc_args["n-threads"         ] = "enable multi-threaded mode and specify the number of threads.";
	opt_args["code-gen-method"   ] = "type";
	doc_args["code-gen-method"   ] = "method used to generate the codewords (RAND, RAND_FAST, AZCW).";
	opt_args["domain"            ] = "LR_or_LLR";
	doc_args["domain"            ] = "choose the domain in which you want to compute (LR or LLR).";

	std::string chan_avail = "ex: AWGN, AWGN_FAST";
#ifdef CHANNEL_GSL
	chan_avail += ", AWGN_GSL";
#endif 
#ifdef CHANNEL_MKL
	chan_avail += ", AWGN_MKL";
#endif
	chan_avail += ", ";

	opt_args["channel-type"      ] = "chan_type";
	doc_args["channel-type"      ] = "type of the channel to use in the simulation (" + chan_avail + "NO = disabled).";
	opt_args["disable-demod-sig2"] = "";
	doc_args["disable-demod-sig2"] = "turn off the division by sigma square in the demodulation.";
	opt_args["dec-algo"          ] = "alg_type";
	doc_args["dec-algo"          ] = "select the algorithm you want to decode the codeword.";
	opt_args["dec-implem"        ] = "impl_type";
	doc_args["dec-implem"        ] = "select the implementation of the algorithm to decode (ex: NAIVE, STD, FAST, VERY_FAST).";

	opt_args["v"                 ] = "";
	doc_args["v"                 ] = "print informations about the version of the code.";
	opt_args["version"           ] = "";
	doc_args["version"           ] = "print informations about the version of the code.";

	if ((typeid(Q) != typeid(float)) && (typeid(Q) != typeid(double)))
	{
		opt_args["quantizer-type"] = "quant_type";
		doc_args["quantizer-type"] = "type of the quantizer to use in the simulation (STD, STD_FAST or TRICKY).";
		opt_args["qpoint-pos"    ] = "point_pos";
		doc_args["qpoint-pos"    ] = "the position of the fixed point in the quantified representation.";
		opt_args["qn-bits"       ] = "n_bits";
		doc_args["qn-bits"       ] = "the number of bits used for the quantizer.";
		opt_args["qmin-max"      ] = "float_val";
		doc_args["qmin-max"      ] = "the min/max bound for the tricky quantizer.";
	}
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::store_args()
{
	using namespace std::chrono;

	// required parameters
	code_params.K      = std::stoi(ar.get_arg("K"));
	code_params.N      = std::stoi(ar.get_arg("N"));
	code_params.N_code = std::stoi(ar.get_arg("N"));
	code_params.m      = std::ceil(std::log2(code_params.N));

	if (code_params.K > code_params.N)
	{
		std::cerr << bold_red("(EE) K have to be smaller than N, exiting.") << std::endl;
		exit(EXIT_FAILURE);
	}

	simu_params.snr_min = std::stof(ar.get_arg("snr-min"));
	simu_params.snr_max = std::stof(ar.get_arg("snr-max"));

	code_params.type = ar.get_arg("code-type");

	// facultative parameters
	if(ar.exist_arg("simu-type"         )) simu_params.type              = ar.get_arg("simu-type");
	if(ar.exist_arg("snr-step"          )) simu_params.snr_step          = std::stof(ar.get_arg("snr-step"));
	if(ar.exist_arg("disable-display"   )) simu_params.disable_display   = true;
	if(ar.exist_arg("stop-time"         )) simu_params.stop_time         = seconds(std::stoi(ar.get_arg("stop-time")));
	if(ar.exist_arg("display-freq"      )) simu_params.display_freq      = milliseconds(std::stoi(ar.get_arg("display-freq")));
	if(ar.exist_arg("n-threads"         )) simu_params.n_threads         = std::stoi(ar.get_arg("n-threads"));
	if(ar.exist_arg("code-gen-method"   )) code_params.generation_method = ar.get_arg("code-gen-method");
	if(ar.exist_arg("domain"            )) chan_params.domain            = ar.get_arg("domain");
	if(ar.exist_arg("channel-type"      )) chan_params.type              = ar.get_arg("channel-type");
	if(ar.exist_arg("mod-type"          )) mod_params.type               = ar.get_arg("mod-type");
	if(ar.exist_arg("disable-demod-sig2")) mod_params.disable_demod_sig2 = true;
	if(ar.exist_arg("dec-algo"          )) deco_params.algo              = ar.get_arg("dec-algo");
	if(ar.exist_arg("dec-implem"        )) deco_params.implem            = ar.get_arg("dec-implem");

	if(ar.exist_arg("mod-type"          )) mod_params.type               = ar.get_arg("mod-type");
	if(ar.exist_arg("mod-bps"           )) mod_params.bits_per_symbol    = std::stof(ar.get_arg("mod-bps"));
	if(ar.exist_arg("mod-ups"           )) mod_params.upsample_factor    = std::stoi(ar.get_arg("mod-ups"));
	if(ar.exist_arg("demod-max"         )) mod_params.demod_max          = ar.get_arg("demod-max");

	if ((typeid(Q) != typeid(float)) && (typeid(Q) != typeid(double)))
	{
		if(ar.exist_arg("quantizer-type")) chan_params.quantizer_type    = ar.get_arg("quantizer-type");
		if(ar.exist_arg("qpoint-pos"    )) chan_params.quant_point_pos   = std::stoi(ar.get_arg("qpoint-pos"));
		if(ar.exist_arg("qn-bits"       )) chan_params.quant_n_bits      = std::stoi(ar.get_arg("qn-bits"));
		if(ar.exist_arg("qmin-max"      )) chan_params.quant_min_max     = std::stof(ar.get_arg("qmin-max"));
	}

	// force the number of bits per symbol to 1 when BPSK mod
	if (mod_params.type == "BPSK" || mod_params.type == "BPSK_FAST")
		mod_params.bits_per_symbol = 1;

	if (mod_params.type == "GSM" || mod_params.type == "GSM_TBLESS")
	{
		mod_params.bits_per_symbol = 1;
		mod_params.upsample_factor = 5;
	}
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::read_arguments()
{
	this->build_args();

	opt_args["h"   ] = "";
	doc_args["h"   ] = "print this help.";
	opt_args["help"] = "";
	doc_args["help"] = "print this help.";

	auto display_help = true;
	if (ar.parse_arguments(req_args, opt_args))
	{
		this->store_args();

		display_help = false;

		// print usage if there is "-h" or "--help" on the command line
		if(ar.exist_arg("h") || ar.exist_arg("help")) display_help = true;
	}

	if (display_help)
	{
		if (ar.parse_doc_args(doc_args))
			ar.print_usage();
		else
			std::cerr << bold_red("(EE) A problem was encountered when parsing arguments documentation, exiting.") 
			          << std::endl;
		exit(EXIT_FAILURE);
	}
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::print_header()
{
	std::string N = std::to_string(code_params.N);
	if (code_params.tail_length > 0) 
		N += " + " + std::to_string(code_params.tail_length) + " (tail bits)";

	std::string quantif = "unused";
	if (type_names[typeid(R)] != type_names[typeid(Q)])
	{
		if (chan_params.quantizer_type == "TRICKY")
			quantif = "{"+std::to_string(chan_params.quant_n_bits)+", "+std::to_string(chan_params.quant_min_max)+"f}";
		else
			quantif = "{"+std::to_string(chan_params.quant_n_bits)+", "+std::to_string(chan_params.quant_point_pos)+"}";
	}

	std::string demod_sig2 = (mod_params.disable_demod_sig2) ? "off" : "on";
	std::string demod_max  = (mod_params.type == "BPSK") || 
	                           (mod_params.type == "BPSK_FAST") ? 
	                           "unused" : mod_params.demod_max;

	std::string modulation = std::to_string((int)(1 << mod_params.bits_per_symbol)) + "-" + mod_params.type;
	if ((mod_params.type == "BPSK") || (mod_params.type == "BPSK_FAST")|| (mod_params.type == "GSM") || 
		(mod_params.type == "GSM_TBLESS"))
		modulation = mod_params.type;
	modulation += " (" + std::to_string(mod_params.upsample_factor) + "-UPS)";

	// display configuration and simulation parameters
	stream << "# " << bold("-------------------------------------------------")                           << std::endl;
	stream << "# " << bold("---- A FAST FORWARD ERROR CORRECTION TOOL >> ----")                           << std::endl;
	stream << "# " << bold("-------------------------------------------------")                           << std::endl;
	stream << "#"                                                                                         << std::endl;
	stream << "# " << bold_underlined("Simulation parameters:")                                           << std::endl;
	stream << "# " << bold("* Simulation type               ") << " = " << simu_params.type               << std::endl;
	stream << "# " << bold("* Code type                     ") << " = " << code_params.type << " codes"   << std::endl;
	stream << "# " << bold("* Number of information bits (K)") << " = " << code_params.K                  << std::endl;
	stream << "# " << bold("* Codeword length            (N)") << " = " << N                              << std::endl;
	stream << "# " << bold("* SNR min                       ") << " = " << simu_params.snr_min   << " dB" << std::endl;
	stream << "# " << bold("* SNR max                       ") << " = " << simu_params.snr_max   << " dB" << std::endl;
	stream << "# " << bold("* SNR step                      ") << " = " << simu_params.snr_step  << " dB" << std::endl;
	stream << "# " << bold("* Domain                        ") << " = " << chan_params.domain             << std::endl;
	stream << "# " << bold("* Codewords generation method   ") << " = " << code_params.generation_method  << std::endl;
	stream << "# " << bold("* Modulation type               ") << " = " << modulation                     << std::endl;
	stream << "# " << bold("* Demodulation sigma square     ") << " = " << demod_sig2                     << std::endl;
	stream << "# " << bold("* Demodulation max type         ") << " = " << demod_max                      << std::endl;
	stream << "# " << bold("* Channel type                  ") << " = " << chan_params.type               << std::endl;
	stream << "# " << bold("* Type of bits               (B)") << " = " << type_names[typeid(B)]          << std::endl;
	stream << "# " << bold("* Type of reals              (R)") << " = " << type_names[typeid(R)]          << std::endl;

	if ((typeid(Q) != typeid(float)) && (typeid(Q) != typeid(double)))
	{
		stream << "# " << bold("* Type of quantified reals   (Q)") << " = " << type_names[typeid(Q)]      << std::endl;
		stream << "# " << bold("* Quantizer type                ") << " = " << chan_params.quantizer_type << std::endl;
		stream << "# " << bold("* Fixed-point representation    ") << " = " << quantif                    << std::endl;
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
