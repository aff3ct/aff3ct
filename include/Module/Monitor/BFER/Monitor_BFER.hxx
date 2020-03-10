#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"
#include "Module/Monitor/BFER/Monitor_BFER.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
Task& Monitor_BFER<B>
::operator[](const mnt::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B>
Socket& Monitor_BFER<B>
::operator[](const mnt::sck::check_errors s)
{
	return Module::operator[]((size_t)mnt::tsk::check_errors)[(size_t)s];
}

template <typename B>
Socket& Monitor_BFER<B>
::operator[](const mnt::sck::check_errors2 s)
{
	return Module::operator[]((size_t)mnt::tsk::check_errors2)[(size_t)s];
}

template <typename B>
template <class A>
int Monitor_BFER<B>
::check_errors(const std::vector<B,A>& U,
               const std::vector<B,A>& V,
               const int frame_id)
{
	if ((int)U.size() != this->K * this->n_frames)
	{
		std::stringstream message;
		message << "'U.size()' has to be equal to 'K' * 'n_frames' ('U.size()' = " << U.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)V.size() != this->K * this->n_frames)
	{
		std::stringstream message;
		message << "'V.size()' has to be equal to 'K' * 'n_frames' ('V.size()' = " << V.size()
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

	return this->check_errors(U.data(), V.data(), frame_id);
}

template <typename B>
template <class A>
int Monitor_BFER<B>
::check_errors2(const std::vector<B,A    >& U,
                const std::vector<B,A    >& V,
                      std::vector<int64_t>& FRA,
                      std::vector<int64_t>& BE,
                      std::vector<int64_t>& FE,
                      std::vector<float  >& BER,
                      std::vector<float  >& FER,
                const int frame_id)
{
	if ((int)U.size() != this->K * this->n_frames)
	{
		std::stringstream message;
		message << "'U.size()' has to be equal to 'K' * 'n_frames' ('U.size()' = " << U.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)V.size() != this->K * this->n_frames)
	{
		std::stringstream message;
		message << "'V.size()' has to be equal to 'K' * 'n_frames' ('V.size()' = " << V.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)FRA.size() != this->n_frames)
	{
		std::stringstream message;
		message << "'FRA.size()' has to be equal to 'n_frames' ('FRA.size()' = " << FRA.size()
		        << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)BE.size() != this->n_frames)
	{
		std::stringstream message;
		message << "'BE.size()' has to be equal to 'n_frames' ('BE.size()' = " << BE.size()
		        << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)FE.size() != this->n_frames)
	{
		std::stringstream message;
		message << "'FE.size()' has to be equal to 'n_frames' ('FE.size()' = " << FE.size()
		        << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)BER.size() != this->n_frames)
	{
		std::stringstream message;
		message << "'BER.size()' has to be equal to 'n_frames' ('BER.size()' = " << BER.size()
		        << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)FER.size() != this->n_frames)
	{
		std::stringstream message;
		message << "'FER.size()' has to be equal to 'n_frames' ('FER.size()' = " << FER.size()
		        << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->check_errors2(U.data(), V.data(), FRA.data(), BE.data(), FE.data(), BER.data(), FER.data(), frame_id);
}
}
}
