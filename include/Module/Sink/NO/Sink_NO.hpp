/*!
 * \file
 * \brief Class module::Sink_NO.
 */
#ifndef SINK_NO_HPP
#define SINK_NO_HPP

#include "Module/Sink/Sink.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Sink_NO
 *
 * \brief Send data to a binary file.
 *
 * \tparam B: type of the data to send or receive.
 *
 */
template <typename B = int>
class Sink_NO : public Sink<B>
{
public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K: Radio_frame length.
	 * \param n_frames: number of frames.
	 */
	Sink_NO(const int K, const int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	~Sink_NO() = default;

	virtual Sink_NO<B>* clone() const;

protected:
	virtual void _send(const B *V, const int frame_id);
};

}
}

#endif /* SINK_NO_HPP */
