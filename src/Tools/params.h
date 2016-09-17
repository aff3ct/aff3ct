#ifndef PARAMS_H_
#define PARAMS_H_

#include <vector>
#include <string>
#include <chrono>

struct simulation_parameters
{
	int                       max_fe;
	int                       benchs;
	bool                      disable_display;
	bool                      enable_debug;
	int                       debug_limit;
	bool                      time_report;
	int                       n_threads;
	bool                      enable_leg_term;
	bool                      enable_dec_thr;
	std::chrono::milliseconds display_freq;
	float                     snr_min;
	float                     snr_max;
	float                     snr_step;
	float                     sig_a_min;
	float                     sig_a_max;
	float                     sig_a_step;
	std::chrono::seconds      stop_time;
	std::string               type;
	std::string               trace_path_file;
	std::string               awgn_codes_dir;
	std::string               bin_pb_path;
	std::string               awgn_codes_file;
	std::string               gen_decoder_dir;
};

struct code_parameters
{
	int         K;
	int         N;
	int         N_code;
	int         tail_length;
	int         m;
	int         M;
	float       sigma; // not noise var, used to set a fixed snr value for frozen bits contstruction (in polar codes)
	std::string type;
	std::string generation_method;
	std::string crc;
	std::string interleaver;
	std::string fb_gen_method;
	bool        coset; // true = enable coset approach
};

struct source_parameters
{
};

struct crc_parameters
{
};

struct encoder_parameters
{
	std::string type;
	bool systematic;
	bool buffered;
};

struct puncturer_parameters
{
	std::string type;
};

struct interleaver_parameters
{
	std::string type;
};

struct modulator_parameters
{
	int          bits_per_symbol;    // bits per symbol
	int          upsample_factor;    // samples per symbol
	int          demod_n_ite;        // number of demodulations/decoding sessions to perform in the BFERI simulations
	std::string  type;               // modulation type (PAM, QAM, ...)
	std::string  demod_max;          // max to use in the demodulation (MAX = max, MAXL = max_linear, MAXS = max_star)
	bool         disable_demod_sig2; // do not divide by (sig^2) / 2 in the demodulation
};

struct channel_parameters
{
	int         quant_point_pos;
	int         quant_n_bits;
	float       quant_min_max;
	std::string domain;
	std::string type;
	std::string quantizer_type;
};

struct quantizer_parameters
{
};

struct decoder_parameters
{
	std::string algo;
	std::string implem;
	std::string simd_strategy;
	std::string scaling_factor;
	std::string max;
	int         max_iter;
	int         L;
};

struct monitor_parameters
{
};

struct terminal_parameters
{
};

struct parameters
{
	simulation_parameters  simulation;
	code_parameters        code;
	source_parameters      source;
	crc_parameters         crc;
	modulator_parameters   modulator;
	encoder_parameters     encoder;
	interleaver_parameters interleaver;
	puncturer_parameters   puncturer;
	channel_parameters     channel;
	quantizer_parameters   quantizer;
	decoder_parameters     decoder;
	monitor_parameters     monitor;
	terminal_parameters    terminal;
};

#endif /* PARAMS_H_ */
