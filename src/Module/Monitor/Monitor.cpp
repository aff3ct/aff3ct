#include "Tools/Exception/exception.hpp"

#include "Monitor.hpp"

bool aff3ct::module::Monitor::interrupt       = false;
bool aff3ct::module::Monitor::first_interrupt = true;
bool aff3ct::module::Monitor::over            = false;

std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> aff3ct::module::Monitor::t_last_interrupt;
