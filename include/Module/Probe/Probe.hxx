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
	AProbe::set_n_frames(n_frames);

	if (size <= 0)
	{
		std::stringstream message;
		message << "'size' has to be greater than 0 ('size' = " << size << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("probe");
	auto p1s_in = this->template create_socket_in<T>(p1, "in", this->size);
	this->create_codelet(p1, [p1s_in](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &prb = static_cast<Probe<T>&>(m);

		prb._probe(static_cast<const T*>(t[p1s_in].get_dataptr()),
		           frame_id);

		return status_t::SUCCESS;
	});
}

template <typename T>
void Probe<T>
::set_n_frames(const size_t n_frames)
{
	const size_t old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		std::stringstream message;
		message << "This method is disabled in the 'Probe' class.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
template <class AT>
void Probe<T>
::probe(const std::vector<T,AT>& in, const int frame_id, const bool managed_memory)
{
	(*this)[prb::sck::probe::in].bind(in);
	(*this)[prb::tsk::probe].exec(frame_id, managed_memory);
}

template <typename T>
void Probe<T>
::probe(const T *in, const int frame_id, const bool managed_memory)
{
	(*this)[prb::sck::probe::in].bind(in);
	(*this)[prb::tsk::probe].exec(frame_id, managed_memory);
}

template <typename T>
void Probe<T>
::_probe(const T *in, const size_t frame_id)
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
