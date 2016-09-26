/*!
 * \file
 * \brief Generates source code.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

#include <vector>

/*!
 * \class Generator
 *
 * \brief Generates source code.
 */
class Generator
{
public:
	/*!
	 * \brief Constructor.
	 */
	Generator()
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Generator()
	{
	}

	/*!
	 * \brief Generates the source code.
	 */
	virtual void generate() = 0;
};

#endif /* GENERATOR_HPP_ */
