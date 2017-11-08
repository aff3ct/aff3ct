#ifndef ARGUMENT_TYPE_BASICS_HPP_
#define ARGUMENT_TYPE_BASICS_HPP_

#include <string>
#include <stdexcept>

#include "Argument_type.hpp"

namespace aff3ct
{
namespace tools
{

class None : public Argument_type
{
public:
	None()
	: Argument_type("")
	{ }

	virtual ~None() {};

	virtual None* clone() const
	{
		return new None(*this);
	}

	virtual void check(const std::string& val) const
	{
	}
};

template <typename T = std::string>
class Text : public Argument_type_limited<T>
{
public:
	Text(const std::vector<Argument_range<T>*>& ranges = {})
	: Argument_type_limited<T>("text", ranges)
	{ }

	virtual ~Text() {};

	virtual Text<T>* clone() const
	{
		auto clone = new Text<T>(*this);

		return dynamic_cast<Text<T>*>(this->clone_ranges(clone));
	}

	virtual void check(const std::string& val) const
	{
		if (val.empty())
			throw std::runtime_error("shall be a text");

		this->check_ranges(val);
	}
};

template <typename T = int>
class Integer : public Argument_type_limited<T>
{
public:
	Integer(const std::vector<Argument_range<T>*>& ranges = {})
	: Argument_type_limited<T>("integer", ranges)
	{ }

	virtual ~Integer() {};

	virtual Integer<T>* clone() const
	{
		auto clone = new Integer<T>(*this);

		return dynamic_cast<Integer<T>*>(this->clone_ranges(clone));
	}

	virtual void check(const std::string& val) const
	{
		int int_num;

		try
		{
			int_num = std::stoi(val);
		}
		catch(std::exception&)
		{
			throw std::runtime_error("shall be an integer");
		}

		this->check_ranges(int_num);
	}
};

template <typename T = float>
class Real : public Argument_type_limited<T>
{
public:
	Real(const std::vector<Argument_range<T>*>& ranges = {})
	: Argument_type_limited<T>("real number", ranges)
	{ }

	virtual ~Real() {};

	virtual Real<T>* clone() const
	{
		auto clone = new Real<T>(*this);

		return dynamic_cast<Real<T>*>(this->clone_ranges(clone));
	}

	virtual void check(const std::string& val) const
	{
		float real_num;

		try
		{
			real_num = std::stof(val);
		}
		catch(std::exception&)
		{
			throw std::runtime_error("shall be a real number");
		}

		this->check_ranges(real_num);
	}
};


}
}
#endif /* ARGUMENT_TYPE_BASICS_HPP_ */