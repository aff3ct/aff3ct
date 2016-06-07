#ifndef BARRIER_HPP
#define BARRIER_HPP

#include <mutex>
#include <atomic>
#include <condition_variable>

/*#ifdef _WIN32
// see https://github.com/meganz/mingw-std-threads
#include <mingw.mutex.h>
#include <mingw.condition_variable.h>
#endif*/

class Barrier
{
private:
	std::mutex mutex_barrier;
	std::condition_variable cond_barrier;
	const int n_threads;
	int counter_barrier;
	int generation;

public:
	Barrier(const int n_threads);
	~Barrier();

	void operator()(const int tid = -1);
};

#endif /* BARRIER_HPP */ 