#include <algorithm>
#include <sstream>
#include <cassert>

#include "Module/Task.hpp"

namespace aff3ct
{
namespace module
{
bool Task
::is_autoalloc() const
{
	return this->autoalloc;
}

bool Task
::is_stats() const
{
	return this->stats;
}

bool Task
::is_fast() const
{
	return this->fast;
}

bool Task
::is_debug() const
{
	return this->debug;
}

bool Task
::is_debug_hex() const
{
	return this->debug_hex;
}

bool Task
::is_last_input_socket(const Socket &s_in) const
{
	return last_input_socket == &s_in;
}

Module& Task
::get_module() const
{
	return *this->module;
}

std::string Task
::get_name() const
{
	return this->name;
}

uint32_t Task
::get_n_calls() const
{
	return this->n_calls;
}

Socket& Task
::operator[](const size_t id)
{
	assert((size_t)id < this->sockets.size());
	assert(this->sockets[id] != nullptr);

	return *this->sockets[id];
}

void Task
::update_timer(const size_t id, const std::chrono::nanoseconds &duration)
{
	if (this->is_stats())
	{
		this->timers_n_calls[id]++;
		this->timers_total[id] += duration;
		if (this->n_calls)
		{
			this->timers_max[id] = std::max(this->timers_max[id], duration);
			this->timers_min[id] = std::min(this->timers_min[id], duration);
		}
		else
		{
			this->timers_max[id] = duration;
			this->timers_min[id] = duration;
		}
	}
}

const std::vector<int>& Task
::get_status() const
{
	return this->status;
}
}
}
