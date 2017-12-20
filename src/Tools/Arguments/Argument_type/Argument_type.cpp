#include "Argument_type.hpp"

const std::string aff3ct::tools::Argument_type::title_description_separator = ":";

aff3ct::tools::Argument_type
::Argument_type(const std::string& title)
: title(title)
{ }

const std::string aff3ct::tools::Argument_type
::get_title() const
{
	return title;
}

const std::string aff3ct::tools::Argument_type
::get_short_title() const
{
	auto pos = title.find(title_description_separator);

	if (pos == std::string::npos)
		return title;

	return title.substr(0, pos);
}