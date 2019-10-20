#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
template <class AB, class AR>
void Monitor_EXIT<B,R>
::check_mutual_info(const std::vector<B,AB>& bits,
                    const std::vector<R,AR>& llrs_a,
                    const std::vector<R,AR>& llrs_e,
                    const int frame_id)
{
	if ((int)bits.size() != this->size * this->n_frames)
	{
		std::stringstream message;
		message << "'bits.size()' has to be equal to 'size' * 'n_frames' ('bits.size()' = " << bits.size()
		        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)llrs_a.size() != this->size * this->n_frames)
	{
		std::stringstream message;
		message << "'llrs_a.size()' has to be equal to 'size' * 'n_frames' ('llrs_a.size()' = " << llrs_a.size()
		        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)llrs_e.size() != this->size * this->n_frames)
	{
		std::stringstream message;
		message << "'llrs_e.size()' has to be equal to 'size' * 'n_frames' ('llrs_e.size()' = " << llrs_e.size()
		        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->check_mutual_info(bits.data(), llrs_a.data(), llrs_e.data(), frame_id);
}
}
}
