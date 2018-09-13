#include <string>
#include <chrono>
#include <iostream>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Barrier.hpp"

using namespace aff3ct::tools;

Barrier
::Barrier(const int n_threads)
: n_threads(n_threads), counter_barrier(n_threads), generation(0)
{
	if (n_threads <= 0)
	{
		std::stringstream message;
		message << "'n_threads' has to be greater than 0 ('n_threads' = " << n_threads << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

void Barrier
::operator()(const int tid)
{

	std::unique_lock<std::mutex> lock(mutex_barrier); // take the lock
	int cur_gen = generation;

	if (--counter_barrier == 0)
	{
		counter_barrier = n_threads;
		generation++;
		cond_barrier.notify_all();
	}
	else
	{
		while (cur_gen == generation)
		{
			// release the lock
			if (cond_barrier.wait_for(lock, std::chrono::milliseconds(6000)) == std::cv_status::timeout)
			{
				std::stringstream message;
				message << "aff3ct::tools::Barrier: some threads did not reach the barrier (counter_barrier = "
				        << counter_barrier << ", tid = " << tid << ", generation = " << generation
				        << "). " << "This message should never happen: there is a bug in the multithreaded code.";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		}
	}

	// release the lock
}
