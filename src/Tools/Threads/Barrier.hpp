/*!
 * \file
 * \brief Waits until all threads reach the barrier.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef BARRIER_HPP
#define BARRIER_HPP

#include <mutex>
#include <atomic>
#include <condition_variable>

/*!
 * \class Barrier
 *
 * \brief Waits until all threads reach the barrier.
 */
class Barrier
{
private:
	std::mutex mutex_barrier;
	std::condition_variable cond_barrier;
	const int n_threads;
	int counter_barrier;
	int generation;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param n_threads: number of threads to wait in the barrier.
	 */
	Barrier(const int n_threads);

	/*!
	 * \brief Destructor.
	 */
	~Barrier();

	/*!
	 * \brief Blocking method, wait until "n_threads" threads call this method.
	 *
	 * \param tid: the number id of the thread which call this method.
	 */
	void operator()(const int tid = -1);
};

#endif /* BARRIER_HPP */ 
