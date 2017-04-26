#include <string>
#include <chrono>
#include <stdexcept>
#include <iostream>

#include "Barrier.hpp"

using namespace aff3ct::tools;

Barrier
::Barrier(const int n_threads)
: n_threads(n_threads), counter_barrier(n_threads), generation(0)
{
	if (n_threads <= 0)
		throw std::invalid_argument("aff3ct::tools::Barrier: \"n_threads\" has to be greater than 0.");
}
	
Barrier
::~Barrier()
{
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
				throw std::invalid_argument("aff3ct::tools::Barrier: some threads did not reach the barrier "
				                            "(counter_barrier = " + std::to_string(counter_barrier) +
				                            ", tid = " + std::to_string(tid) +
				                            ", generation = " + std::to_string(generation) + ")." +
				                            "This message should never happen: there is a bug in the "
				                            "multithreaded code.");
			}
		}
	}

	// release the lock
}
