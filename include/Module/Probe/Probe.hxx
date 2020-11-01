#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Probe/Probe.hpp"

namespace aff3ct
{
namespace module
{

template <typename T>
Task& Probe<T>
::operator[](const prb::tsk t)
{
	return Module::operator[]((int)t);
}

template <typename T>
Socket& Probe<T>
::operator[](const prb::sck::probe s)
{
	return Module::operator[]((int)prb::tsk::probe)[(int)s];
}

template <typename T>
Probe<T>
::Probe(const int size, const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames)
: AProbe(), size(size), col_name(col_name), reporter(reporter)
{
	const std::string name = "Probe<" + col_name + ">";
	this->set_name(name);
	this->set_short_name(name);

	if (size <= 0)
	{
		std::stringstream message;
		message << "'size' has to be greater than 0 ('size' = " << size << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("probe");
	auto p1s_in = this->template create_socket_in<T>(p1, "in", this->size);
	this->create_codelet(p1, [p1s_in](Module &m, Task &t) -> int
	{
		static_cast<Probe<T>&>(m).probe(static_cast<const T*>(t[p1s_in].get_dataptr()));
		return 0;
	});

	AProbe::set_n_frames(n_frames);
}

template <typename T>
void Probe<T>
::set_n_frames(const int n_frames)
{
	std::stringstream message;
	message << "This method is disabled in the 'Probe' class.";
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
}

template <typename T>
template <class AT>
void Probe<T>
::probe(const std::vector<T,AT>& in, const int frame_id)
{
	if (this->size * this->n_frames != (int)in.size())
	{
		std::stringstream message;
		message << "'in.size()' has to be equal to 'size' * 'n_frames' ('in.size()' = " << in.size()
		        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->probe(in.data(), frame_id);
}

template <typename T>
void Probe<T>
::probe(const T *in, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_probe(in + f * this->size, f);
}

template <typename T>
void Probe<T>
::_probe(const T *in, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename T>
void Probe<T>
::reset()
{
}

}
}
