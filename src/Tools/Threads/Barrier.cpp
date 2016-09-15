#include <chrono>
#include <cassert>
#include <iostream>

#include "Tools/Display/bash_tools.h"

#include "Barrier.hpp"

Barrier
::Barrier(const int n_threads)
: n_threads(n_threads), counter_barrier(n_threads), generation(0)
{
	assert(n_threads > 0);
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
			if (cond_barrier.wait_for(lock, std::chrono::milliseconds(3000)) == std::cv_status::timeout)
			{
				std::cerr << bold_red("(EE) Some threads did not reach the barrier (")
				          << bold_red("counter_barrier = ") << bold_red(std::to_string(counter_barrier)) << bold_red(", ")
				          << bold_red("tid = ")             << bold_red(std::to_string(tid))             << bold_red(", ")
				          << bold_red("generation = ")      << bold_red(std::to_string(generation))      << bold_red("). ")
				          << bold_red("This message should never happen: there is a bug in the multithreaded code.")
				          << std::endl;
			}
		}
	}

	// release the lock
}
