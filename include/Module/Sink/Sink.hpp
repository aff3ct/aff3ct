/*!
 * \file
 * \brief Class module::Sink.
 */
#ifndef SINK_HPP
#define SINK_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include "Tools/Interface/Interface_reset.hpp"
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
			enum class send : size_t { V, status };
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
class Sink : public Module, public tools::Interface_reset
{
public:
	inline Task&   operator[](const snk::tsk       t);
	inline Socket& operator[](const snk::sck::send s);

protected:
	const int K; /*!< Size of one frame (= number of samples in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K: Radio_frame length.
	 */
	Sink(const int K);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Sink() = default;

	virtual Sink<B>* clone() const;

	/*!
	 * \brief Consume a frame.
	 *
	 * \param V : a vector of data to consume.
	 */
	template <class A = std::allocator<B>>
	void send(const std::vector<B,A>& V, const int frame_id = -1, const bool managed_memory = true);

	void send(const B *V, const int frame_id = -1, const bool managed_memory = true);

	virtual void reset();

protected:
	virtual void _send(const B *V, const size_t frame_id);
};

}
}

#include "Module/Sink/Sink.hxx"

#endif /* SINK_HPP */
