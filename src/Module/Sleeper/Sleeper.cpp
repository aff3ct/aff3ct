#include <chrono>
#include <thread>
#include <string>

#include "Module/Sleeper/Sleeper.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Sleeper
::Sleeper(const size_t ns)
: Module(), ns(ns)
{
	const std::string name = "Sleeper";
	this->set_name(name);
	this->set_short_name(name);

	auto &p = this->create_task("sleep");
	this->create_codelet(p, [](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &slp = static_cast<Sleeper&>(m);
		slp._sleep(frame_id);
		return status_t::SUCCESS;
	});
}

Sleeper* Sleeper
::clone() const
{
	auto m = new Sleeper(*this);
	m->deep_copy(*this);
	return m;
}

size_t Sleeper
::get_ns() const
{
	return this->ns;
}

void Sleeper
::set_ns(const size_t ns)
{
	this->ns = ns;
}

void Sleeper
::sleep(const int frame_id, const bool managed_memory)
{
	(*this)[slp::tsk::sleep].exec(frame_id, managed_memory);
}

void Sleeper
::_sleep(const size_t frame_id)
{
	auto t_start = std::chrono::steady_clock::now();
	std::chrono::nanoseconds duration;
	do
		duration = std::chrono::steady_clock::now() - t_start;
	while ((size_t)duration.count() < this->ns);
}
