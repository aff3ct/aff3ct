/*!
 * \file
 * \brief Class tools::Interface_get_set_n_frames.
 */
#ifndef INTERFACE_GET_SET_N_FRAMES_HPP__
#define INTERFACE_GET_SET_N_FRAMES_HPP__

#include <cstddef> // size_t

namespace aff3ct
{
namespace tools
{
class Interface_get_set_n_frames
{
public:
	virtual size_t get_n_frames() const = 0;
	virtual void set_n_frames(const size_t n_frames) = 0;
};

}
}

#endif // INTERFACE_GET_SET_N_FRAMES_HPP__