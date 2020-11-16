#include "Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace module
{
int Decoder
::compute_status(const std::vector<int>& status, const int frame_id)
{
	if (frame_id == -1)
	{
		int ret_status = 0;
		for (size_t w = 0; w < this->get_n_waves(); w++)
		{
			ret_status <<= this->get_n_frames_per_wave();
			ret_status |= status[w];
		}
		return ret_status & this->mask;
	}
	else
	{
		const auto w = (frame_id % this->get_n_frames()) / this->get_n_frames_per_wave();
		return status[w] & this->mask;
	}
}
}
}