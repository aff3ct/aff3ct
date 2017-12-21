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
	return title;
}

void* aff3ct::tools::Argument_type
::get_val(const std::string& val) const
{
	return nullptr;
}