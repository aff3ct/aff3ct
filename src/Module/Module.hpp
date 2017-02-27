/*!
 * \file
 * \brief A Module is an abstract concept. Basically, all the objects used in a Simulation are a Module.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MODULE_HPP_
#define MODULE_HPP_

#include <string>
#include <cassert>

namespace aff3ct
{
namespace module
{
/*!
 * \class Module
 *
 * \brief A Module is an abstract concept. Basically, all the objects used in a Simulation are a Module.
 */
class Module
{
protected:
	int         n_frames; /*!< Number of frames to process in this Module */
	std::string name;     /*!< Name of the Module. */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param n_frames: number of frames to process in this Module.
	 * \param name    : Module's name.
	 */
	Module(const int n_frames = 1, const std::string name = "Module") : n_frames(n_frames), name(name)
	{ assert(n_frames > 0); }

	/*!
	 * \brief Destructor.
	 */
	virtual ~Module() {}

	/*!
	 * \brief Set the number of frames.
	 *
	 * \param n_frames: number of frames to process in this Module.
	 */
	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}

	/*!
	 * \brief Get the number of frames.
	 *
	 * \return the number of frames to process in this Module.
	 */
	virtual int get_n_frames() const
	{
		return n_frames;
	}

	/*!
	 * \brief Rename the Module.
	 *
	 * \param name: the new Module name.
	 */
	void rename(const std::string name)
	{
		this->name = name;
	}
};
}
}

#endif /* MODULE_HPP_ */
