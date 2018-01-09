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
#include "Number/Real.hpp"
#include "Number/Integer.hpp"
#include "Number/Boolean.hpp"
#include "Container/Text.hpp"
#include "Container/List.hpp"
#include "File_system/File.hpp"
#include "File_system/Folder.hpp"
#include "File_system/Path.hpp"

#endif /* ARGUMENT_TYPE_HPP_ */