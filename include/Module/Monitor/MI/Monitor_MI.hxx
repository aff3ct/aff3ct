#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Monitor/MI/Monitor_MI.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
template <class AB, class AR>
R Monitor_MI<B,R>
::get_mutual_info(const std::vector<B,AB>& X, const std::vector<R,AR>& Y, const int frame_id)
{
	if ((int)X.size() != this->N * this->n_frames)
	{
		std::stringstream message;
		message << "'X.size()' has to be equal to 'N' * 'n_frames' ('X.size()' = " << X.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)Y.size() != this->N * this->n_frames)
	{
		std::stringstream message;
		message << "'Y.size()' has to be equal to 'N' * 'n_frames' ('Y.size()' = " << Y.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->get_mutual_info(X.data(), Y.data(), frame_id);
}
}
}
