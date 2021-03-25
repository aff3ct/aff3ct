#include <chrono>
#include <thread>
#include <string>
#include <sstream>

#include "Module/Initializer/Initializer.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename T>
Initializer<T>
::Initializer(const size_t n_elmts)
: Module(), init_data(this->get_n_frames(), std::vector<T>(n_elmts, 0))
{
	const std::string name = "Initializer";
	this->set_name(name);
	this->set_short_name(name);

	if (n_elmts == 0)
	{
		std::stringstream message;
		message << "'n_elmts' has to be greater than 0.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("initialize");
	auto ps_out = this->template create_socket_out<T>(p, "out", n_elmts);
	this->create_codelet(p, [ps_out](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &ini = static_cast<Initializer&>(m);
		ini._initialize(static_cast<T*>(t[ps_out].get_dataptr()), frame_id);
		return status_t::SUCCESS;
	});
}

template <typename T>
Initializer<T>* Initializer<T>
::clone() const
{
	auto m = new Initializer(*this);
	m->deep_copy(*this);
	return m;
}

template <typename T>
const std::vector<std::vector<T>>& Initializer<T>
::get_init_data() const
{
	return this->init_data;
}

template <typename T>
void Initializer<T>
::set_init_data(const std::vector<T>& init_data)
{
	if (init_data.size() != this->init_data[0].size())
	{
		std::stringstream message;
		message << "'init_data.size()' has to be equal to 'this->init_data[0].size()' ('init_data.size()' = "
		        << init_data.size() << ", 'this->init_data[0].size()' = " << this->init_data[0].size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto &ini : this->init_data)
		ini = init_data;
}

template <typename T>
void Initializer<T>
::set_init_data(const std::vector<std::vector<T>>& init_data)
{
	if (init_data.size() != this->get_n_frames())
	{
		std::stringstream message;
		message << "'init_data.size()' has to be equal to 'this->get_n_frames()' ('init_data.size()' = "
		        << init_data.size() << ", 'this->get_n_frames()' = " << this->get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (size_t f = 0; f< init_data.size(); f++)
	{
		if (init_data[f].size() != this->init_data[f].size())
		{
			std::stringstream message;
			message << "'init_data[f].size()' has to be equal to 'this->init_data[f].size()' ('init_data[f].size()' = "
			        << init_data[f].size() << ", 'this->init_data[f].size()' = " << this->init_data[f].size()
			        << ", 'f' = " << f << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
		this->init_data[f] = init_data[f];
	}
}

template <typename T>
void Initializer<T>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Module::set_n_frames(n_frames);

		this->init_data.resize(n_frames);
		for (size_t f = old_n_frames; f < n_frames; f++)
			this->init_data[f] = this->init_data[0];
	}
}

template <typename T>
void Initializer<T>
::initialize(T *out, const int frame_id, const bool managed_memory)
{
	(*this)[ini::sck::initialize::out].bind(out);
	(*this)[ini::tsk::initialize].exec(frame_id, managed_memory);
}

template <typename T>
void Initializer<T>
::_initialize(T *out, const size_t frame_id)
{
	std::copy(this->init_data[frame_id].begin(),
	          this->init_data[frame_id].end(),
	          out);
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Initializer<int8_t>;
template class aff3ct::module::Initializer<int16_t>;
template class aff3ct::module::Initializer<int32_t>;
template class aff3ct::module::Initializer<int64_t>;
template class aff3ct::module::Initializer<float>;
template class aff3ct::module::Initializer<double>;
// ==================================================================================== explicit template instantiation
