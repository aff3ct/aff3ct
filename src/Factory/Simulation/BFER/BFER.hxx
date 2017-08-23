#include <thread>

#include "BFER.hpp"

namespace aff3ct
{
namespace factory
{
template <class C>
void BFER::store_args(const arg_val_map &vals, parameters<C> &params, const std::string p)
{
#if !defined(STARPU) && !defined(SYSTEMC)
	params.n_threads = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;
#endif

	Simulation::store_args(vals, params, p);

	if(exist(vals, {p+"-snr-type",   "E"})) params.snr_type         = vals.at({p+"-snr-type", "E"});
	if(exist(vals, {p+"-err-trk-path"   })) params.err_track_path   = vals.at({p+"-err-trk-path" });
	if(exist(vals, {p+"-err-trk-rev"    })) params.err_track_revert = true;
	if(exist(vals, {p+"-err-trk"        })) params.err_track_enable = true;
	if(exist(vals, {p+"-stats"          })) params.statistics       = true;
	if(exist(vals, {p+"-debug",      "d"})) params.debug            = true;
	if(exist(vals, {p+"-coset",      "c"})) params.coset            = true;
	if(exist(vals, {p+"-coded",         })) params.coded_monitoring = true;
	if(exist(vals, {p+"-debug-limit"}))
	{
		params.debug = true;
		params.debug_limit = std::stoi(vals.at({p+"-debug-limit"}));
	}
	if(exist(vals, {p+"-debug-prec"}))
	{
		params.debug = true;
		params.debug_precision = std::stoi(vals.at({p+"-debug-prec"}));
	}

	if (params.debug && !(exist(vals, {p+"-threads", "t"}) && std::stoi(vals.at({p+"-threads", "t"})) > 0))
		// check if debug is asked and if n_thread kept its default value
		params.n_threads = 1;

	if (params.err_track_revert)
	{
		params.err_track_enable = false;
		params.n_threads = 1;
	}

	if (params.err_track_revert)
	{
		params.err_track_enable = false;
		if (params.src.type != "AZCW")
			params.src.type = "USER";
		if (params.coset)
			params.cdc.enc.type = "USER";
		params.chn.type = "USER";
		params.src.path     = params.err_track_path + std::string("_$snr.src");
		params.cdc.enc.path = params.err_track_path + std::string("_$snr.enc");
		params.chn.path     = params.err_track_path + std::string("_$snr.chn");
	}
}

template <class C>
void BFER::make_header(params_list& head_sim, const parameters<C> &params, const bool full)
{
	Simulation::make_header(head_sim, params, full);

	head_sim.push_back(std::make_pair("SNR type", params.snr_type));
	head_sim.push_back(std::make_pair("Statistics", params.statistics ? "on" : "off"));
	head_sim.push_back(std::make_pair("Debug mode", params.debug ? "on" : "off"));
	if (params.debug)
	{
		head_sim.push_back(std::make_pair("Debug precision", std::to_string(params.debug_precision)));
		if (params.debug_limit)
			head_sim.push_back(std::make_pair("Debug limit", std::to_string(params.debug_limit)));
	}
	head_sim.push_back(std::make_pair("Coset approach (c)", params.coset ? "yes" : "no"));
	head_sim.push_back(std::make_pair("Coded monitoring", params.coded_monitoring ? "yes" : "no"));

	std::string enable_track = (params.err_track_enable) ? "on" : "off";
	head_sim.push_back(std::make_pair("Bad frames tracking", enable_track));

	std::string enable_rev_track = (params.err_track_revert) ? "on" : "off";
	head_sim.push_back(std::make_pair("Bad frames replay", enable_rev_track));

	if (params.err_track_enable || params.err_track_revert)
	{
		std::string path = params.err_track_path + std::string("_$snr.[src,enc,chn]");
		head_sim.push_back(std::make_pair("Bad frames base path", path));
	}

	const auto bit_rate = (float)params.src.K / (float)params.cdc.N;
	head_sim.push_back(std::make_pair("Bit rate", std::to_string(bit_rate)));
	head_sim.push_back(std::make_pair("Inter frame level", std::to_string(params.src.n_frames)));
}
}
}
