#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Monitor/BFER/Monitor_BFER.hpp"

namespace aff3ct
{
namespace module
{
template <typename B>
template <class A>
int Monitor_BFER<B>
::check_errors(const std::vector<B,A>& U, const std::vector<B,A>& Y, const int frame_id)
{
	if ((int)U.size() != this->K * this->n_frames)
	{
		std::stringstream message;
		message << "'U.size()' has to be equal to 'K' * 'n_frames' ('U.size()' = " << U.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)Y.size() != this->K * this->n_frames)
	{
		std::stringstream message;
		message << "'Y.size()' has to be equal to 'K' * 'n_frames' ('Y.size()' = " << Y.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->check_errors(U.data(), Y.data(), frame_id);
}
}
}
