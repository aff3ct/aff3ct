#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Source/Source.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
Source<B>
::Source(const int K, const int n_frames)
: Module(n_frames), K(K)
{
	const std::string name = "Source";
	this->set_name(name);
	this->set_short_name(name);

	if (K <= 0)
	{
		std::stringstream message;
		message << "'K' has to be greater than 0 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("generate");
	auto &ps_U_K = this->template create_socket_out<B>(p, "U_K", this->K * this->n_frames);
	this->create_codelet(p, [this, &ps_U_K]() -> int
	{
		this->generate(static_cast<B*>(ps_U_K.get_dataptr()));

		return 0;
	});
}

template <typename B>
int Source<B>
::get_K() const
{
	return K;
}

template <typename B>
template <class A>
void Source<B>
::generate(std::vector<B,A>& U_K, const int frame_id)
{
	if (this->K * this->n_frames != (int)U_K.size())
	{
		std::stringstream message;
		message << "'U_K.size()' has to be equal to 'K' * 'n_frames' ('U_K.size()' = " << U_K.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->generate(U_K.data(), frame_id);
}

template <typename B>
void Source<B>
::generate(B *U_K, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_generate(U_K + f * this->K, f);
}
template <typename B>
void Source<B>
::_generate(B *U_K, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
