#include <iostream>
#include <iomanip>
#include <ios>

#include <rang.hpp>

#include "Module.hpp"
#include "Socket.hpp"
#include "Task.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Task::Task(const Module &module, const std::string &name, const bool autoalloc, const bool autoexec,
           const bool stats, const bool fast, const bool debug)
: module(module),
  name(name),
  autoalloc(autoalloc),
  autoexec(autoexec),
  stats(stats),
  fast(fast),
  debug(debug),
  debug_hex(false),
  debug_limit(-1),
  debug_precision(2),
  debug_frame_max(-1),
  codelet([]() -> int { throw tools::unimplemented_error(__FILE__, __LINE__, __func__); return 0; }),
  n_calls(0),
  duration_total(std::chrono::nanoseconds(0)),
  duration_min(std::chrono::nanoseconds(0)),
  duration_max(std::chrono::nanoseconds(0)),
  last_input_socket(nullptr)
{
}

void Task::set_autoalloc(const bool autoalloc)
{
	if (autoalloc != this->autoalloc)
	{
		this->autoalloc = autoalloc;

		if (!autoalloc)
		{
			this->out_buffers.clear();
			for (auto& s : sockets)
				if (get_socket_type(*s) == socket_t::SOUT)
					s->dataptr = nullptr;
		}
		else
		{
			for (auto& s : sockets)
				if (get_socket_type(*s) == socket_t::SOUT)
				{
					out_buffers.push_back(mipp::vector<uint8_t>(s->databytes));
					s->dataptr = out_buffers.back().data();
				}
		}
	}
}

void Task::set_autoexec(const bool autoexec)
{
	this->autoexec = autoexec;
}

void Task::set_stats(const bool stats)
{
	this->stats = stats;

	if (this->stats)
		this->set_fast(false);
}

void Task::set_fast(const bool fast)
{
	this->fast = fast;
	if (this->fast)
	{
		this->set_debug(false);
		this->set_stats(false);
	}

	for (size_t i = 0; i < sockets.size(); i++)
		sockets[i]->set_fast(this->fast);
}

void Task::set_debug(const bool debug)
{
	this->debug = debug;

	if (this->debug)
		this->set_fast(false);
}

void Task::set_debug_hex(const bool debug_hex)
{
	this->debug_hex = debug_hex;
}

void Task::set_debug_limit(const uint32_t limit)
{
	this->debug_limit = (int32_t)limit;
}

void Task::set_debug_precision(const uint8_t prec)
{
	this->debug_precision = prec;
}

void Task::set_debug_frame_max(const uint32_t limit)
{
	this->debug_frame_max = limit;
}

// trick to compile on the GNU compiler version 4 (where 'std::hexfloat' is unavailable)
#if !defined(__clang__) && !defined(__llvm__) && defined(__GNUC__) && defined(__cplusplus) && __GNUC__ < 5
namespace std {
class Hexfloat {
public:
	void message(std::ostream &os) const { os << " /!\\ 'std::hexfloat' is not supported by this compiler. /!\\ "; }
};
Hexfloat hexfloat;
}
std::ostream& operator<<(std::ostream &os, const std::Hexfloat &obj) { obj.message(os); return os; }
#endif

template <typename T>
static inline void display_data(const T *data,
                                const size_t fra_size, const size_t n_fra, const size_t limit, const size_t max_frame,
                                const uint8_t p, const uint8_t n_spaces, const bool hex)
{
	constexpr bool is_float_type = std::is_same<float, T>::value || std::is_same<double, T>::value;

	std::ios::fmtflags f(std::cout.flags());
	if (hex)
	{
		if (is_float_type) std::cout << std::hexfloat << std::hex;
		else               std::cout << std::hex;
	}
	else
		std::cout << std::fixed << std::setprecision(p) << std::dec;

	if (n_fra == 1 && max_frame != 0)
	{
		for (size_t i = 0; i < limit; i++)
		{
			if (hex)
				std::cout << (!is_float_type ? "0x" : "") << +data[i] << (i < limit -1 ? ", " : "");
			else
				std::cout << std::setw(p +3) << +data[i] << (i < limit -1 ? ", " : "");
		}
		std::cout << (limit < fra_size ? ", ..." : "");
	}
	else
	{
		std::string spaces = "#";
		for (uint8_t s = 0; s < n_spaces -1; s++)
			spaces += " ";

		auto n_digits_dec = [](size_t f) -> size_t
		{
			size_t count = 0;
			while (f && ++count)
				f /= 10;
			return count;
		};

		const auto n_digits = n_digits_dec(max_frame);
		auto ftos = [&n_digits_dec,&n_digits](size_t f) -> std::string
		{
			auto n_zero = n_digits - n_digits_dec(f);
			std::string f_str = "";
			for (size_t z = 0; z < n_zero; z++)
				f_str += "0";
			f_str += std::to_string(f);
			return f_str;
		};

		for (size_t f = 0; f < max_frame; f++)
		{
			std::cout << (f >= 1 ? spaces : "") << rang::style::bold << rang::fg::gray << "f" << ftos(f+1)
			          << rang::style::reset << "(";

			for (size_t i = 0; i < limit; i++)
			{
				if (hex)
					std::cout << (!is_float_type ? "0x" : "") << +data[f * fra_size +i]
					          << (i < limit -1 ? ", " : "");
				else
					std::cout << std::setw(p +3) << +data[f * fra_size +i] << (i < limit -1 ? ", " : "");
			}
			std::cout << (limit < fra_size ? ", ..." : "") << ")" << (f < n_fra -1 ? ", \n" : "");
		}

		if (max_frame < n_fra)
			std::cout << (max_frame >= 1 ? spaces : "") << rang::style::bold << rang::fg::gray << "f"
			          << std::setw(n_digits) << max_frame + 1 << "->" << "f" << n_fra << ":"
			          << rang::style::reset << "(...)";
	}

	std::cout.flags(f);
}

int Task::exec()
{
	if (fast)
	{
		auto exec_status = this->codelet();
		this->n_calls++;
		return exec_status;
	}

	if (can_exec())
	{
		size_t max_n_chars = 0;
		if (debug)
		{
			auto n_fra = (size_t)this->module.get_n_frames();

			std::cout << "# ";
			std::cout << rang::style::bold << rang::fg::green << module.get_name() << rang::style::reset
			          << "::"
			          << rang::style::bold << rang::fg::magenta << get_name() << rang::style::reset
			          << "(";
			for (auto i = 0; i < (int)sockets.size(); i++)
			{
				auto &s = *sockets[i];
				auto s_type = get_socket_type(s);
				auto n_elmts = s.get_databytes() / (size_t)s.get_datatype_size();
				std::cout << rang::style::bold << rang::fg::blue << (s_type == socket_t::SIN ? "const " : "")
				          << s.get_datatype_string() << rang::style::reset
				          << " " << s.get_name() << "[" << (n_fra > 1 ? std::to_string(n_fra) + "x" : "")
				          << (n_elmts / n_fra) << "]"
				          << (i < (int)sockets.size() -1 ? ", " : "");

				max_n_chars = std::max(s.get_name().size(), max_n_chars);
			}
			std::cout << ")" << std::endl;

			for (auto& s : sockets)
			{
				auto s_type = get_socket_type(*s);
				if (s_type == socket_t::SIN || s_type == socket_t::SIN_SOUT)
				{
					std::string spaces; for (size_t ss = 0; ss < max_n_chars - s->get_name().size(); ss++) spaces += " ";

					auto n_elmts = s->get_databytes() / (size_t)s->get_datatype_size();
					auto fra_size = n_elmts / n_fra;
					auto limit     = debug_limit != -1 ? std::min(fra_size, (size_t)debug_limit) : fra_size;
					auto max_frame = debug_frame_max != -1 ? std::min(n_fra, (size_t)debug_frame_max) : n_fra;
					auto p = debug_precision;
					auto h = debug_hex;
					std::cout << "# {IN}  " << s->get_name() << spaces << " = [";
					     if (s->get_datatype() == typeid(int8_t )) display_data((int8_t *)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					else if (s->get_datatype() == typeid(int16_t)) display_data((int16_t*)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					else if (s->get_datatype() == typeid(int32_t)) display_data((int32_t*)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					else if (s->get_datatype() == typeid(int64_t)) display_data((int64_t*)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					else if (s->get_datatype() == typeid(float  )) display_data((float  *)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					else if (s->get_datatype() == typeid(double )) display_data((double *)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					std::cout << "]" << std::endl;
				}
			}
		}

		int exec_status;
		if (stats)
		{
			auto t_start = std::chrono::steady_clock::now();
			exec_status = this->codelet();
			auto duration = std::chrono::steady_clock::now() - t_start;

			this->duration_total += duration;
			if (n_calls)
			{
				this->duration_min = std::min(this->duration_min, duration);
				this->duration_max = std::max(this->duration_max, duration);
			}
			else
			{
				this->duration_min = duration;
				this->duration_max = duration;
			}
		}
		else
			exec_status = this->codelet();
		this->n_calls++;

		if (debug)
		{
			auto n_fra = (size_t)this->module.get_n_frames();
			for (auto& s : sockets)
			{
				auto s_type = get_socket_type(*s);
				if (s_type == socket_t::SOUT || s_type == socket_t::SIN_SOUT)
				{
					std::string spaces; for (size_t ss = 0; ss < max_n_chars - s->get_name().size(); ss++) spaces += " ";

					auto n_elmts = s->get_databytes() / (size_t)s->get_datatype_size();
					auto fra_size = n_elmts / n_fra;
					auto limit = debug_limit != -1 ? std::min(fra_size, (size_t)debug_limit) : fra_size;
					auto max_frame = debug_frame_max != -1 ? std::min(n_fra, (size_t)debug_frame_max) : n_fra;
					auto p = debug_precision;
					auto h = debug_hex;
					std::cout << "# {OUT} " << s->get_name() << spaces << " = [";
					     if (s->get_datatype() == typeid(int8_t )) display_data((int8_t *)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					else if (s->get_datatype() == typeid(int16_t)) display_data((int16_t*)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					else if (s->get_datatype() == typeid(int32_t)) display_data((int32_t*)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					else if (s->get_datatype() == typeid(int64_t)) display_data((int64_t*)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					else if (s->get_datatype() == typeid(float  )) display_data((float  *)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					else if (s->get_datatype() == typeid(double )) display_data((double *)s->get_dataptr(), fra_size, n_fra, limit, max_frame, p, (uint8_t)max_n_chars +12, h);
					std::cout << "]" << std::endl;
				}
			}
			std::cout << "# Returned status: " << exec_status << std::endl;
			std::cout << "#" << std::endl;
		}

		return exec_status;
	}
	else
	{
		std::stringstream message;
		message << "The task cannot be executed because some of the inputs/outputs are not fed ('task.name' = "
		        << this->get_name() << ", 'module.name' = " << module.get_name() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
Socket& Task::create_socket(const std::string &name, const size_t n_elmts)
{
	if (name.empty())
	{
		std::stringstream message;
		message << "Impossible to create this socket because the name is empty ('task.name' = " << this->get_name()
		        << ", 'module.name' = " << module.get_name() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto &s : sockets)
		if (s->get_name() == name)
		{
			std::stringstream message;
			message << "Impossible to create this socket because an other socket has the same name ('socket.name' = "
			        << name << ", 'task.name' = " << this->get_name()
			        << ", 'module.name' = " << module.get_name() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

	auto s = std::make_shared<Socket>(*this, name, typeid(T), n_elmts * sizeof(T), this->is_fast());

	sockets.push_back(std::move(s));

	return *sockets.back();
}

template <typename T>
Socket& Task::create_socket_in(const std::string &name, const size_t n_elmts)
{
	auto &s = create_socket<T>(name, n_elmts);

	socket_type.push_back(socket_t::SIN);
	last_input_socket = &s;

	return s;
}

template <typename T>
Socket& Task::create_socket_in_out(const std::string &name, const size_t n_elmts)
{
	auto &s = create_socket<T>(name, n_elmts);

	socket_type.push_back(socket_t::SIN_SOUT);
	last_input_socket = &s;

	return s;
}

template <typename T>
Socket& Task::create_socket_out(const std::string &name, const size_t n_elmts)
{
	auto &s = create_socket<T>(name, n_elmts);

	socket_type.push_back(socket_t::SOUT);

	// memory allocation
	if (is_autoalloc())
	{
		out_buffers.push_back(mipp::vector<uint8_t>(s.get_databytes()));
		s.dataptr = out_buffers.back().data();
	}

	return s;
}

void Task::create_codelet(std::function<int(void)> &codelet)
{
	this->codelet = codelet;
}

bool Task::can_exec() const
{
	for (size_t i = 0; i < sockets.size(); i++)
		if (sockets[i]->dataptr == nullptr)
			return false;
	return true;
}

std::chrono::nanoseconds Task::get_duration_total() const
{
	return this->duration_total;
}

std::chrono::nanoseconds Task::get_duration_avg() const
{
	return this->duration_total / this->n_calls;
}

std::chrono::nanoseconds Task::get_duration_min() const
{
	return this->duration_min;
}

std::chrono::nanoseconds Task::get_duration_max() const
{
	return this->duration_max;
}

const std::vector<std::string>& Task::get_timers_name() const
{
	return this->timers_name;
}

const std::vector<uint32_t>& Task::get_timers_n_calls() const
{
	return this->timers_n_calls;
}

const std::vector<std::chrono::nanoseconds>& Task::get_timers_total() const
{
	return this->timers_total;
}

const std::vector<std::chrono::nanoseconds>& Task::get_timers_min() const
{
	return this->timers_min;
}

const std::vector<std::chrono::nanoseconds>& Task::get_timers_max() const
{
	return this->timers_max;
}

socket_t Task::get_socket_type(const Socket &s) const
{
	for (size_t i = 0; i < sockets.size(); i++)
		if (sockets[i].get() == &s)
			return socket_type[i];

	std::stringstream message;
	message << "The socket does not exist ('s.name' = " << s.get_name() << ", 'task.name' = " << this->get_name()
	        << ", 'module.name' = " << module.get_name() << ").";
	throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
}

void Task::register_timer(const std::string &name)
{
	this->timers_name   .push_back(name                       );
	this->timers_n_calls.push_back(0                          );
	this->timers_total  .push_back(std::chrono::nanoseconds(0));
	this->timers_max    .push_back(std::chrono::nanoseconds(0));
	this->timers_min    .push_back(std::chrono::nanoseconds(0));
}

void Task::reset_stats()
{
	this->n_calls        =                          0;
	this->duration_total = std::chrono::nanoseconds(0);
	this->duration_min   = std::chrono::nanoseconds(0);
	this->duration_max   = std::chrono::nanoseconds(0);

	for (auto &x : this->timers_n_calls) x =                          0;
	for (auto &x : this->timers_total  ) x = std::chrono::nanoseconds(0);
	for (auto &x : this->timers_min    ) x = std::chrono::nanoseconds(0);
	for (auto &x : this->timers_max    ) x = std::chrono::nanoseconds(0);
}

// ==================================================================================== explicit template instantiation
template Socket& Task::create_socket_in<int8_t >(const std::string&, const size_t);
template Socket& Task::create_socket_in<int16_t>(const std::string&, const size_t);
template Socket& Task::create_socket_in<int32_t>(const std::string&, const size_t);
template Socket& Task::create_socket_in<int64_t>(const std::string&, const size_t);
template Socket& Task::create_socket_in<float  >(const std::string&, const size_t);
template Socket& Task::create_socket_in<double >(const std::string&, const size_t);

template Socket& Task::create_socket_in_out<int8_t >(const std::string&, const size_t);
template Socket& Task::create_socket_in_out<int16_t>(const std::string&, const size_t);
template Socket& Task::create_socket_in_out<int32_t>(const std::string&, const size_t);
template Socket& Task::create_socket_in_out<int64_t>(const std::string&, const size_t);
template Socket& Task::create_socket_in_out<float  >(const std::string&, const size_t);
template Socket& Task::create_socket_in_out<double >(const std::string&, const size_t);

template Socket& Task::create_socket_out<int8_t >(const std::string&, const size_t);
template Socket& Task::create_socket_out<int16_t>(const std::string&, const size_t);
template Socket& Task::create_socket_out<int32_t>(const std::string&, const size_t);
template Socket& Task::create_socket_out<int64_t>(const std::string&, const size_t);
template Socket& Task::create_socket_out<float  >(const std::string&, const size_t);
template Socket& Task::create_socket_out<double >(const std::string&, const size_t);
// ==================================================================================== explicit template instantiation