#ifndef ARGUMENT_TYPE_BOOLEAN_HPP_
#define ARGUMENT_TYPE_BOOLEAN_HPP_

#include <string>
#include <stdexcept>
#include <sstream>

#include "../Argument_type_limited.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = int> // shall not be of type "bool" because a val of (int 5) would be converted
                            // to (bool 1) and then can't be checked correctly as a boolean value 0 or 1
class Boolean_type : public Argument_type_limited<T,Set_range<T>>
{
public:
	Boolean_type()
	: Argument_type_limited<T,Set_range<T>>("boolean", Including_set((T)0, (T)1))
	{ }

	virtual ~Boolean_type() {};

	virtual Boolean_type<T>* clone() const
	{
		return new Boolean_type<T>();
	}

	virtual int convert(const std::string& val) const
	{
		return std::stoi(val);
	}

	virtual void check(const std::string& val) const
	{
		int bool_num;

		try
		{
			bool_num = this->convert(val);
		}
		catch(std::exception&)
		{
			throw std::runtime_error("shall be a boolean");
		}

		this->check_ranges(bool_num);
	}
};

template <typename T = int>
Boolean_type<T>* Boolean()
{
	return new Boolean_type<T>();
}

}
}
#endif /* ARGUMENT_TYPE_BOOLEAN_HPP_ */