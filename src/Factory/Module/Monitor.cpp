#include "Tools/Exception/exception.hpp"

#include "Module/Monitor/Standard/Monitor_std.hpp"

#include "Monitor.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Monitor::name   = "Monitor";
const std::string aff3ct::factory::Monitor::prefix = "mnt";

template <typename B>
module::Monitor<B>* Monitor
::build(const parameters& params)
{
	if (params.type == "STD") return new module::Monitor_std<B>(params.size, params.n_frame_errors, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Monitor
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	req_args[{p+"-size", "K"}] =
		{"positive_int",
		 "number of bits to check."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-max-fe", "e"}] =
		{"positive_int",
		 "max number of frame errors for each SNR simulation."};

	opt_args[{p+"-err-trk"}] =
		{"",
		 "enable the tracking of the bad frames (by default the frames are stored in the current folder)."};

	opt_args[{p+"-err-trk-rev"}] =
		{"",
		 "automatically replay the saved frames."};

	opt_args[{p+"-err-trk-path"}] =
		{"string",
		 "base path for the files where the bad frames will be stored or read."};
}

void Monitor
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	if(exist(vals, {p+"-size",    "K"})) params.size             = std::stoi(vals.at({p+"-size",    "K"}));
	if(exist(vals, {p+"-fra",     "F"})) params.n_frames         = std::stoi(vals.at({p+"-fra",     "F"}));
	if(exist(vals, {p+"-max-fe",  "e"})) params.n_frame_errors   = std::stoi(vals.at({p+"-max-fe",  "e"}));
	if(exist(vals, {p+"-err-trk-path"})) params.err_track_path   =           vals.at({p+"-err-trk-path"});
	if(exist(vals, {p+"-err-trk-rev" })) params.err_track_revert = true;
	if(exist(vals, {p+"-err-trk"     })) params.err_track_enable = true;

	if(params.err_track_revert)
		params.err_track_enable = false;
}

void Monitor
::header(params_list& head_mon, const parameters& params)
{
	head_mon.push_back(std::make_pair("Frame error count (e)", std::to_string(params.n_frame_errors)));
	head_mon.push_back(std::make_pair("Size (K)", std::to_string(params.size)));
	head_mon.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));

	std::string enable_track = (params.err_track_enable) ? "on" : "off";
	head_mon.push_back(std::make_pair("Bad frames tracking", enable_track));

	std::string enable_rev_track = (params.err_track_revert) ? "on" : "off";
	head_mon.push_back(std::make_pair("Bad frames replay", enable_rev_track));

	if (params.err_track_enable || params.err_track_revert)
	{
		std::string path = params.err_track_path + std::string("_$snr.[src,enc,chn]");
		head_mon.push_back(std::make_pair("Bad frames base path", path));
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Monitor<B_8 >* aff3ct::factory::Monitor::build<B_8 >(const aff3ct::factory::Monitor::parameters&);
template aff3ct::module::Monitor<B_16>* aff3ct::factory::Monitor::build<B_16>(const aff3ct::factory::Monitor::parameters&);
template aff3ct::module::Monitor<B_32>* aff3ct::factory::Monitor::build<B_32>(const aff3ct::factory::Monitor::parameters&);
template aff3ct::module::Monitor<B_64>* aff3ct::factory::Monitor::build<B_64>(const aff3ct::factory::Monitor::parameters&);
#else
template aff3ct::module::Monitor<B>* aff3ct::factory::Monitor::build<B>(const aff3ct::factory::Monitor::parameters&);
#endif
// ==================================================================================== explicit template instantiation
