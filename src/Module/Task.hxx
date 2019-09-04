#include "Task.hpp"

namespace aff3ct
{
namespace module
{
Socket& Task
::operator[](const int id)
{
	return *this->sockets[id];
}

void Task
::update_timer(const int id, const std::chrono::nanoseconds &duration)
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
}
}