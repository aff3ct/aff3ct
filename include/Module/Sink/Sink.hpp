/*!
 * \file
 * \brief Class module::Sink.
 */
#ifndef SINK_HPP
#define SINK_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace snk
	{
		enum class tsk : size_t { send, SIZE };

		namespace sck
		{
			enum class send : size_t { V, SIZE };
		}
	}

/*!
 * \class Sink
 *
 * \brief Send data into outer world.
 *
 * \tparam B: type of the data to send or receive.
 *
 */
template <typename B = int>
class Sink : public Module
{
public:
	inline Task&   operator[](const snk::tsk       t);
	inline Socket& operator[](const snk::sck::send s);

protected:
	const int N; /*!< Size of one frame (= number of samples in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N: Radio_frame length.
	 */
	Sink(const int N, const int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Sink() = default;

	/*!
	 * \brief Consume a frame.
	 *
	 * \param V : a vector of data to consume.
	 */
	template <class A = std::allocator<B>>
	void send(const std::vector<B,A>& V, const int frame_id = -1);

	virtual void send(const B *V, const int frame_id = -1);

protected:
	virtual void _send(const B *V, const int frame_id);
};

}
}

#include "Module/Sink/Sink.hxx"

#endif /* SINK_HPP */
