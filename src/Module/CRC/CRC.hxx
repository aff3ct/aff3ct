#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/CRC/CRC.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
CRC<B>
::CRC(const int K, const int size, const int n_frames)
: Module(n_frames), K(K), size(size)
{
	const std::string name = "CRC";
	this->set_name(name);
	this->set_short_name(name);

	if (K <= 0)
	{
		std::stringstream message;
		message << "'K' has to be greater than 0 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("build");
	auto &p1s_U_K1 = this->template create_socket_in <B>(p1, "U_K1",  this->K               * this->n_frames);
	auto &p1s_U_K2 = this->template create_socket_out<B>(p1, "U_K2", (this->K + this->size) * this->n_frames);
	this->create_codelet(p1, [this, &p1s_U_K1, &p1s_U_K2]() -> int
	{
		this->build(static_cast<B*>(p1s_U_K1.get_dataptr()),
		            static_cast<B*>(p1s_U_K2.get_dataptr()));

		return 0;
	});

	auto &p2 = this->create_task("extract");
	auto &p2s_V_K1 = this->template create_socket_in <B>(p2, "V_K1", (this->K + this->size) * this->n_frames);
	auto &p2s_V_K2 = this->template create_socket_out<B>(p2, "V_K2",  this->K               * this->n_frames);
	this->create_codelet(p2, [this, &p2s_V_K1, &p2s_V_K2]() -> int
	{
		this->extract(static_cast<B*>(p2s_V_K1.get_dataptr()),
		              static_cast<B*>(p2s_V_K2.get_dataptr()));

		return 0;
	});

	auto &p3 = this->create_task("check");
	auto &p3s_V_K = this->template create_socket_in<B>(p3, "V_K", (this->K + this->size) * this->n_frames);
	this->create_codelet(p3, [this, &p3s_V_K]() -> int
	{
		return this->check(static_cast<B*>(p3s_V_K.get_dataptr())) ? 1 : 0;
	});
}

template <typename B>
int CRC<B>
::get_K() const
{
	return this->K;
}


template <typename B>
int CRC<B>
::get_size()
{
	return size;
}


template <typename B>
template <class A>
void CRC<B>
::build(const std::vector<B,A>& U_K1, std::vector<B,A>& U_K2, const int frame_id)
{
	if (this->K * this->n_frames != (int)U_K1.size())
	{
		std::stringstream message;
		message << "'U_K1.size()' has to be equal to 'K' * 'n_frames' ('U_K1.size()' = " << U_K1.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((this->K + this->get_size()) * this->n_frames != (int)U_K2.size())
	{
		std::stringstream message;
		message << "'U_K2.size()' has to be equal to ('K' + 'get_size()') * 'n_frames' ('U_K2.size()' = "
		        << U_K2.size() << ", 'K' = " << this->K << ", 'get_size()' = " << this->get_size()
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

	this->build(U_K1.data(), U_K2.data(), frame_id);
}

template <typename B>
void CRC<B>
::build(const B *U_K1, B *U_K2, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_build(U_K1 + f *  this->K,
		             U_K2 + f * (this->K + this->get_size()),
		             f);
}

template <typename B>
template <class A>
void CRC<B>
::extract(const std::vector<B,A>& V_K1, std::vector<B,A>& V_K2, const int frame_id)
{
	if ((this->K + this->get_size()) * this->n_frames != (int)V_K1.size())
	{
		std::stringstream message;
		message << "'V_K1.size()' has to be equal to ('K' + 'get_size()') * 'n_frames' ('V_K1.size()' = "
		        << V_K1.size() << ", 'K' = " << this->K << ", 'get_size()' = " << this->get_size()
		        << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->K * this->n_frames != (int)V_K2.size())
	{
		std::stringstream message;
		message << "'V_K2.size()' has to be equal to 'K' * 'n_frames' ('V_K2.size()' = " << V_K2.size()
		        << ", 'K' = " << this->K << ", 'n_frames' =  " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->extract(V_K1.data(), V_K2.data(), frame_id);
}

template <typename B>
void CRC<B>
::extract(const B *V_K1, B *V_K2, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_extract(V_K1 + f * (this->K + this->get_size()),
		               V_K2 + f *  this->K,
		               f);
}


template <typename B>
template <class A>
bool CRC<B>
::check(const std::vector<B,A>& V_K, const int n_frames, const int frame_id)
{
	if (n_frames <= 0 && n_frames != -1)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 or equal to -1 ('n_frames' = " << n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= (n_frames ? n_frames : this->n_frames))
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << (n_frames ? n_frames : this->n_frames) << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((this->K + (int)this->get_size()) *       n_frames != (int)V_K.size() &&
	    (this->K + (int)this->get_size()) * this->n_frames != (int)V_K.size())
	{
		std::stringstream message;
		message << "'V_K.size()' has to be equal to ('K' + 'size') * 'n_frames' ('V_K.size()' = " << V_K.size()
		        << ", 'K' = " << this->K
		        << ", 'n_frames' = " << (n_frames != -1 ? n_frames : this->n_frames) << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->check(V_K.data(), n_frames, frame_id);
}

template <typename B>
bool CRC<B>
::check(const B *V_K, const int n_frames, const int frame_id)
{
	const int real_n_frames = (n_frames != -1) ? n_frames : this->n_frames;

	const auto f_start = (frame_id < 0) ? 0 : frame_id % real_n_frames;
	const auto f_stop  = (frame_id < 0) ? real_n_frames : f_start +1;

	auto f = f_start;
	while (f < f_stop && this->_check(V_K + f * (this->K + this->get_size()), f))
		f++;

	return f == f_stop;
}

template <typename B>
template <class A>
bool CRC<B>
::check_packed(const std::vector<B,A>& V_K, const int n_frames, const int frame_id)
{
	if (n_frames <= 0 && n_frames != -1)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 or equal to -1 ('n_frames' = " << n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= (n_frames ? n_frames : this->n_frames))
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << (n_frames ? n_frames : this->n_frames) << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((this->K + (int)this->get_size()) *       n_frames != (int)V_K.size() &&
	    (this->K + (int)this->get_size()) * this->n_frames != (int)V_K.size())
	{
		std::stringstream message;
		message << "'V_K.size()' has to be equal to ('K' + 'size') * 'n_frames' ('V_K.size()' = " << V_K.size()
		        << ", 'K' = " << this->K
		        << ", 'n_frames' = " << (n_frames != -1 ? n_frames : this->n_frames) << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->check_packed(V_K.data(), n_frames, frame_id);
}

template <typename B>
bool CRC<B>
::check_packed(const B *V_K, const int n_frames, const int frame_id)
{
	const int real_n_frames = (n_frames != -1) ? n_frames : this->n_frames;

	const auto f_start = (frame_id < 0) ? 0 : frame_id % real_n_frames;
	const auto f_stop  = (frame_id < 0) ? real_n_frames : f_start +1;

	auto f = f_start;
	while (f < f_stop && this->_check_packed(V_K + f * (this->K + this->get_size()), f))
		f++;

	return f == f_stop;
}

template <typename B>
void CRC<B>
::_build(const B *U_K1, B *U_K2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
void CRC<B>
::_extract(const B *V_K1, B *V_K2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
bool CRC<B>
::_check(const B *V_K, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	return false;
}

template <typename B>
bool CRC<B>
::_check_packed(const B *V_K, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	return false;
}

}
}
