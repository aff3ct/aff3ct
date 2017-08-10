#include "Process.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Process::Process(const std::string name, const bool autostart)
: name(name), autostart(autostart), codelet([]() { throw tools::unimplemented_error(__FILE__, __LINE__, __func__); })
{
}

std::string Process::get_name()
{
	return this->name;
}

bool Process::is_autostart()
{
	return this->autostart;
}

void Process::exec()
{
	if (can_exec())
		this->codelet();
	else
	{
		std::stringstream message;
		message << "The process cannot be executed because some of the inputs are not fed ('name' = "
		        << this->name << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
void Process::create_socket_in(const std::string name, const size_t n_elmts)
{
	in.push_back(Socket(*this, name, typeid(T), n_elmts * sizeof(T)));
}

template <typename T>
void Process::create_socket_out(const std::string name, const size_t n_elmts)
{
	out.push_back(Socket(*this, name, typeid(T), n_elmts * sizeof(T)));

	// memory allocation
	out_buffers.push_back(std::vector<uint8_t>(out.back().databytes));
	out.back().dataptr = out_buffers.back().data();
}

void Process::create_codelet(std::function<void(void)> codelet)
{
	this->codelet = codelet;
}

Socket& Process::operator[](const std::string name)
{
	for (auto &s : in ) if (s.get_name() == name) return s;
	for (auto &s : out) if (s.get_name() == name) return s;

	std::stringstream message;
	message << "Socket 'name' does not exist ('name' = " << name << ", 'process.name' = " << this->name << ").";
	throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
}

bool Process::last_input_socket(Socket &s_in)
{
	return &s_in == &in.back();
}

bool Process::can_exec()
{
	for (auto &s : this->in)
		if (s.dataptr == nullptr)
			return false;
	return true;
}

// ==================================================================================== explicit template instantiation
template void Process::create_socket_in<int8_t >(const std::string, const size_t);
template void Process::create_socket_in<int16_t>(const std::string, const size_t);
template void Process::create_socket_in<int32_t>(const std::string, const size_t);
template void Process::create_socket_in<int64_t>(const std::string, const size_t);
template void Process::create_socket_in<float  >(const std::string, const size_t);
template void Process::create_socket_in<double >(const std::string, const size_t);

template void Process::create_socket_out<int8_t >(const std::string, const size_t);
template void Process::create_socket_out<int16_t>(const std::string, const size_t);
template void Process::create_socket_out<int32_t>(const std::string, const size_t);
template void Process::create_socket_out<int64_t>(const std::string, const size_t);
template void Process::create_socket_out<float  >(const std::string, const size_t);
template void Process::create_socket_out<double >(const std::string, const size_t);
// ==================================================================================== explicit template instantiation
