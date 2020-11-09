#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename R>
Post_processing_SISO<B,R>
::Post_processing_SISO()
: n_frames(1)
{
}

template <typename B, typename R>
Post_processing_SISO<B,R>* Post_processing_SISO<B,R>
::clone() const
{
	throw unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
void Post_processing_SISO<B,R>
::deep_copy(const Post_processing_SISO<B,R>& t)
{
}

template <typename B, typename R>
bool Post_processing_SISO<B,R>
::siso_n(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext,
               mipp::vector<B>& s)
{
	return false;
}

template <typename B, typename R>
bool Post_processing_SISO<B,R>
::siso_i(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext)
{
	return false;
}

template <typename B, typename R>
void Post_processing_SISO<B,R>
::end(const int n_ite)
{
}

template <typename B, typename R>
size_t Post_processing_SISO<B,R>
::get_n_frames() const
{
	return this->n_frames;
}

template <typename B, typename R>
void Post_processing_SISO<B,R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		if (n_frames <= 0)
		{
			std::stringstream message;
			message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
		this->n_frames = n_frames;
	}
}

}
}
