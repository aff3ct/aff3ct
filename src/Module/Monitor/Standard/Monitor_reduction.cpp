#include "Monitor_reduction.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include <exception>

#include "Tools/Display/bash_tools.h"

template <typename B, typename R>
Monitor_reduction<B,R>
::Monitor_reduction(const int& K, const int& N, const int& max_fe, std::vector<Monitor<B,R>*>& monitors,
                    const int& n_frames, const std::string name)
: Monitor_std<B,R>(K, N, max_fe, n_frames, name),
  monitors        (monitors                    )
{
	assert(monitors.size() != 0);
	for (size_t i = 0; i < monitors.size(); ++i)
		assert(monitors[i] != nullptr);
}

template <typename B, typename R>
Monitor_reduction<B,R>
::~Monitor_reduction()
{
}

template <typename B, typename R>
unsigned long long Monitor_reduction<B,R>
::get_n_analyzed_fra() const
{
	unsigned long long cur_fra = this->n_analyzed_frames;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_fra += monitors[i]->get_n_analyzed_fra();

	return cur_fra;
}

template <typename B, typename R>
int Monitor_reduction<B,R>
::get_n_fe() const
{
	auto cur_fe = this->n_frame_errors;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_fe += monitors[i]->get_n_fe();

	return cur_fe;
}

template <typename B, typename R>
int Monitor_reduction<B,R>
::get_n_be() const
{
	auto cur_be = this->n_bit_errors;
	for (unsigned i = 0; i < monitors.size(); i++)
		cur_be += monitors[i]->get_n_be();

	return cur_be;
}

template <typename B, typename R>
bool Monitor_reduction<B,R>
::check_path(const std::string& error_tracker_head_path)
{
	size_t pos = error_tracker_head_path.find_last_of("/");

	if (pos == std::string::npos)
		return true;

	std::string pathname = error_tracker_head_path.substr(0, pos);

	// TODO : check compatibility on windows. Normally it is good.
	struct stat info;

	if( stat( pathname.data(), &info ) != 0 )
		return false;
	else if( info.st_mode & S_IFDIR )
		return true;
	else
		return false;
}

template <typename B, typename R>
void Monitor_reduction<B,R>
::get_tracker_paths(const std::string& error_tracker_head_path, const float snr,
                    std::string& path_src, std::string& path_enc, std::string& path_noise)
{
	if (!check_path(error_tracker_head_path))
	{
		std::cerr << bold_red("(EE) issue while trying to open error tracker log files ; check head of the path: \"")
		          << bold_red(error_tracker_head_path)
		          << bold_red("\" and please create yourself the needed directory.")
		          << std::endl;
		std::exit(-1);
	}

	std::stringstream snr_stream;
	snr_stream << std::fixed << std::setprecision(3) << snr;

	std::string path_head = error_tracker_head_path + std::string("_") + snr_stream.str();

	path_src   = path_head + std::string(".src");
	path_enc   = path_head + std::string(".enc");
	path_noise = path_head + std::string(".chn");
}

template <typename B, typename R>
void Monitor_reduction<B,R>
::flush_wrong_frames(const std::string& error_tracker_head_path, const float snr)
{
	// ************* get path and open them ******************
	std::string path_src, path_enc, path_noise;

	get_tracker_paths(error_tracker_head_path, snr, path_src, path_enc, path_noise);

	std::ofstream file_src  (path_src);
	std::ofstream file_enc  (path_enc);
	std::ofstream file_noise(path_noise, std::ios_base::binary);

	if (!file_src.is_open() || !file_enc.is_open() || !file_noise.is_open())
	{
		std::cerr << bold_red("(EE) issue while trying to open error tracker log files ; check file name: \"")
		          << bold_red(error_tracker_head_path) << bold_red("\"") << std::endl;
		std::exit(-1);
	}

	int n_fe   = get_n_fe();
	int Y_size = 0;
	// ************* get Y_size *******************
	for (unsigned i = 0; i <= monitors.size(); i++)
	{
		auto mon = (i == monitors.size()) ? this : monitors[i];

		// write noise
		auto buff_noise = mon->get_buff_noise();

		if(buff_noise.empty())
			continue;

		Y_size = buff_noise.front().size();
		break;
	}

	// ************* write headers *****************
	file_src << n_fe          << std::endl << std::endl; // write number frames
	file_src << this->get_K() << std::endl << std::endl; // write length of frames

	file_enc << n_fe          << std::endl << std::endl; // write number frames
	file_enc << this->get_K() << std::endl << std::endl; // write length of non coded frames
	file_enc << this->get_N() << std::endl << std::endl; // write length of coded frames

	file_noise.write((char *)&n_fe  , sizeof(int)); // write number frames
	file_noise.write((char *)&Y_size, sizeof(int)); // write length of frames

	// ************* write frames ********************
	for (unsigned i = 0; i <= monitors.size(); i++)
	{
		auto mon = (i == monitors.size()) ? this : monitors[i];

		// write source
		auto buff_src = mon->get_buff_src();
		for (unsigned f = 0; f < buff_src.size(); f++)
		{
			for (unsigned b = 0; b < buff_src[f].size(); b++)
				file_src << buff_src[f][b] << " ";

			file_src << std::endl << std::endl;
		}

		// write encoder
		auto buff_enc = mon->get_buff_enc();
		assert(buff_src.size() == buff_enc.size());
		for (unsigned f = 0; f < buff_enc.size(); f++)
		{
			for (unsigned b = 0; b < buff_enc[f].size(); b++)
				file_enc << buff_enc[f][b] << " ";

			file_enc << std::endl << std::endl;
		}

		// write noise
		auto buff_noise = mon->get_buff_noise();
		assert(buff_src.size() == buff_noise.size());
		for (unsigned f = 0; f < buff_noise.size(); f++)
			for (unsigned b = 0; b < buff_noise[f].size(); b++)
				file_noise.write((char *)&buff_noise[f][b], sizeof(R));
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Monitor_reduction<B_8 ,R_8>;
template class Monitor_reduction<B_16,R_16>;
template class Monitor_reduction<B_32,R_32>;
template class Monitor_reduction<B_64,R_64>;
#else
template class Monitor_reduction<B,R>;
#endif
// ==================================================================================== explicit template instantiation
