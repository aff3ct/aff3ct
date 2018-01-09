#ifndef ARGUMENT_TYPE_HPP_
#define ARGUMENT_TYPE_HPP_

#include <string>
#include <stdexcept>

namespace aff3ct
{
namespace tools
{

class Argument_type
{
public:
	static const std::string title_description_separator;

protected:
	std::string title;

public:
	Argument_type(const std::string& title);

	virtual ~Argument_type() {};

	virtual void check(const std::string&) const = 0;
	virtual Argument_type* clone()         const = 0;

	virtual void* get_val(const std::string& val) const;

	virtual const std::string get_title() const;

	const std::string get_short_title() const;
};

}
}

#include "None.hpp"
#include "Text.hpp"
#include "Real.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "List.hpp"
#include "File.hpp"

#endif /* ARGUMENT_TYPE_HPP_ */