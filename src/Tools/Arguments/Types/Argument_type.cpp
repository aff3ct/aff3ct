#include "Argument_type.hpp"

using namespace aff3ct::tools;

const std::string Argument_type::title_description_separator = ":";

Argument_type
::Argument_type(const std::string& title)
: title(title)
{ }

const std::string Argument_type
::get_title() const
{
	return title;
}

const std::string Argument_type
::get_short_title() const
{
	return title;
}

void* Argument_type
::get_val(const std::string& val) const
{
	return nullptr;
}