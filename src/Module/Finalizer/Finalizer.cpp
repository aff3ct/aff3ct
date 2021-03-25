#include <string>
#include <sstream>

#include "Module/Finalizer/Finalizer.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename T>
Finalizer<T>
::Finalizer(const size_t n_elmts)
: Module(), final_data(this->get_n_frames(), std::vector<T>(n_elmts, 0))
{
	const std::string name = "Finalizer";
	this->set_name(name);
	this->set_short_name(name);

	if (n_elmts == 0)
	{
		std::stringstream message;
		message << "'n_elmts' has to be greater than 0.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("finalize");
	auto ps_in = this->template create_socket_in<T>(p, "in", n_elmts);
	this->create_codelet(p, [ps_in](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &fin = static_cast<Finalizer&>(m);
		fin._finalize(static_cast<const T*>(t[ps_in].get_dataptr()), frame_id);
		return status_t::SUCCESS;
	});
}

template <typename T>
Finalizer<T>* Finalizer<T>
::clone() const
{
	auto m = new Finalizer(*this);
	m->deep_copy(*this);
	return m;
}

template <typename T>
const std::vector<std::vector<T>>& Finalizer<T>
::get_final_data() const
{
	return this->final_data;
}

template <typename T>
void Finalizer<T>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Module::set_n_frames(n_frames);
		this->final_data.resize(n_frames);
		for (size_t f = old_n_frames; f < n_frames; f++)
			this->final_data[f].resize(this->final_data[0].size());
	}
}

template <typename T>
void Finalizer<T>
::finalize(const T *in, const int frame_id, const bool managed_memory)
{
	(*this)[fin::sck::finalize::in].bind(in);
	(*this)[fin::tsk::finalize].exec(frame_id, managed_memory);
}

template <typename T>
void Finalizer<T>
::_finalize(const T *in, const size_t frame_id)
{
	std::copy(in,
	          in + this->final_data[0].size(),
	          this->final_data[frame_id].begin());
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Finalizer<int8_t>;
template class aff3ct::module::Finalizer<int16_t>;
template class aff3ct::module::Finalizer<int32_t>;
template class aff3ct::module::Finalizer<int64_t>;
template class aff3ct::module::Finalizer<float>;
template class aff3ct::module::Finalizer<double>;
// ==================================================================================== explicit template instantiation
