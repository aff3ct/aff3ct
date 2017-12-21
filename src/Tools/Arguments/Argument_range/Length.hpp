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

template <typename T = std::string>
class Length : public Argument_range<T>
{
	const size_t min_size;
	const size_t max_size;

public:
	/*
	 * min_size is the minimal size must have the checked container (0 means no minimum)
	 * max_size is the maximal size must have the checked container (0 means infinite)
	 */
	Length(size_t min_size = 0, size_t max_size = 0)
	: Argument_range<T>("limited length [" + std::to_string(min_size) + ";" + (max_size?std::to_string(max_size):"inf") + "]"),
	  min_size(min_size), max_size(max_size)
	{ }

	virtual ~Length() {};

	virtual Length<T>* clone() const
	{
		return new Length(*this);
	}

	virtual void check(const T& val) const
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

}
}

#endif /* ARGUMENT_RANGE_LENGTH_HPP_ */