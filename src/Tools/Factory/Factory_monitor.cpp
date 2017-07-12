#include "Tools/Exception/exception.hpp"

#include "Module/Monitor/Standard/Monitor_std.hpp"

#include "Factory_monitor.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Monitor<B>* Factory_monitor
::build(const parameters& params)
{
	if (params.type == "STD") return new Monitor_std<B>(params.size, params.n_frame_errors, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_monitor
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ------------------------------------------------------------------------------------------------------- monitor
	opt_args[{"mnt-max-fe", "e"}] =
		{"positive_int",
		 "max number of frame errors for each SNR simulation."};

	opt_args[{"mnt-err-trk"}] =
		{"",
		 "enable the tracking of the bad frames (by default the frames are stored in the current folder)."};

	opt_args[{"mnt-err-trk-rev"}] =
		{"",
		 "automatically replay the saved frames."};

	opt_args[{"mnt-err-trk-path"}] =
		{"string",
		 "base path for the files where the bad frames will be stored or read."};
}

void Factory_monitor
::store_args(const Arguments_reader& ar, parameters &params, const int size, const int n_frames)
{
	params.size     = size;
	params.n_frames = n_frames;

	// ------------------------------------------------------------------------------------------------------- monitor
	if(ar.exist_arg({"mnt-max-fe", "e" })) params.n_frame_errors   = ar.get_arg_int({"mnt-max-fe", "e"});

	if(ar.exist_arg({"mnt-err-trk-rev" })) params.err_track_revert = true;
	if(ar.exist_arg({"mnt-err-trk"     })) params.err_track_enable = true;
	if(ar.exist_arg({"mnt-err-trk-path"})) params.err_track_path   = ar.get_arg({"mnt-err-trk-path"});

	if(params.err_track_revert)
	{
		params.err_track_enable = false;
//		params.source. type = "USER";
//		params.encoder.type = "USER";
//		params.channel.type = "USER";
//		params.source. path = params.err_track_path + std::string("_$snr.src");
//		params.encoder.path = params.err_track_path + std::string("_$snr.enc");
//		params.channel.path = params.err_track_path + std::string("_$snr.chn");
		// the paths are set in the Simulation class
	}

//	if (params.err_track_revert && !(ar.exist_arg({"sim-threads", "t"}) && ar.get_arg_int({"sim-threads", "t"}) > 0))
//		params.simulation.n_threads = 1;
}

void Factory_monitor
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"mnt", "Monitor parameter(s)"});
}

void Factory_monitor
::header(Header::params_list& head_mon, const parameters& params)
{
	// ------------------------------------------------------------------------------------------------------- monitor

	head_mon.push_back(std::make_pair("Frame error count (e)", std::to_string(params.n_frame_errors)));

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
template aff3ct::module::Monitor<B_8 >* aff3ct::tools::Factory_monitor::build<B_8 >(const aff3ct::tools::Factory_monitor::parameters&);
template aff3ct::module::Monitor<B_16>* aff3ct::tools::Factory_monitor::build<B_16>(const aff3ct::tools::Factory_monitor::parameters&);
template aff3ct::module::Monitor<B_32>* aff3ct::tools::Factory_monitor::build<B_32>(const aff3ct::tools::Factory_monitor::parameters&);
template aff3ct::module::Monitor<B_64>* aff3ct::tools::Factory_monitor::build<B_64>(const aff3ct::tools::Factory_monitor::parameters&);
#else
template aff3ct::module::Monitor<B>* aff3ct::tools::Factory_monitor::build<B>(const aff3ct::tools::Factory_monitor::parameters&);
#endif
// ==================================================================================== explicit template instantiation
