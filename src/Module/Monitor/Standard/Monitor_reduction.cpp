#include "Monitor_reduction.hpp"

template <typename B, typename R>
Monitor_reduction<B,R>
::Monitor_reduction(const int& K, const int& N, const int& Y_size, const int& max_fe,
                    std::vector<Monitor<B,R>*>& monitors,
                    const float snr,
                    const bool& error_tracker_enable,
                    const std::string& error_tracker_head_file_name,
                    const int& n_frames, const std::string name)
: Monitor_std<B,R>            (K, N, Y_size, max_fe, error_tracker_enable, n_frames, name),
  monitors                    (monitors                    ),
  snr                         (snr                         ),
  error_tracker_head_file_name(error_tracker_head_file_name)
{
	assert(monitors.size() != 0);
}

template <typename B, typename R>
Monitor_reduction<B,R>
::~Monitor_reduction()
{
	if(this->error_tracker_enable)
		flush_erroneous_frame();
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
void Monitor_reduction<B,R>
::flush_erroneous_frame()
{
	std::string file_name_head  = error_tracker_head_file_name + std::string("_") + std::to_string(snr);
	std::string file_name_src   = file_name_head + std::string(".src");
	std::string file_name_enc   = file_name_head + std::string(".enc");
	std::string file_name_noise = file_name_head + std::string(".cha");

	std::ofstream file_src  (file_name_src  );
	std::ofstream file_enc  (file_name_enc  );
	std::ofstream file_noise(file_name_noise);

	int n_fe = get_n_fe();

	// ************* write headers *****************
	file_src << n_fe          << std::endl << std::endl; // write number frames
	file_src << this->get_K() << std::endl << std::endl; // write length of frames

	file_enc << n_fe          << std::endl << std::endl; // write number frames
	file_enc << this->get_K() << std::endl << std::endl; // write length of non coded frames
	file_enc << this->get_N() << std::endl << std::endl; // write length of coded frames

	file_noise << n_fe               << std::endl << std::endl; // write number frames
	file_noise << this->get_Y_size() << std::endl << std::endl; // write length of frames

	// ************* write frames ********************
	Monitor<B,R>* mon = nullptr;
	for (unsigned i = 0; i <= monitors.size(); i++)
	{
		mon = (i == monitors.size()) ? this : monitors[i];

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

		// noise encoder
		auto buff_noise = mon->get_buff_noise();
		assert(buff_src.size() == buff_noise.size());
		for (unsigned f = 0; f < buff_noise.size(); f++)
		{
			for (unsigned b = 0; b < buff_noise[f].size(); b++)
				file_noise << buff_noise[f][b] << " ";

			file_noise << std::endl << std::endl;
		}
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
