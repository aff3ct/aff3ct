#include "Tools/Exception/exception.hpp"

#include "Monitor.hpp"

bool aff3ct::module::Monitor_i::interrupt       = false;
bool aff3ct::module::Monitor_i::first_interrupt = true;
bool aff3ct::module::Monitor_i::over            = false;

std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> aff3ct::module::Monitor_i::t_last_interrupt;
