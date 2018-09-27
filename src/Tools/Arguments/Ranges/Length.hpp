#ifndef ARGUMENT_RANGE_LENGTH_HPP_
#define ARGUMENT_RANGE_LENGTH_HPP_

#include <string>
#include <sstream>
#include <stdexcept>

#include "Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

class Length_range : public Argument_range
{
	const size_t min_size;
	const size_t max_size;

public:
	/*
	 * min_size is the minimal size must have the checked container (0 means no minimum)
	 * max_size is the maximal size must have the checked container (0 means infinite)
	 */
	Length_range(size_t min_size = 0, size_t max_size = 0)
	: Argument_range("limited length [" + std::to_string(min_size) + ";" + (max_size?std::to_string(max_size):"inf") + "]"),
	  min_size(min_size), max_size(max_size)
	{ }

	virtual ~Length_range() {};

	virtual Length_range* clone() const
	{
		return new Length_range(*this);
	}

	template <typename T>
	void check(const T& val) const
	{
		size_t container_size = val.size();

		if (min_size > container_size || (max_size < container_size && max_size != 0))
		{
			std::stringstream message;
			message << "shall have a length ";

			if (min_size == max_size)
				message << "of " << max_size;
			else if (min_size == 0)
				message << "of maximum " << max_size;
			else if (max_size != 0)
				message << "of minimum " << min_size << " and maximum " << max_size;
			else
				message << "of minimum " << min_size;

			message << " elements (given " << container_size << " elements)";

			throw std::runtime_error(message.str());
		}
	}
};

Length_range* Length(size_t min_size = 0, size_t max_size = 0);

}
}

#endif /* ARGUMENT_RANGE_LENGTH_HPP_ */