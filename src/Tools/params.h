#ifndef PARAMS_H_
#define PARAMS_H_

#include <vector>
#include <string>
#include <chrono>

namespace aff3ct
{
namespace tools
{
struct simulation_parameters
{
	std::chrono::milliseconds mpi_comm_freq;
	std::chrono::seconds      stop_time;
	std::string               type;
	std::string               trace_path;
	std::string               bin_pb_path;
	std::string               json_path;
	std::string               pyber;
	std::string               snr_type;
	float                     snr_min;
	float                     snr_max;
	float                     snr_step;
	float                     sig_a_min;
	float                     sig_a_max;
	float                     sig_a_step;
	bool                      debug;
	bool                      benchs_no_ldst;
	bool                      time_report;
	int                       benchs;
	int                       debug_limit;
	int                       n_threads;
	int                       inter_frame_level;
	int                       seed;
	int                       mpi_rank;
	int                       mpi_size;
};

struct code_parameters
{
	std::string type;
	std::string fb_gen_method;
	std::string alist_path;
	std::string awgn_fb_path;
	float       sigma; // not noise var, used to set a fixed snr value for frozen bits construction (in polar codes)
	bool        coset; // true = enable coset approach
	int         K;
	int         N;
	int         N_code;
	int         m;
	int         M;
	int         tail_length;
};

struct source_parameters
{
	std::string type;
	std::string path;
};

struct crc_parameters
{
	std::string type;
};

struct encoder_parameters
{
	std::string      type;
	std::string      path;
	bool             systematic;
	bool             buffered;
	std::vector<int> poly;
};

struct interleaver_parameters
{
	std::string type;
	std::string path;
};

struct modulator_parameters
{
	std::string type;            // modulation type (PAM, QAM, ...)
	std::string const_path;      // PATH to constellation file (CSV file)
	bool        complex;         // true if the modulated signal is complex
	int         bits_per_symbol; // bits per symbol
	int         upsample_factor; // samples per symbol

	std::string cpm_std;         // the selection of a default cpm standard hardly implemented (GSM)
	std::string mapping;         // symbol mapping layout (natural, gray)
	std::string wave_shape;      // wave shape (gmsk, rcos, rec)
	int         cpm_L;           // cpm pulse width or cpm memory
	int         cpm_k;           // modulation index numerator
	int         cpm_p;           // modulation index denumerator
};

struct demodulator_parameters
{
	std::string max;     // max to use in the demodulation (MAX = max, MAXL = max_linear, MAXS = max_star)
	bool        no_sig2; // do not divide by (sig^2) / 2 in the demodulation
	int         n_ite;   // number of demodulations/decoding sessions to perform in the BFERI simulations
};

struct channel_parameters
{
	std::string type;
	std::string domain;
	std::string path;
	std::string block_fading;
};

struct quantizer_parameters
{
	std::string type;
	float       range;
	int         n_bits;
	int         n_decimals;
};

struct decoder_parameters
{
	std::string type;
	std::string implem;
	std::string simd_strategy;
	std::string scaling_factor;
	std::string max;
	std::string gen_path;
	bool        enable_syndrome;
	float       normalize_factor;
	float       offset;
	int         n_ite;
	int         L;
	bool        self_corrected;
};

struct monitor_parameters
{
	int n_frame_errors;
};

struct terminal_parameters
{
	std::chrono::milliseconds frequency;
	std::string               type;
	bool                      disabled;
};

struct parameters
{
	simulation_parameters  simulation;
	code_parameters        code;
	source_parameters      source;
	crc_parameters         crc;
	modulator_parameters   modulator;
	demodulator_parameters demodulator;
	encoder_parameters     encoder;
	interleaver_parameters interleaver;
	channel_parameters     channel;
	quantizer_parameters   quantizer;
	decoder_parameters     decoder;
	monitor_parameters     monitor;
	terminal_parameters    terminal;
};
}
}

#endif /* PARAMS_H_ */
