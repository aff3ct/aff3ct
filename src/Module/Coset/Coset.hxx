#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Coset/Coset.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename D>
Coset<B,D>
::Coset(const int size, const int n_frames)
: Module(n_frames), size(size)
{
	const std::string name = "Coset";
	this->set_name(name);
	this->set_short_name(name);

	if (size <= 0)
	{
		std::stringstream message;
		message << "'size' has to be greater than 0. ('size' = " << size << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("apply");
	auto &ps_ref = this->template create_socket_in <B>(p, "ref", this->size * this->n_frames);
	auto &ps_in  = this->template create_socket_in <D>(p, "in",  this->size * this->n_frames);
	auto &ps_out = this->template create_socket_out<D>(p, "out", this->size * this->n_frames);
	this->create_codelet(p, [this, &ps_ref, &ps_in, &ps_out]() -> int
	{
		this->apply(static_cast<B*>(ps_ref.get_dataptr()),
		            static_cast<D*>(ps_in .get_dataptr()),
		            static_cast<D*>(ps_out.get_dataptr()));

		return 0;
	});
}

template <typename B, typename D>
int Coset<B,D>
::get_size() const
{
	return this->size;
}

template <typename B, typename D>
template <class AB, class AD>
void Coset<B,D>
::apply(const std::vector<B,AB>& ref, const std::vector<D,AD> &in, std::vector<D,AD> &out,
           const int frame_id)
{
	if (ref.size() != in.size() || in.size() != out.size())
	{
		std::stringstream message;
		message << "'ref.size()' has to be equal to 'in_data.size()' and 'out_data.size()' ('ref.size()' = "
		        << ref.size() << ", 'in_data.size()' = " << in.size() << ", 'out_data.size()' = "
		        << out.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->size * this->n_frames != (int)ref.size())
	{
		std::stringstream message;
		message << "'ref.size()' has to be equal to 'size' * 'n_frames' ('ref.size()' = " << ref.size()
		        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->size * this->n_frames != (int)in.size())
	{
		std::stringstream message;
		message << "'in_data.size()' has to be equal to 'size' * 'n_frames' ('in_data.size()' = " << in.size()
		        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->size * this->n_frames != (int)out.size())
	{
		std::stringstream message;
		message << "'out_data.size()' has to be equal to 'size' * 'n_frames' ('out_data.size()' = " << out.size()
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

	this->apply(ref.data(), in.data(), out.data(), frame_id);
}

template <typename B, typename D>
void Coset<B,D>
::apply(const B *ref, const D *in, D *out, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_apply(ref + f * this->size,
		             in  + f * this->size,
		             out + f * this->size,
		             f);
}

template <typename B, typename D>
void Coset<B,D>
::_apply(const B *ref, const D *in, D *out, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
